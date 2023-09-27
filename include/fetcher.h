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
        Register scxReg;
        Register scyReg;
        Register winYReg;
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
        void fetchMapTileRow();
        void fetchSpriteTileRow();
        void clearBgFifo();
        void clearSpriteFifo();

    public:
        Fetcher();
        void runFSM();
        void resetCycles();
        void emulateFetchCycle();
        void beginSpriteFetch(Object obj);
        Regval8 getBgFifoSize();
        Regval8 getSpriteFifoSize();
        GBPixel popPixel();
        void prepBgLine();
        void prepWinLine();
        void prepSpriteFetch();
};
#endif