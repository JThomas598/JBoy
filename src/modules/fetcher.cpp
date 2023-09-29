#include "fetcher.h"
#include "lcd.h"
#include "memory.h"
#include "util.h"
#include <queue>
#include <stdexcept>
#include <iostream> 

constexpr int BYTES_PER_TILE = 16;
constexpr int BYTES_PER_TILE_ROW = 2;
constexpr int TILE_MAP_BORDER_LEN = 32;

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
    tileMapAddr = TILE_MAP_ADDR_1;
    tileDataAddr = TILE_DATA_ADDR_1;
    fetchCyclesLeft = NUM_FETCH_CYCLES;
    drawingWindow = false; 
    mode = MAP_FETCH;
}

void Fetcher::incX(){
    //horizontally wrap around tile map
    if(++mapX == TILE_MAP_BORDER_LEN){
        mapX = 0;
    }
}

void Fetcher::fetchMapTileRow(){
    bool notSigned = util::checkBit(lcdcReg, LCDC_BG_WIN_DATA_SEL);
    uint8_t index = mem.read(tileMapAddr + (mapY*TILE_MAP_BORDER_LEN) + mapX);
    Regval16 tileRowAddr;
    Regval16 tileOffset;
    if(notSigned)
        tileOffset = (index * BYTES_PER_TILE);
    else
        tileOffset = ((int8_t)index * BYTES_PER_TILE);

    tileRowAddr = tileDataAddr + tileOffset + (mapTileRowNum * BYTES_PER_TILE_ROW);
    const Regval8 lsbTileRow = mem.read(tileRowAddr);
    const Regval8 msbTileRow = mem.read(tileRowAddr + 1);

    //chop pixels from leftmost tiles on screen
    Regval8 initialMapX = scxReg / 8;
    Regval8 numChoppedPixels = scxReg % 8;
    if(drawingWindow || mapX != initialMapX){
        numChoppedPixels = 0;
    }

    //fetch neccessary pixels
    for(int i = TILE_WIDTH - 1 - numChoppedPixels; i >= 0; i--){
        GbPixel pixel;
        pixel.paletteIndex = getPaletteIndex(msbTileRow, lsbTileRow, i);
        pixel.palette = BGP;
        bgFifo.push_back(pixel);
    }
}

void Fetcher::fetchSpriteTileRow(){
    Regval16 spriteRowNum = (lyReg + 16) - lastFetchedObj.y_pos;
    Regval8 tileRowEquation;

    if(util::checkBit(lastFetchedObj.flags, Y_FLIP)){
        //vertical mirroring process changes depending on sprite height
        if(util::checkBit(lcdcReg, LCDC_OBJ_SIZE))
            tileRowEquation = (((TILE_WIDTH * BYTES_PER_TILE_ROW) - spriteRowNum - 1) * BYTES_PER_TILE_ROW);
        else
            tileRowEquation = ((TILE_WIDTH - spriteRowNum - 1) * BYTES_PER_TILE_ROW);
    }
    else
        tileRowEquation = (spriteRowNum * BYTES_PER_TILE_ROW);

    //Sprite fetches always use base address 0x8000 (TILE_DATA_ADDR_1)
    Regval16 tileRowAddr = TILE_DATA_ADDR_1 + (lastFetchedObj.tileIndex * BYTES_PER_TILE) + tileRowEquation;
    const Regval8 lsbTileRow = mem.read(tileRowAddr);
    const Regval8 msbTileRow = mem.read(tileRowAddr + 1);
    
    //Determine if sprite is chopped
    Regval8 numChoppedPixels = 0; 
    if(lastFetchedObj.x_pos < TILE_WIDTH){
        numChoppedPixels = TILE_WIDTH - lastFetchedObj.x_pos;
    }

    //Get sprite row
    GbPixel pixel;
    //check for horizontal mirroring
    if(util::checkBit(lastFetchedObj.flags, X_FLIP)){
        for(int i = numChoppedPixels; i <=  TILE_WIDTH - 1; i++){
            pixel.paletteIndex = getPaletteIndex(msbTileRow, lsbTileRow, i);
            pixel.palette = util::checkBit(lastFetchedObj.flags, PALLETE_NUMBER) ? OBP1 : OBP0;
            spriteBuffer.push_back(pixel);
        }
    }
    else{
        for(int i = TILE_WIDTH - numChoppedPixels - 1; i >= 0; i--){
            pixel.paletteIndex = getPaletteIndex(msbTileRow, lsbTileRow, i);
            pixel.palette = util::checkBit(lastFetchedObj.flags, PALLETE_NUMBER) ? OBP1 : OBP0;
            spriteBuffer.push_back(pixel);
        }
    }
    
    //Resolve conflicts with overlapping sprites if present, otherwise, push to fifo
    if(spriteFifo.size())
        mixSprites(); 
    else
        spriteFifo = spriteBuffer;
}

void Fetcher::mixSprites(){
    std::vector<GbPixel>::iterator fifoIter = spriteFifo.begin();
    std::vector<GbPixel>::iterator bufferIter = spriteBuffer.begin();

    //overwrite any transparent pixels in sprite fifo with those of the newly fetched sprite
    while(fifoIter != spriteFifo.end()){
        if((*fifoIter).paletteIndex == COLOR_0){
            *fifoIter = *bufferIter;
        }
        fifoIter++;
        bufferIter++;
    }

    //push remaining, non-overlapping pixels into the fifo
    while(bufferIter != spriteBuffer.end()){
        spriteFifo.push_back(*bufferIter);
        bufferIter++;
    }
}

PaletteIndex Fetcher::getPaletteIndex(Regval8 msbTileRow, Regval8 lsbTileRow, Regval8 bitIndex){
        Regval8 paletteIndex = 0x00;
        paletteIndex = (msbTileRow >> bitIndex) & 0x01;
        paletteIndex <<= 1;
        paletteIndex |= (lsbTileRow >> bitIndex) & 0x01;
        return (PaletteIndex)paletteIndex;
}

GbPixel Fetcher::fifoPop(std::vector<GbPixel>&vector){
    GbPixel pixel = vector.front();
    vector.erase(vector.begin());
    return pixel;
}


void Fetcher::emulateFetchCycle(){
    switch(mode){
        case MAP_FETCH:  
            //once cycles are up, try to push to fifo until successful 
            if(!fetchCyclesLeft){
                if(bgFifo.size() <= BG_FIFO_MIN){
                    fetchMapTileRow();
                    incX();
                    fetchCyclesLeft = NUM_FETCH_CYCLES;
                }
            }
            else
                fetchCyclesLeft--;
            break;
        case SPRITE_FETCH:
            if(!fetchCyclesLeft){
                fetchSpriteTileRow();
                mode = MAP_FETCH;
                fetchCyclesLeft = NUM_FETCH_CYCLES;
            }
            else
                fetchCyclesLeft--;
            break;
        default:
            break;
    }
}

void Fetcher::clearBgFifo(){
    std::vector<GbPixel> empty;
    std::swap(bgFifo, empty);
}

void Fetcher::clearSpriteFifo(){
    std::vector<GbPixel> empty;
    std::swap(spriteFifo, empty);
}

void Fetcher::clearSpriteBuffer(){
    std::vector<GbPixel> empty;
    std::swap(spriteBuffer, empty);
}


Regval8 Fetcher::getBgFifoSize(){
    return bgFifo.size();
}

Regval8 Fetcher::getSpriteFifoSize(){
    return spriteFifo.size();
}

GBPixel Fetcher::popPixel(){
    if(bgFifo.size() <= TILE_WIDTH){
        throw std::logic_error("Fetcher::popPixel(): attempted pop with bgFifo size at or below 8 pixels");
    }
    GbPixel pixel;
    if(spriteFifo.size() > 0){
        bool spriteIsTransparent = spriteFifo.front().paletteIndex == COLOR_0;
        bool bgHasPriority = util::checkBit(lastFetchedObj.flags, MAP_OVER_OBJ);
        bool bgIsOpaque = bgFifo.front().paletteIndex > COLOR_0;
        if(spriteIsTransparent || (bgHasPriority && bgIsOpaque)){
            pixel = bgFifo.front();
        }
        else
            pixel = spriteFifo.front();
        fifoPop(spriteFifo);
    }
    else{
        pixel = bgFifo.front();
    }
    fifoPop(bgFifo);
    return pixel;
}

void Fetcher::prepBgLine(){
    fetchCyclesLeft = NUM_FETCH_CYCLES;
    mode = MAP_FETCH;
    clearBgFifo();
    clearSpriteBuffer();
    clearSpriteFifo();
    tileMapAddr = util::checkBit(lcdcReg, LCDC_BG_MAP_SEL) ? TILE_MAP_ADDR_2 : TILE_MAP_ADDR_1;
    tileDataAddr = util::checkBit(lcdcReg, LCDC_BG_WIN_DATA_SEL) ? TILE_DATA_ADDR_1 : TILE_DATA_ADDR_2;
    mapX = scxReg / TILE_WIDTH; 
    mapY = ((scyReg + lyReg) / TILE_WIDTH) % TILE_MAP_BORDER_LEN;  //vertically wrap around tilemap
    mapTileRowNum = (scyReg + lyReg) % TILE_WIDTH;
    drawingWindow = false;
}

void Fetcher::prepWinLine(){
    fetchCyclesLeft = NUM_FETCH_CYCLES;
    mode = MAP_FETCH;
    clearBgFifo();
    tileMapAddr = util::checkBit(lcdcReg, LCDC_WIN_MAP_SEL) ? TILE_MAP_ADDR_2 : TILE_MAP_ADDR_1;
    tileDataAddr = util::checkBit(lcdcReg, LCDC_BG_WIN_DATA_SEL) ? TILE_DATA_ADDR_1 : TILE_DATA_ADDR_2;
    mapX = 0;
    mapY = ((lyReg - winYReg) / TILE_WIDTH);
    mapTileRowNum = (lyReg - winYReg) % TILE_WIDTH;
    drawingWindow = true;
}

void Fetcher::prepSpriteFetch(Object obj){
    fetchCyclesLeft = NUM_FETCH_CYCLES;
    mode = SPRITE_FETCH;
    clearSpriteBuffer();
    this->lastFetchedObj = obj;
}