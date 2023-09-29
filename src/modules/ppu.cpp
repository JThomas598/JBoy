#include "memory.h"
#include "gameboy.h"
#include "ppu.h"
#include "lcd.h"
#include "util.h"
#include <stdexcept>
#include <iostream>
#include <chrono>
#include <thread>
/*
1. fetch two bytes from background map
2. decode the color of each pixel of the tile row
    for each pixel and n = 7
    i. take a copy of the msb and shift by n - 1
    ii. take a copy of the lsb amd shift by n
    iii. isolate bit 1 of the msb copy and bit 0 of the lsb copy
    iv. or the copies.
    v. use this value that must be 0-3 to determine the color of the pixel
3. convert the value to one of the pallete colors
4. 
*/

/*
TODO:
-Fix pixel drawing to accomodate fetcher
-Get Background drawing done first

*/

constexpr double FRAME_RATE = 59.7;

PPU::PPU() : 
    mem(PPU_PERM), 
    lcdcReg(mem.getRegister(LCDC_REG_ADDR)),
    intFlagReg(mem.getRegister(IF_REG_ADDR)),
    lyReg(mem.getRegister(LY_REG_ADDR)),
    statReg(mem.getRegister(STAT_REG_ADDR)),
    lycReg(mem.getRegister(LYC_REG_ADDR)),
    winYReg(mem.getRegister(WINY_REG_ADDR)),
    winXReg(mem.getRegister(WINX_REG_ADDR))
{
    window = SDL_CreateWindow("JK EMU", 
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WIN_DIMENSION_SCALE_FACTOR * SCREEN_WIDTH,
        WIN_DIMENSION_SCALE_FACTOR * SCREEN_HEIGHT,
        SDL_WINDOW_ALLOW_HIGHDPI);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    windowSurface = SDL_GetWindowSurface(window);
    frameTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);
    lcdcReg = 0x91;
    //lyReg = 0x91;
    statReg = 0x81;
    trashPixelCount = 0;
    drawingWindow = false;
    state = OAM_SEARCH;
    cyclesLeft = CYCLES_PER_LINE;
    fetchCyclesLeft = 6; 
    scanX = 0;
    lastFrameTime = std::chrono::high_resolution_clock::now();
}

PPU::~PPU(){
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyTexture(frameTexture);
}


void PPU::updateDisplay() {
    const std::chrono::duration<double> targetFrameDuration(1.0 / FRAME_RATE);

    // Calculate time since the last frame
    std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();

    //draw frame
    Uint32* pixPtr;
    SDL_LockTexture(frameTexture, NULL, (void**)&pixPtr, &windowSurface->pitch);
    memcpy((void*)pixPtr, (void*)frameBuffer, sizeof(frameBuffer));
    SDL_UnlockTexture(frameTexture);
    SDL_RenderCopy(renderer, frameTexture, NULL, NULL);
    SDL_RenderPresent(renderer);

    //TODO: Figure out how to make this properly sleep instead for resource/power efficency
    //spin until frametime has passed
    while (true){
        currentTime = std::chrono::high_resolution_clock::now();
        if((currentTime - lastFrameTime) > targetFrameDuration){
            lastFrameTime = currentTime;
            break;
        }
    }
}

void PPU::drawPixel(GbPixel pixel){
        frameBuffer[(lyReg * SCREEN_WIDTH) + scanX] = palette.getColor(pixel.palette, pixel.paletteIndex);
        scanX++;
}

void PPU::printStatus(){
    std::cout << "curr pixel X pos: " << (int)scanX << std::endl;
    std::cout << "line number: " << (int)lyReg << std::endl; 
    std::cout << "frame number: " << (int)numFrames << std::endl;
}

void PPU::changeStatMode(State state){
    switch(state){
        case H_BLANK:
            statReg &= ~0x03;break;
        case V_BLANK:
            statReg &= ~0x02;
            statReg |= 0x01;
            break;
        case OAM_SEARCH:
            statReg |= 0x02;
            statReg &= ~0x01;
            break;
        case DRAW:
            statReg |= 0x03;
        default:
            break;
    }
}

void PPU::runFSM(){
    switch(state){
        case OAM_SEARCH:
            if(cyclesLeft == CYCLES_PER_LINE - OAM_CYCLES){
                oam.searchLine(lyReg);
                state = DRAW;
                changeStatMode(state);
                break;
            }
            cyclesLeft--;
        break;
        case DRAW:
            if(fetcher.getBgFifoSize() > BG_FIFO_MIN){
                //if not drawing window and within its rectangle, start drawing it
                if(util::checkBit(lcdcReg, LCDC_WIN_EN) && scanX + 7 >= winXReg && lyReg >= winYReg && !drawingWindow){
                    drawingWindow = true;
                    fetcher.prepWinLine();
                    break;
                }

                //if a sprite occupies the current pixel, start fetching it
                if(util::checkBit(lcdcReg, LCDC_OBJ_EN)){
                    Regval8 minX = oam.getMinX();
                    if((scanX + TILE_WIDTH == minX) || (minX > 0 && minX < TILE_WIDTH)){
                        fetcher.prepSpriteFetch(oam.popObj());
                        fetchCyclesLeft = NUM_FETCH_CYCLES;
                        state = FETCH_OBJ;
                        break;
                    }
                }

                //pop pixel from fetcher and draw it onto the screen
                GbPixel pixel = fetcher.popPixel();
                drawPixel(pixel);
                if(scanX == SCREEN_WIDTH){
                    if(statReg & STAT_HBLANK_ENABLE_MASK){
                        intFlagReg |= LCD_STAT_INT;
                    }
                    state = H_BLANK;
                    changeStatMode(state);
                    oam.clearQueue();
                    break;
                }
            }
            cyclesLeft--;
            fetcher.emulateFetchCycle(); 
            break;
        case FETCH_OBJ:
            if(fetcher.emulateFetchCycle()){
                state = DRAW;
                break; 
            }
            cyclesLeft--;
            break;
        case H_BLANK:
            if(!cyclesLeft){
                //Go to next line and check for LYC interrupt
                ++lyReg;
                if((lyReg == lycReg) && (statReg & STAT_LYC_ENABLE_MASK)){
                    statReg |= STAT_LYC_FLAG_MASK;
                    intFlagReg |= LCD_STAT_INT;
                }

                //if done scanning, transition to V_BLANK and draw frame 
                if(lyReg == SCREEN_HEIGHT){
                    updateDisplay();
                    state = V_BLANK;
                    changeStatMode(state);
                    signal.raiseSignal(FRAME_SIGNAL);
                    if(statReg & STAT_VBLANK_ENABLE_MASK){
                        intFlagReg |= LCD_STAT_INT;
                    }
                    intFlagReg |= VBLANK_INT;
                    cyclesLeft = CYCLES_PER_LINE * 10;
                }

                //Otherwise, prpare to draw another line
                else{
                    fetcher.prepBgLine();
                    drawingWindow = false;
                    scanX = 0;
                    state = OAM_SEARCH;
                    changeStatMode(state);
                    cyclesLeft = CYCLES_PER_LINE;
                }
                break;
            }
            cyclesLeft--;
            break;
        case V_BLANK:
            if(cyclesLeft < 0){
                state = OAM_SEARCH;
                changeStatMode(state);
                lyReg = 0;
                scanX = 0;
                fetcher.prepBgLine();
                drawingWindow = false;
                cyclesLeft = CYCLES_PER_LINE;
                break;
            }
            cyclesLeft--;
            if(cyclesLeft % CYCLES_PER_LINE == 0){
                ++lyReg;
            }
            break;
        default:
            break; 
    }
}

void PPU::emulateCycle(){
    if(util::checkBit(lcdcReg, LCDC_LCD_EN)){
        runFSM(); 
    }
}