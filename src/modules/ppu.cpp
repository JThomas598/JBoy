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

PPU::PPU() : 
    mem(PPU_PERM), 
    lcdcReg(mem.getRegister(LCDC_REG_ADDR)),
    intFlagReg(mem.getRegister(IF_REG_ADDR)),
    lyReg(mem.getRegister(LY_REG_ADDR)),
    scxReg(mem.getRegister(SCX_REG_ADDR)),
    scyReg(mem.getRegister(SCY_REG_ADDR)),
    statReg(mem.getRegister(STAT_REG_ADDR))
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
    statReg = 0x81;
    trashPixelCount = 0;
    state = OAM_SEARCH;
    cyclesLeft = CYCLES_PER_LINE;
    numFrames = 0;
    scanX = 0;
    lastFrameTime = std::chrono::high_resolution_clock::now();
}

PPU::~PPU(){
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyTexture(frameTexture);
}


void PPU::updateDisplay() {
    const double targetFrameRate = 59.7;  // Target frame rate in frames per second
    const std::chrono::duration<double> targetFrameDuration(1.0 / targetFrameRate);
    // Calculate time since the last frame
    std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = currentTime - lastFrameTime;
    Uint32* pixPtr;
    SDL_LockTexture(frameTexture, NULL, (void**)&pixPtr, &windowSurface->pitch);
    memcpy((void*)pixPtr, (void*)frameBuffer, sizeof(frameBuffer));
    SDL_UnlockTexture(frameTexture);
    SDL_RenderCopy(renderer, frameTexture, NULL, NULL);
    SDL_RenderPresent(renderer);
    std::chrono::duration<double> sleepTime = targetFrameDuration - elapsed;
    while (true){
        currentTime = std::chrono::high_resolution_clock::now();
        if((currentTime - lastFrameTime) > targetFrameDuration){
            lastFrameTime = currentTime;
            break;
        }
    }
}

void PPU::prepBackgroundLine(){
    fetcher.resetCycles();
    fetcher.setMode(MAP_FETCH);
    fetcher.clearBgFifo(); 
    Regval16 tileMapAddr = util::checkBit(lcdcReg, LCDC_BG_MAP_SEL) ? TILE_MAP_ADDR_2 : TILE_MAP_ADDR_1;
    fetcher.setMapAddr(tileMapAddr);
    Regval16 tileDataAddr = util::checkBit(lcdcReg, LCDC_BG_WIN_DATA_SEL) ? TILE_DATA_ADDR_1 : TILE_DATA_ADDR_2;
    fetcher.setTileBlockAddr(tileDataAddr);
    fetcher.setMapX(scxReg / 8);
    trashPixelCount = scxReg % 8;
    fetcher.setMapY(((scyReg + lyReg) / 8) % 32);
    fetcher.setTileRow((lyReg + scyReg) % 8);
}

//TODO: Figure out sprite overlapping issue
void PPU::prepSpriteFetch(){
    fetcher.resetCycles();
    fetcher.clearSpriteFifo();
    fetcher.setMode(SPRITE_FETCH);
}

void PPU::drawPixel(GbPixel pixel){
    if(trashPixelCount == 0){
        uint32_t rgbVal;
        palette.update(); 
        switch(pixel.palette){
            case BGP:
                rgbVal = palette.getColor(BGP, pixel.paletteIndex);
                break;
            case OBP0:
                rgbVal = palette.getColor(OBP0, pixel.paletteIndex);
                break;
            case OBP1:
                rgbVal = palette.getColor(OBP1, pixel.paletteIndex);
                break;
            default:
                throw std::logic_error("PPU::drawPixel(): Something went horribly wrong...");
        }
        frameBuffer[(lyReg * SCREEN_WIDTH) + scanX] = rgbVal;
    }
    else
        trashPixelCount--;
}

/*
FETCHER INTERFACE
-fetchMapRow()
-fetchSpriteRow()
-popPixel()
-clearMapFifo()
-clearSpriteFifo()

FETCHER PSEUDO FOR FETCHING A BG/WIN TILE ROW:

bgMapAddr = figure out bg map addr from lcdc;
winMapAddr = figure out win map addr from lcdc;
if(at or past winY AND at or past winX AND window is enabled):
	activeMapAddr = winMapAddr;
	winMap.xCoord = x; //x is the fetchers hor offset from first win tile drawn on line
	winMap.yCoord = y; //y is fetchers vert offset from first win tile drawn on frame
	tileRow = (lyReg - winY) % 8
else:
	activeMapAddr = bgMap;
	winMap.xCoord = ((mem.read(SCX) / 8) + x) & 0x1F;
	winMap.yCoord = (mem.read(SCY) + lyReg) & 0xFF;   //y is fetcher's y position on the tile map

STATE MACHINE PSEUDO:

initialization:
    mode = OAM_SEARCH;
    modeCyclesLeft = 20;
    lineCyclesLeft = CYCLES_PER_LINE; //456 dots

switch(mode){
    case OAM_SEARCH:
        if(!cyclesLeft){
            oam.searchLine(n)
        }
        cyclesLeft--;
    case DRAW_BG:
        if(at or past winY AND at or past winX AND window is enabled){
            fetcher.resetCycles();
            fetcher.clearBgFifo();
            fetcher.setMapAddr(figure out win map addr from lcdc);
            fetcher.setTileBlock(figure out window tile data location from lcdc);
            fetcher.setMapX((scanX - winX) / 8);
            fetcher.setMapY((lyReg - mem.read(winY)) / 8);
            fetcher.setTileRow((lyReg - winY) % 8)
            mode = DRAW_WIN
        }
        else if(scanX is at sprite's x coord AND objects are enabled){
            fetcher.resetCycles();
            fetcher.setMode(SPRITE);
            Object obj = oam.popObj();
            fetcher.setObj(obj); //find better name later
            mode = DRAW_OBJ;
        }
        else{
            if(fetcher.getBgFifoSize() > 8){
                GbColor color = fetcher.popPixel();
                drawPixel(color);
                scanX++;
                if(scanX == SCREEN_WIDTH){
                    mode = H_BLANK;
                }
            }
            fetcher.emulateFetchCycle();
        }
        lineCyclesLeft--;
        break;
    case DRAW_WIN:
        if(scanX is at sprite's x coord AND objects are enabled){
            fetcher.resetCycles();
            fetcher.setMode(SPRITE);
            Object obj = oam.popObj();
            fetcher.setObj(obj); //find better name later
            mode = DRAW_OBJ;
        }
        else{
            if(fetcher.getBgFifoSize() > 8){
                GbColor color = fetcher.popPixel();
                drawPixel(color);
                scanX++;
                if(scanX == SCREEN_WIDTH){
                    mode = H_BLANK;
                }
            }
            fetcher.emulateFetchCycle();
        }
        lineCyclesLeft--;
    case DRAW_OBJ:
        //later
    case H_BLANK:
        if(!lineCyclesLeft){
            if(pixY == HEIGHT - 1){
                mode = V_BLANK;
                modeCyclesLeft = 10 * CYCLES_PER_LINE;
                break;
            }
            fetcher.resetCycles();
            fetcher.setMode(MAP); 
            fetcher.setMapAddr(figure out bg map addr from lcdc);
            fetcher.setTileBlock(figure out bg tile data location from lcdc);
            fetcher.setMapX(SCX / 8);
            fetcher.setMapY(SCY / 8);
            fetcher.setTileRow(SCY % 8);
            mode = DRAW_BG;
            pixY++;
            scanX = 0;
            break;
        }
        lineCyclesLeft--;
        break;
    case V_BLANK:
        if(!lineCyclesLeft){
            fetcher.resetCycles();
            fetcher.resetCycles();
            fetcher.setMode(MAP); 
            fetcher.setMapAddr(figure out bg map addr from lcdc);
            fetcher.setTileBlock(figure out bg tile data location from lcdc);
            fetcher.setMapX(SCX / 8);
            fetcher.setMapY(SCY / 8);
            fetcher.setTileRow(SCY % 8);
            mode = DRAW_BG;
            pixY = 0;
            scanX = 0;
        }
        lineCyclesLeft--;
        break;
}
*/
void PPU::printStatus(){
    std::cout << "curr pixel X pos: " << (int)scanX << std::endl;
    std::cout << "line number: " << (int)lyReg << std::endl; 
    std::cout << "frame number: " << (int)numFrames << std::endl;
    std::cout << "tetris timer: " << (int)mem.read(0xffa6) << std::endl;
}

void PPU::runFSM(){
    switch(state){
        case OAM_SEARCH:
            if(cyclesLeft == CYCLES_PER_LINE - OAM_CYCLES){
                oam.searchLine(lyReg);
                state = DRAW_BG_WIN;
                break;
            }
            cyclesLeft--;
        break;
        case DRAW_BG_WIN:
            if(fetcher.getBgFifoSize() > BG_FIFO_MIN){
                if(scanX + 8 == oam.getMinX() && util::checkBit(lcdcReg, LCDC_OBJ_EN)){
                    fetcher.beginSpriteFetch(oam.popObj());
                    prepSpriteFetch();
                    state = FETCH_OBJ;
                    break;
                }
                GbPixel pixel = fetcher.popPixel();
                drawPixel(pixel);
                if(++scanX == SCREEN_WIDTH){
                    state = H_BLANK;
                    oam.clearQueue();
                    break;
                }
            }
            cyclesLeft--;
            fetcher.emulateFetchCycle(); 
            break;
        case FETCH_OBJ:
            if(fetcher.getSpriteFifoSize()){
                state = DRAW_BG_WIN;
                break; 
            }
            fetcher.emulateFetchCycle(); 
            cyclesLeft--;
            break;
        case H_BLANK:
            if(!cyclesLeft){
                ++lyReg;
                if(lyReg == SCREEN_HEIGHT){
                    updateDisplay();
                    numFrames++;
                    state = V_BLANK;
                    intFlagReg |= VBLANK_INT;
                    cyclesLeft = CYCLES_PER_LINE * 10;
                    break;
                }
                prepBackgroundLine();
                scanX = 0;
                state = OAM_SEARCH;
                cyclesLeft = CYCLES_PER_LINE;
                break;
            }
            cyclesLeft--;
            break;
        case V_BLANK:
            if(!cyclesLeft){
                state = OAM_SEARCH;
                signal.raiseSignal(FRAME_SIGNAL);
                prepBackgroundLine();
                cyclesLeft = CYCLES_PER_LINE;
                mem.write(lyReg, 0);
                scanX = 0;
                lyReg = 0; 
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