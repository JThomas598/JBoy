#include "instructions.h"
#include <stdlib.h>
#include <iostream>
#include <stdint.h>
#include <stdio.h>

#define NUM_REGS_8 8
#define NUM_REGS_16 4

#define ROM_BANK_SIZE 32768
#define WRAM_SIZE 8192
#define EX_RAM_SIZE 8192

//FLAG MASKS
#define ZERO_FLAG 0b10000000
#define SUBTRACT_FLAG 0b01000000
#define HALF_CARRY_FLAG 0b00100000
#define CARRY_FLAG 0b00010000

typedef uint8_t RegVal_8;
typedef uint16_t RegVal_16;

typedef enum RegIndex_8{
    A,B,C,D,E,F,H,L
}RegIndex_8;

typedef enum RegIndex_16{
    SP,PC,IX,IY
}RegIndex_16;

class CPU{
    private:
        RegVal_8 regs_8[NUM_REGS_8];
        RegVal_16 regs_16[NUM_REGS_16];
        RegVal_8 rom[ROM_BANK_SIZE];
        RegVal_8 wram[WRAM_SIZE];
        RegVal_8 ex_ram[EX_RAM_SIZE];

        /**
            @brief Swaps the upper and lower nibble of the indexed 8-bit register.

            @param reg the register to be swapped.

            @return the new swapped value of the register.
        */
        RegVal_8 Swap(RegIndex_8 reg);
        /**
            @brief Shifts the indexed register rightward.

            @param reg the register to be shifted.

            @return the new shifted value of the register.
        */
        RegVal_8 ShiftRight(RegIndex_8 reg);
        /**
            @brief Shifts the indexed register rightward.

            @param reg the register to be shifted.

            @return the new shifted value of the register.
        */
        RegVal_8 ShiftLeft(RegIndex_8 reg);
        /**
            @brief Returns the value of the indexed 8-bit register.

            @param reg the register to be read from.

            @return the 8-bit value within the requested register.
        */
        RegVal_8 GetReg(RegIndex_8 reg);
        /**
            @brief Returns the value of the indexed 16-bit register.

            @param reg the general purpose register to be read from.

            @return the 16-bit value within the requested register.
        */
        RegVal_16 GetReg(RegIndex_16 reg);
        /**
            @brief Overwrites the value of the indexed 8-bit register.

            @param reg the general purpose register to be read from.

            @return the 8-bit value within the requested register.
        */
        RegVal_8 SetReg(RegIndex_8 reg, RegVal_8 val);
        /**
            @brief Overwrites the value of the indexed 16-bit register.

            @param reg the register to be overwritten.

            @return the 16-bit value within the requested register.
        */
        RegVal_16 SetReg(RegIndex_16 reg, RegVal_16 val);
        /**
            @brief Prints a summary of the current register states.
        */
    public:
        /**
            @brief Constructor
        */
        CPU();
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