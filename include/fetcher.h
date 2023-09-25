#ifndef FETCHER_H
#define FETCHER_H

#include <queue>
#include "memory.h"
#include "lcd.h"
#include "oam.h"
#include "palette.h"

typedef enum SourcePalette{
    BG,
    WINDOW,
    OBJ0,
    OBJ1,
}SourcePalette;

typedef struct GBPixel{
    PaletteSelect palette;
    PaletteIndex paletteIndex;
}GbPixel;

constexpr int BG_FIFO_MIN = 8;


typedef enum FetcherMode{
    MAP_FETCH,
    SPRITE_FETCH
}FetcherMode;

class Fetcher{
    private:
        std::queue<PaletteIndex> bgFifo;
        std::queue<PaletteIndex> spriteFifo;
        Memory mem;
        Register lcdcReg;
        Register lyReg;
        Regval8 mapX;
        Regval8 mapY;
        Regval8 tileRow;
        Regval16 tileMapAddr;
        Regval16 tileDataAddr;
        Regval8 fetchCyclesLeft;
        FetcherMode mode;
        Object obj;

        void pushTileRow();
        void incX();

    public:
        Fetcher();
        void runFSM();
        void resetCycles();
        void emulateFetchCycle();
        void beginSpriteFetch(Object obj);
        void fetchMapTileRow();
        void fetchSpriteTileRow();
        void clearBgFifo();
        void clearSpriteFifo();
        void setMapAddr(Regval16 addr);
        void setTileBlockAddr(Regval16 addr);
        void setMapX(Regval8 x);
        void setMapY(Regval8 y);
        void setTileRow(Regval8 rowNum);
        void setMode(FetcherMode mode); 
        Regval8 getBgFifoSize();
        Regval8 getSpriteFifoSize();
        GBPixel popPixel();
};
#endif