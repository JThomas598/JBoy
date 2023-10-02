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

constexpr int NUM_FETCH_CYCLES = 6;
constexpr int TILE_WIDTH = 8;

class Fetcher{
    private:
        std::vector<GbPixel> bgFifo;
        std::vector<GbPixel> spriteFifo;
        std::vector<GbPixel> spriteBuffer;

        Memory mem;

        Register lcdcReg;
        Register lyReg;
        Register scxReg;
        Register scyReg;
        Register winYReg;

        Regval8 mapX;
        Regval8 mapY;
        Regval8 pixX;
        Regval8 mapTileRowNum;
        Regval16 tileMapAddr;
        Regval16 tileDataAddr;

        Regval8 fetchCyclesLeft;

        FetcherMode mode;
        Object lastFetchedObj;

        bool drawingWindow;

        void incX();
        void fetchMapTileRow();
        void fetchSpriteTileRow();
        void mixSprites();
        PaletteIndex getPaletteIndex(Regval8 msbTileRow, Regval8 lsbTileRow, Regval8 bitIndex);
        GbPixel fifoPop(std::vector<GbPixel>&vector);
        void clearBgFifo();
        void clearSpriteBuffer();
        void clearSpriteFifo();

    public:
        Fetcher();
        bool emulateFetchCycle();
        Regval8 getBgFifoSize();
        Regval8 getSpriteFifoSize();
        GBPixel popPixel();
        void prepBgLine();
        void prepWinLine();
        void prepSpriteFetch(Object obj);
};
#endif