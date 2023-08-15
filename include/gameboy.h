#include "cpu.h"

constexpr uint16_t MEM_SIZE = (uint16_t)65535;

extern RegVal_8 mem[MEM_SIZE];

typedef enum InstrState{
    FETCH_OP,
    FETCH_1,
    FETCH_2,
    EXECUTE_1,
    EXECUTE_2,
    EXECUTE_3,
    EXECUTE_4
}InstrState;

class Gameboy{
    private:
        CPU cpu;
        uint8_t opcode;
        uint8_t cb_op;
        InstrState state;
        RegVal_8 imm_8;
        RegVal_16 imm_16;
        RegVal_8 msb;
        RegVal_8 lsb;
        //ppu comes later
        void printDebug(char* s);
    public:
        Gameboy();
        /**
            @brief Loads the game into program memory.

            @param filename filename of the desired game.

            @return total bytes read during loading process.
        */
        size_t LoadGame(std::string filename);
        /**
            @brief Emulates one machine cycle
        */
        void EmulateCycle();
        /**
            @brief Prints a summary of the current state of the system
        */
        void printStatus();
};