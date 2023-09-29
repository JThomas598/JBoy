#ifndef PPU_H
#define PPU_H
#include "memory.h"
#include "SDL2/SDL.h"
#include "fetcher.h"
#include "lcd.h"
#include "signal.h"
#include <queue>
#include <chrono>
#include <thread>

//Rendering Constants

typedef enum State{
    OAM_SEARCH,
    DRAW,
    FETCH_OBJ,
    H_BLANK,
    V_BLANK
}State;

constexpr Regval8 STAT_LYC_ENABLE_MASK = 0x40;
constexpr Regval8 STAT_OAM_ENABLE_MASK = 0x20;
constexpr Regval8 STAT_VBLANK_ENABLE_MASK = 0x10;
constexpr Regval8 STAT_HBLANK_ENABLE_MASK = 0x08;
constexpr Regval8 STAT_LYC_FLAG_MASK = 0x04;


constexpr int CYCLES_PER_LINE = 456;
constexpr int OAM_CYCLES = 20;

constexpr int WIN_DIMENSION_SCALE_FACTOR = 2;

class PPU{
    private:
        Signal signal;
        Memory mem;
        Fetcher fetcher;
        OAM oam;
        Palette palette;

        State state;

        Register lcdcReg;
        Register intFlagReg;
        Register lyReg;
        Register statReg;
        Register lycReg;
        Register winYReg;
        Register winXReg;

        bool drawingWindow;

        Regval8 trashPixelCount;

        Regval8 scanX;
        int fetchCyclesLeft;
        int cyclesLeft;
        int numFrames; 

        SDL_Window* window;
        SDL_Surface* windowSurface;
        SDL_Renderer* renderer; 
        SDL_Texture* frameTexture;
        Uint32 frameBuffer[SCREEN_WIDTH * SCREEN_HEIGHT];
        std::chrono::high_resolution_clock::time_point lastFrameTime;

        void runFSM();
        void updateDisplay();
        uint32_t resolveColor(PaletteIndex color);
        void prepLine();
        void prepWindowLine();
        void prepSpriteFetch();
        void drawPixel(GbPixel pixel);
        void changeStatMode(State state);
    public:
        PPU();
        ~PPU();

        void emulateCycle();
        void printStatus();
        
};
#endif