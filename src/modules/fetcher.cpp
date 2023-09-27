#include "fetcher.h"
#include "lcd.h"
#include "memory.h"
#include "util.h"
#include <queue>
#include <stdexcept>
#include <iostream> 

constexpr int TILE_SIZE = 16;
constexpr int TILE_MAP_ROW_SIZE = 32;

Fetcher::Fetcher() :
    mem(PPU_PERM), 
    lcdcReg(mem.getRegister(LCDC_REG_ADDR)),
    lyReg(mem.getRegister(LY_REG_ADDR)),
    scxReg(mem.getRegister(SCX_REG_ADDR)),
    scyReg(mem.getRegister(SCY_REG_ADDR)),
    winYReg(mem.getRegister(WINY_REG_ADDR))
{
    mapX = 0;
    mapY = 0;
    tileRow = 0;
    tileMapAddr = TILE_MAP_ADDR_1;
    tileDataAddr = TILE_DATA_ADDR_1;
    fetchCyclesLeft = 6;
    mode = MAP_FETCH;
}

/*
if(all cycles passed){
    fetchBgTileRow();
    mapX++;
}
cyclesLeft--;
*/
void Fetcher::incX(){
    if(++mapX == TILE_MAP_ROW_SIZE){
        mapX = 0;
    }
}

void Fetcher::runFSM(){
    switch(mode){
        case MAP_FETCH:  
            if(fetchCyclesLeft == 0){
                if(bgFifo.size() <= BG_FIFO_MIN){
                    fetchMapTileRow();
                    incX();
                    fetchCyclesLeft = 6;
                }
            }
            else{
                fetchCyclesLeft--;
            }
            break;
        case SPRITE_FETCH:
            if(!fetchCyclesLeft){
                fetchSpriteTileRow();
                mode = MAP_FETCH;
                fetchCyclesLeft = 6;
            }
            else
                fetchCyclesLeft--;
            break;
        default:
            break;
    }
}

void Fetcher::fetchMapTileRow(){
    bool notSigned = util::checkBit(lcdcReg, LCDC_BG_WIN_DATA_SEL);
    uint8_t index = mem.read(tileMapAddr + (mapY*TILE_MAP_ROW_SIZE) + mapX);
    Regval16 tileRowAddr;
    if(notSigned)
        tileRowAddr = tileDataAddr + (index * TILE_SIZE) + (tileRow * 2);
    else
        tileRowAddr = tileDataAddr + ((int8_t)index * TILE_SIZE) + (tileRow * 2);
    const Regval8 lsbTileRow = mem.read(tileRowAddr);
    const Regval8 msbTileRow = mem.read(tileRowAddr + 1);
    //chop pixels from leftmost tiles on screen
    Regval8 initialMapX = scxReg / 8;
    Regval8 numChoppedPixels = scxReg % 8;
    Regval16 windowMapAddr = util::checkBit(lcdcReg, LCDC_WIN_MAP_SEL) ? TILE_MAP_ADDR_2 : TILE_MAP_ADDR_1;
    bool drawingWindow = tileMapAddr == windowMapAddr;
    if(drawingWindow || mapX != initialMapX){
        numChoppedPixels = 0;
    }
    //fetch neccessary pixels
    for(int i = 7 - numChoppedPixels; i >= 0; i--){
        Regval8 paletteIndex = 0x00;
        paletteIndex = (msbTileRow >> i) & 0x01;
        paletteIndex <<= 1;
        paletteIndex |= (lsbTileRow >> i) & 0x01;
        bgFifo.push((PaletteIndex)paletteIndex);
    }
}

//FIXME: Fix sprite overlapping problem 
void Fetcher::fetchSpriteTileRow(){
    Regval16 spriteRow = (lyReg + 16) - obj.y_pos;
    Regval8 index = obj.tileIndex;
    Regval16 tileRowAddr;
    if(util::checkBit(obj.flags, Y_FLIP)){
        if(util::checkBit(lcdcReg, LCDC_OBJ_SIZE)){
            tileRowAddr = TILE_DATA_ADDR_1 + (index * TILE_SIZE) + ((15 - spriteRow) * 2);
        }
        else{
            tileRowAddr = TILE_DATA_ADDR_1 + (index * TILE_SIZE) + ((7 - spriteRow) * 2);
        }
    }
    else
        tileRowAddr = TILE_DATA_ADDR_1 + (index * TILE_SIZE) + (spriteRow * 2);
    const Regval8 lsbTileRow = mem.read(tileRowAddr);
    const Regval8 msbTileRow = mem.read(tileRowAddr + 1);
    Regval8 choppedPixels = 0; 
    if(obj.x_pos < 8){
        choppedPixels = 8 - obj.x_pos;
    }
    //get sprite row
    if(util::checkBit(obj.flags, X_FLIP)){
        for(int i = choppedPixels; i <=  7; i++){
            Regval8 paletteIndex = 0x00;
            paletteIndex = (msbTileRow >> i) & 0x01;
            paletteIndex <<= 1;
            paletteIndex |= (lsbTileRow >> i) & 0x01;
            spriteFifo.push((PaletteIndex)paletteIndex);
        }
    }
    else{
        for(int i = 7 - choppedPixels; i >= 0; i--){
            Regval8 paletteIndex = 0x00;
            paletteIndex = (msbTileRow >> i) & 0x01;
            paletteIndex <<= 1;
            paletteIndex |= (lsbTileRow >> i) & 0x01;
            spriteFifo.push((PaletteIndex)paletteIndex);
        }
    }
}


void Fetcher::resetCycles(){
    fetchCyclesLeft = 6;
}

void Fetcher::emulateFetchCycle(){
    runFSM();
}

void Fetcher::beginSpriteFetch(Object obj){
    fetchCyclesLeft = 6;
    mode = SPRITE_FETCH;
    clearSpriteFifo();
    this->obj = obj;
}

void Fetcher::clearBgFifo(){
    std::queue<PaletteIndex> empty;
    std::swap(bgFifo, empty);
}

void Fetcher::clearSpriteFifo(){
    std::queue<PaletteIndex> empty;
    std::swap(spriteFifo, empty);
}


Regval8 Fetcher::getBgFifoSize(){
    return bgFifo.size();
}

Regval8 Fetcher::getSpriteFifoSize(){
    return spriteFifo.size();
}

GBPixel Fetcher::popPixel(){
    if(bgFifo.size() <= 8){
        throw std::logic_error("Fetcher::popPixel(): attempted pop with bgFifo size at or below 8 pixels");
    }
    GbPixel pixel;
    if(spriteFifo.size() > 0){
        if(spriteFifo.front() == COLOR_0){
            pixel.paletteIndex = bgFifo.front();
            pixel.palette = BGP;
        }
        else{
            pixel.paletteIndex = spriteFifo.front();
            if(util::checkBit(obj.flags, AttributeBitIndex::PALLETE_NUMBER))
                pixel.palette = OBP1;
            else
                pixel.palette = OBP0;

        }
        spriteFifo.pop();
    }
    else{
        pixel.paletteIndex = bgFifo.front();
        pixel.palette = BGP;
    }
    bgFifo.pop();
    return pixel;
}

void Fetcher::prepBgLine(){
    fetchCyclesLeft = 6;
    mode = MAP_FETCH;
    clearBgFifo();
    clearSpriteFifo();
    tileMapAddr = util::checkBit(lcdcReg, LCDC_BG_MAP_SEL) ? TILE_MAP_ADDR_2 : TILE_MAP_ADDR_1;
    tileDataAddr = util::checkBit(lcdcReg, LCDC_BG_WIN_DATA_SEL) ? TILE_DATA_ADDR_1 : TILE_DATA_ADDR_2;
    mapX = scxReg / 8; 
    mapY = ((scyReg + lyReg) / 8) % 32; 
    tileRow = (lyReg + scyReg) % 8;
}

void Fetcher::prepWinLine(){
    fetchCyclesLeft = 6;
    mode = MAP_FETCH;
    clearBgFifo();
    tileMapAddr = util::checkBit(lcdcReg, LCDC_WIN_MAP_SEL) ? TILE_MAP_ADDR_2 : TILE_MAP_ADDR_1;
    tileDataAddr = util::checkBit(lcdcReg, LCDC_BG_WIN_DATA_SEL) ? TILE_DATA_ADDR_1 : TILE_DATA_ADDR_2;
    mapX = 0;
    mapY = ((lyReg - winYReg) / 8);
    tileRow = (lyReg - winYReg) % 8;
}

void Fetcher::prepSpriteFetch(){
    fetchCyclesLeft = 6;
    mode = SPRITE_FETCH;
    clearSpriteFifo();
}
