#include <stdint.h>
#include <stdio.h>

#define NUM_REGS_8 8
#define NUM_REGS_16 4

//FLAG MASKS
#define ZERO_FLAG 0b10000000
#define SUBTRACT_FLAG 0b01000000
#define HALF_CARRY_FLAG 0b00100000
#define CARRY_FLAG 0b00010000

typedef uint8_t RegVal_8;
typedef uint16_t RegVal_16;

typedef enum RegIndex_8{
    ACC,B,C,D,E,H,L,FLAGS
}RegIndex_8;

typedef enum RegIndex_16{
    SP,PC,IX,IY
}RegIndex_16;

class Registers
{
    private:
        RegVal_8 regs_8[NUM_REGS_8];
        RegVal_16 regs_16[NUM_REGS_16];
    public:
        /**
            @brief Constructor
        */
        Registers();
        /**
            @brief Returns the value of the indexed 8-bit register.

            @param reg the register to be read from.

            @return the 8-bit value within the requested register.
        */
        RegVal_8 Read(RegIndex_8 reg);
        /**
            @brief Returns the value of the indexed 16-bit register.

            @param reg the general purpose register to be read from.

            @return the 16-bit value within the requested register.
        */
        RegVal_16 Read(RegIndex_16 reg);
        /**
            @brief Overwrites the value of the indexed 8-bit register.

            @param reg the general purpose register to be read from.

            @return the 8-bit value within the requested register.
        */
        RegVal_8 Write(RegIndex_8 reg, RegVal_8 val);
        /**
            @brief Overwrites the value of the indexed 16-bit register.

            @param reg the register to be overwritten.

            @return the 16-bit value within the requested register.
        */
        RegVal_16 Write(RegIndex_16 reg, RegVal_16 val);
        /**
            @brief Prints a summary of the current state of the registers.
        */
        void Print();
};