#include "cpu.h"

constexpr uint16_t MEM_SIZE = 65536;

class Gameboy{
    private:
        char mem[MEM_SIZE];
        CPU cpu;
        //ppu comes later
    public:
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
            @brief Prints a summary of the current state of the registers
        */
        void Print();
};