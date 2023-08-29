#include "memory.h"
#include "SDL2/SDL.h"
#include <queue>
#include <chrono>
#include <thread>

//Rendering Constants
constexpr int WIDTH = 160, HEIGHT = 144;
constexpr int TRUE_WIDTH = 210, TRUE_HEIGHT = 154;
constexpr int TILE_SIZE = 16;
constexpr int TILE_MAP_ROW_SIZE = 32;
constexpr int SCREEN_TILE_WIDTH = 20;
constexpr int SCREEN_TILE_HEIGHT = 18;

//Gameboy Color Palette
constexpr Uint32 ARGB_BLACK = 0xFF000000;
constexpr Uint32 ARGB_DARK_GRAY = 0xFF545454;
constexpr Uint32 ARGB_LIGHT_GRAY = 0xFFA9A9A9;
constexpr Uint32 ARGB_WHITE = 0xFFFFFFFF;


typedef enum Color{
    BLACK,
    DARK_GRAY,
    LIGHT_GRAY,
    WHITE,
}Color;

typedef enum State{
    PIX_TRANS,
    H_BLANK,
    V_BLANK
}State;

class PPU{
    private:
        std::queue<Uint32> fifo;
        SDL_Window* window;
        SDL_Surface* windowSurface;
        SDL_Renderer* renderer; 
        RegVal_8 hCount;
        RegVal_8 vCount;
        RegVal_8 mapX;
        RegVal_8 mapY;
        RegVal_8 viewportTileX;
        RegVal_8 viewportTileY;
        RegVal_8 scx;
        RegVal_8 scy;
        std::chrono::high_resolution_clock::time_point lastFrameTime;
        RegVal_8 trashPixelCount;
        RegVal_8 tileRowCount;
        
        Memory& mem;
        State state;

        Uint32 resolveColor(RegVal_8 val);
        void runFSM();
        void fetchTileRow();
        void nextTile();
        void updateDisplay();
        void drawPixel();
        void clearHangingPixels();
    public:
        PPU(Memory& memr);
        //~PPU();

        void emulateCycle();
        
};