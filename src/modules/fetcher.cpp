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
    for(int i = 7; i >= 0; i--){
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
    //get sprite row
    if(util::checkBit(obj.flags, X_FLIP)){
        for(int i = 0; i <= 7; i++){
            Regval8 paletteIndex = 0x00;
            paletteIndex = (msbTileRow >> i) & 0x01;
            paletteIndex <<= 1;
            paletteIndex |= (lsbTileRow >> i) & 0x01;
            spriteFifo.push((PaletteIndex)paletteIndex);
        }
    }
    else{
        for(int i = 7; i >= 0; i--){
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
    mode = SPRITE_FETCH;
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
