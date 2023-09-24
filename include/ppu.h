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
    DMA_TRANS,
    OAM_SEARCH,
    DRAW_BG_WIN,
    FETCH_OBJ,
    DRAW_OBJ,
    H_BLANK,
    V_BLANK
}State;


constexpr int CYCLES_PER_LINE = 456;
constexpr int OAM_CYCLES = 20;


class PPU{
    private:
        Signal signal;
        Memory mem;
        Fetcher fetcher;
        OAM oam;
        State state;
        Register lcdcReg;
        Register intFlagReg;
        Register lyReg;
        Register scxReg;
        Register scyReg;
        Register statReg;
        Regval8 trashPixelCount;
        Regval8 scanX;
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
        uint32_t resolveColor(GBColor color);
        void prepBackgroundLine();
        void prepSpriteFetch();
        void prepWindowLine();
        void drawPixel(GBColor color);
    public:
        PPU();
        ~PPU();

        void emulateCycle();
        void printStatus();
        
};
#endif