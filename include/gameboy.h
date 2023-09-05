#ifndef GAMEBOY_H
#define GAMEBOY_H

#include "cpu.h"
#include "ppu.h"
#include "memory.h"

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
    RegVal_8 regs_8[8];
    RegVal_16 regs_16[4];
}GbState;

class Gameboy{
    private:
        CPU cpu;
        PPU ppu;
        Memory mem;
        uint8_t opcode;
        uint8_t cb_op;
        InstrState state;
        RegVal_8 imm_8;
        RegVal_16 imm_16;
        RegVal_8 msb;
        RegVal_8 lsb;
        FILE* fp;

        void printDebug(char* s);
        void limitCycleRate();
        void runFSM();
        void executeCBOP();
        void printSerial();
    public:
        void loadBootRom();
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
        RegVal_16 emulateCycle();
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
};
#endif