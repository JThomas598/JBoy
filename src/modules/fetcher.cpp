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
    lyReg(mem.getRegister(LY_REG_ADDR))
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
    Regval8 bit0;
    Regval8 bit1;
    for(int i = 7; i > 0; i--){
        bit0 = (lsbTileRow >> i) & 0x01;
        bit1 = (msbTileRow >> (i-1)) & 0x02;
        bgFifo.push((GBColor)(bit1 | bit0));
    }
    //edge case
    bit0 = lsbTileRow & 0x01;
    bit1 = (msbTileRow << 1) & 0x02;
    bgFifo.push((GBColor)(bit1 | bit0));
}

void Fetcher::fetchSpriteTileRow(){
    //remember to implement logic for 8x16 sprites
    Regval16 spriteRow = (lyReg + 16) - obj.y_pos;
    Regval16 tileRowAddr;
    tileRowAddr = TILE_DATA_ADDR_1 + (obj.tileIndex * TILE_SIZE) + (spriteRow * 2);
    const Regval8 lsbTileRow = mem.read(tileRowAddr);
    const Regval8 msbTileRow = mem.read(tileRowAddr + 1);
    Regval8 bit0;
    Regval8 bit1;
    for(int i = 7; i > 0; i--){
        bit0 = (lsbTileRow >> i) & 0x01;
        bit1 = (msbTileRow >> (i-1)) & 0x02;
        spriteFifo.push((GBColor)(bit1 | bit0));
    }
    //edge case
    bit0 = lsbTileRow & 0x01;
    bit1 = (msbTileRow << 1) & 0x02;
    spriteFifo.push((GBColor)(bit1 | bit0));
}


void Fetcher::resetCycles(){
    fetchCyclesLeft = 6;
}

void Fetcher::emulateFetchCycle(){
    runFSM();
}

void Fetcher::beginSpriteFetch(Object obj){
    mode = SPRITE_FETCH;
    this->obj = obj;
}

void Fetcher::clearBgFifo(){
    std::queue<GBColor> empty;
    std::swap(bgFifo, empty);
}

void Fetcher::clearSpriteFifo(){
    std::queue<GBColor> empty;
    std::swap(spriteFifo, empty);
}

void Fetcher::setMapAddr(Regval16 addr){
    if(addr != TILE_MAP_ADDR_1 && addr != TILE_MAP_ADDR_2){
        throw std::invalid_argument("address is not a valid GB tilemap address.");
    }
    tileMapAddr = addr;
}

void Fetcher::setTileBlockAddr(Regval16 addr){
    if(addr != TILE_DATA_ADDR_1 && addr != TILE_DATA_ADDR_2){
        throw std::invalid_argument("Fetcher::setTileBlockAddr(): passed address is not a valid GB tilemap address.");
    }
    tileDataAddr = addr;
}

void Fetcher::setMapX(Regval8 x){
    if(x >= 32){
        throw std::range_error("Fetcher::setMapX(): passed value is out of tilemap range");
    }
    mapX = x;
}

void Fetcher::setMapY(Regval8 y){
    if(y >= 32){
        throw std::range_error("Fetcher::setMapy(): passed value is outside of valid range");
    }
    mapY = y;
}

void Fetcher::setTileRow(Regval8 rowNum){
    if(rowNum > 7){
        throw std::invalid_argument("Fetcher::setTileRow(): passed value is outside of valid range");
    }
    tileRow = rowNum;
}

void Fetcher::setMode(FetcherMode mode){
    this->mode = mode;
}

Regval8 Fetcher::getBgFifoSize(){
    return bgFifo.size();
}

Regval8 Fetcher::getSpriteFifoSize(){
    return spriteFifo.size();
}

GBColor Fetcher::popPixel(){
    if(bgFifo.size() <= 8){
        throw std::logic_error("Fetcher::popPixel(): attempted pop with bgFifo size at or below 8 pixels");
    }
    //insert logic of mixing sprite and bg/win data here
    GBColor retval;
    if(spriteFifo.size() > 0){
        if(spriteFifo.front() == 0)
            retval = bgFifo.front();
        else
            retval = spriteFifo.front();
        spriteFifo.pop();
        bgFifo.pop();
    }
    else{
        retval = bgFifo.front();
        bgFifo.pop();
    }
    return retval;
}
