#include "memory.h"
#include "ppu.h"
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
1. Implement scrolling.
2. Implement LCD control.
3. Chop code up into subroutines.
4. Use Renderer for hardware acceleration.
5. Make a proper destructor for the SDL stuff.
6. Use initializer properly for constructor.
7. Implement switchable pallete (probably with lookup table)
*/

PPU::PPU(Memory& memr): mem(memr)
{
    SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_HAPTIC | SDL_INIT_HAPTIC | SDL_INIT_EVENTS | SDL_INIT_NOPARACHUTE);
    window = SDL_CreateWindow("JK EMU", 
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WIDTH,
        HEIGHT,
        SDL_WINDOW_ALLOW_HIGHDPI);
    windowSurface = SDL_GetWindowSurface(window);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    mem.write(LCDC, (RegVal_8)0x91);
    mem.write(STAT, (RegVal_8)0x81);
    hCount = 0;
    vCount = 0;
    mapX = 0;
    mapY = 0;
    tileRowCount = 0;
    trashPixelCount = 0;
    viewportTileX = 0;
    viewportTileY = 0;
    mem.write(SCX, 0);
    mem.write(SCY, 0);
    state = PIX_TRANS;
    lastFrameTime = std::chrono::high_resolution_clock::now();
}


void PPU::updateDisplay() {
    const int targetFrameRate = 60;  // Target frame rate in frames per second
    const std::chrono::duration<double> targetFrameDuration(1.0 / targetFrameRate);
    // Calculate time since the last frame
    std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = currentTime - lastFrameTime;
    SDL_UpdateWindowSurface(window);
    lastFrameTime = currentTime;
    // Wait to draw next frame
    std::chrono::duration<double> sleepTime = targetFrameDuration - elapsed;
    if (sleepTime > std::chrono::duration<double>(0)) {
        std::this_thread::sleep_for(sleepTime);
    }
}

Uint32 PPU::resolveColor(RegVal_8 val){
    switch(val){
        case 0:
            return ARGB_WHITE; 
        case 1:
            return ARGB_LIGHT_GRAY;
        case 2:
            return ARGB_DARK_GRAY;
        case 3:
            return ARGB_BLACK;
        default:
            throw std::invalid_argument("[ERROR] PPU::resolveColor(): passed argument is not a valid color in the defined palette.");
    }
}

void PPU::fetchTileRow(){
    RegVal_16 tileDataStart;
    RegVal_16 tileMapStart;
    if(mem.read(LCDC) & BG_MAP_SEL){
        tileMapStart = 0x9C00; 
    }
    else{
        tileMapStart = 0x9800;
    }
    if(mem.read(LCDC) & BG_WIN_TILE_BLK_SEL){
        tileDataStart = 0x8000; 
    }
    else{
        tileDataStart = 0x9000;
    }
    uint8_t index = mem.read(tileMapStart + (mapY*TILE_MAP_ROW_SIZE) + mapX);
    RegVal_16 tileRowAddr = tileDataStart + (index * TILE_SIZE) + (tileRowCount * 2);
    const RegVal_8 lsbTileRow = mem.read(tileRowAddr);
    const RegVal_8 msbTileRow = mem.read(tileRowAddr + 1);
    RegVal_8 bit0;
    RegVal_8 bit1;
    for(int i = 7; i > 0; i--){
        bit0 = (lsbTileRow >> i) & 0x01;
        bit1 = (msbTileRow >> (i-1)) & 0x02;
        fifo.push(resolveColor(bit1 | bit0));
    }
    //edge case
    bit0 = lsbTileRow & 0x01;
    bit1 = (msbTileRow << 1) & 0x02;
    fifo.push(resolveColor(bit1 | bit0));
}

void PPU::nextTile(){
    mapX++;
    viewportTileX++;
    if(mapX == TILE_MAP_ROW_SIZE){ //wrap viewport around tilemap
        mapX = 0;
    }
    if(viewportTileX == SCREEN_TILE_WIDTH){
        viewportTileX = 0;
        tileRowCount = (tileRowCount + 1) % 8;
        if(tileRowCount == 0){
            mapY++;
            viewportTileY++;
            if(mapY == TILE_MAP_ROW_SIZE){
                mapY = 0;
            }
            if(viewportTileY == SCREEN_TILE_HEIGHT){
                viewportTileY = 0;
            }
        }
    }
}


void PPU::drawPixel(){
    if(trashPixelCount == 0){
        Uint32* pixPtr = (Uint32*)windowSurface->pixels;
        pixPtr[(vCount * WIDTH) + hCount] = fifo.front();
        hCount++;
    }
    else
        trashPixelCount--;
    fifo.pop();
}

void PPU::clearHangingPixels(){
    RegVal_8 scx = mem.read(SCX);
    if((scx % 8) != 0){
        for(int i = 0; i < (8 - (scx % 8)); i++){
            fifo.pop(); 
        }
    }
}

void PPU::runFSM(){
    switch(state){
        case PIX_TRANS:
            if(hCount == WIDTH){
                mem.write(STAT, 0x00);
                clearHangingPixels();
                std::queue<Uint32> empty;
                std::swap(fifo, empty);
                state = H_BLANK;
                break;
            }
            if(fifo.size() <= 8){
                fetchTileRow();
                nextTile();
            }
            else{
                drawPixel();
            }
            break;
        case H_BLANK:
            if(hCount == TRUE_WIDTH){
                hCount = 0;
                mem.write(LY, ++vCount);
                if(vCount == HEIGHT){
                    mem.write(STAT, 0x01);
                    state = V_BLANK;
                    SDL_UpdateWindowSurface(window);
                    //updateDisplay();
                    break;
                }
                mapX = mem.read(SCX) / 8;
                viewportTileX = 0;
                trashPixelCount = mem.read(SCX) % 8;
                mem.write(STAT, 0x03);
                state = PIX_TRANS;
                break;
            }
            hCount++;
            break;
        case V_BLANK:
            if(hCount == TRUE_WIDTH){
                hCount = 0;
                mem.write(LY, ++vCount);
                if(vCount == TRUE_HEIGHT){
                    vCount = 0;
                    mem.write(STAT, 0x03);
                    state = PIX_TRANS;
                    mapX = mem.read(SCX) / 8;
                    mapY = mem.read(SCY) / 8;
                    std::queue<Uint32> empty;
                    std::swap(fifo, empty);
                    viewportTileX = 0;
                    viewportTileY = 0;
                    trashPixelCount = mem.read(SCX) % 8;
                    tileRowCount = mem.read(SCY) % 8;
                    break;
                }
                break;
            }
            hCount++;
            break;
        default:
            break;
    }
}

void PPU::emulateCycle(){
    runFSM(); 
}