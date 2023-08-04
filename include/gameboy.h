#include "cpu.h"

constexpr uint16_t MEM_SIZE = (uint16_t)65535;

extern char mem[MEM_SIZE];

typedef enum InstrState{
    FETCH_OP,
    FETCH_1,
    FETCH_2,
    EXECUTE_1,
    EXECUTE_2,
    EXECUTE_3
}InstrState;

class Gameboy{
    private:
        CPU cpu;
        uint8_t opcode;
        InstrState state;
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