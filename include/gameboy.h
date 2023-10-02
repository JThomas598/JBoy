#ifndef GAMEBOY_H
#define GAMEBOY_H

#include "cpu.h"
#include "ppu.h"
#include "memory.h"
#include "SDL2/SDL.h"
#include "dma.h"
#include "counters.h"
constexpr uint32_t CYCLE_RATE = 4194304; //Hz

typedef enum InstrState{
    FETCH_OP,
    FETCH_1,
    FETCH_2,
    EXECUTE_1,
    EXECUTE_2,
    EXECUTE_3,
    EXECUTE_4
}InstrState;

typedef struct GbState{
    uint8_t opcode;
    Regval8 regs_8[8];
    Regval16 regs_16[4];
}GbState;


class Gameboy{
    private:
        CPU cpu;
        PPU ppu;
        DMA dma;
        Counters counters;
        Signal signal;
        Memory mem;
        Regval8 joypadBuff;
        uint8_t opcode;
        uint8_t cb_op;
        InstrState state;
        uint cyclesLeft;
        Regval8 imm_8;
        Regval16 imm_16;
        Regval8 msb;
        Regval8 lsb;
        bool IME;
 
        void printDebug(char* s);
        void limitCycleRate();
        void runFSM();
        void handleEvent(SDL_Event* event);
        void executeCBOP();
        void printSerial();
        void clearInterrupt(Regval8 mask);
        void handleInterrupt();
    public:
        bool loadSram(std::string filename);
        void saveSram(std::string filename);
        Gameboy();
        /**
            @brief Loads the game into program memory.

            @param filename filename of the desired game.

            @return total bytes read during loading process.
        */
        size_t loadGame(std::string filename);
        /**
         * @brief Emulates one machine cycle
         * 
         * @return current address of program counter
         */
        Regval16 emulateCycle();
        /**
         * @brief Gives current state of emulator
         * 
         * @return GbState struct with relevant information
         */
        GbState getState();
        /**
            @brief Prints a summary of the current state of the system
        */
        void printStatus();

        void setJoypad(Regval8 byte);
        
        Regval8 getJoypad();
        Regval8 readMem(Regval16 addr); 
};
#endif