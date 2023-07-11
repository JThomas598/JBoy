#include "instructions.h"
#include <stdlib.h>
#include <iostream>
#include <stdint.h>
#include <stdio.h>

constexpr uint8_t NUM_REGS_8 = 8;
constexpr uint8_t NUM_REGS_16 = 4;

//FLAG MASKS
constexpr uint8_t ZERO_FLAG = 0b10000000;
constexpr uint8_t SUBTRACT_FLAG = 0b01000000;
constexpr uint8_t HALF_CARRY_FLAG = 0b00100000;
constexpr uint8_t CARRY_FLAG = 0b00010000;

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
    public:
        /**
            @brief Constructor
        */
        CPU();
        /**
            @brief  Adds a general purpose register's value to reg A.

            @param reg register to add.

            @return New accumulator value.
        */
        RegVal_8 AddReg(RegIndex_8 reg);
        /**
            @brief  Adds the contents of the address in the HL register pair to reg A.

            @return New accumulator value.
        */
        RegVal_8 AddIndirect();
        /**
            @brief  Adds an unsigned immediate to reg A.

            @param reg Immediate to add.

            @return New accumulator value.
        */
        RegVal_8 AddImm(RegVal_8 imm);
        /**
            @brief  Adds a general purpose register's value and the carry flag to reg A.

            @param reg register to add.

            @return New accumulator value.
        */
        RegVal_8 AddRegCarry(RegIndex_8 reg);
        /**
            @brief  Adds the contents of the address in the HL register pair,
            along with the carry flag, to reg A.

            @return New accumulator value.
        */
        RegVal_8 AddIndirectCarry();
        /**
            @brief  Adds an unsigned immediate and the carry flag to reg A.

            @param imm Immediate to add.

            @return New accumulator value.
        */
        RegVal_8 AddImmCarry(RegVal_8 imm);
        /**
            @brief  Subtracts a general purpose register's value from reg A.

            @param reg register to subtract.

            @return New accumulator value.
        */
        RegVal_8 SubReg(RegIndex_8 reg);
        /**
            @brief  Subtracts the contents of the address in the HL register pair from reg A.

            @return New accumulator value.
        */
        RegVal_8 SubIndirect();
        /**
            @brief  Subtracts an unsigned immediate from reg A.

            @param imm Immediate to subtract.

            @return New accumulator value.
        */
        RegVal_8 SubImm(RegVal_8 imm);
        /**
            @brief  Subtracts a general purpose register's value and the carry flag to reg A.

            @param reg register to subtract.

            @return New accumulator value.
        */
        RegVal_8 SubRegCarry(RegIndex_8 reg);
        /**
            @brief  Subtracts the contents of the address in the HL register pair,
            along with the carry flag, from reg A.

            @return New accumulator value.
        */
        RegVal_8 SubIndirectCarry();
        /**
            @brief  Subtracts an unsigned immediate and the carry flag from reg A.

            @param imm Immediate to subtract.

            @return New accumulator value.
        */
        RegVal_8 SubImmCarry(RegVal_8 imm);
        /**
            @brief Subtracts a general purpose register from reg A (without modifying it) and sets flags
            accordingly.

            @param reg Register to compare.

            @return New Accumulator value.
        */
       RegVal_8 CompareReg(RegIndex_8 reg);
        /**
            @brief Subtracts the contents of the address in the HL register pair from reg A 
            (without modifying it) and sets flags accordingly.

            @return Flag register state
        */
       RegVal_8 CompareIndirect();
        /**
            @brief Subtracts an immediate from reg A (without modifying it) and sets flags accordingly.

            @param imm Immediate to compare.

            @return Flag register state
        */
       RegVal_8 CompareImmediate(RegIndex_8 imm);
        /**
            @brief Increments a general purpose register.

            @param reg register to increment.

            @return New register value.
        */
       RegVal_8 IncrementReg(RegIndex_8 reg);
        /**
            @brief Increments the contents of the address in the HL register pair.

            @return New data value.
        */
       RegVal_8 IncrementIndirect();
        /**
            @brief Decrements a general purpose register.

            @param reg register to Decrement.

            @return New register value.
        */
       RegVal_8 DecrementReg(RegIndex_8 reg);
        /**
            @brief Decrements the contents of the address in the HL register pair.

            @return New data value.
        */
       RegVal_8 DecrementIndirect();
        /**
            @brief Perform bitwise AND on reg A with general purpose register.

            @param reg register to AND.

            @return New accumulator value.
        */
       RegVal_8 AndReg(RegIndex_8 reg);
        /**
            @brief Perform bitwise AND on reg A with contents of address in HL register pair.

            @return New accumulator value.
        */
       RegVal_8 AndIndirect();
        /**
            @brief Perform bitwise AND on reg A with immediate.

            @param imm immediate to AND.

            @return New accumulator value.
        */
       RegVal_8 AndImm(RegVal_8 imm);
        /**
            @brief Perform bitwise OR on reg A with general purpose register.

            @param reg register to OR.

            @return New accumulator value.
        */
       RegVal_8 OrReg(RegIndex_8 reg);
        /**
            @brief Perform bitwise OR on reg A with contents of address in HL register pair.

            @return New accumulator value.
        */
       RegVal_8 OrIndirect();
        /**
            @brief Perform bitwise OR on reg A with immediate.

            @param imm immediate to OR.

            @return New accumulator value.
        */
       RegVal_8 OrImm(RegVal_8 imm);
        /**
            @brief Perform bitwise XOR on reg A with general purpose register.

            @param reg register to XOR.

            @return New accumulator value.
        */
       RegVal_8 XorReg(RegIndex_8 reg);
        /**
            @brief Perform bitwise XOR on reg A with contents of address in HL register pair.

            @return New accumulator value.
        */
       RegVal_8 XorIndirect();
        /**
            @brief Perform bitwise XOR on reg A with immediate.

            @param imm immediate to XOR.

            @return New accumulator value.
        */
       RegVal_8 XorImm(RegVal_8 imm);
        /**
            @brief Perform bitwise XOR on reg A with contents of address in HL register pair.

            @return New accumulator value.
        */
        /**
            @brief Flips the carry flag, and clears the N/H flags.

            @return New flag value.
        */
       RegVal_8 CompCarryFlag();
        /**
            @brief Sets the carry flag, and clears the N/H flags.

            @return New flag value.
        */
       RegVal_8 SetCarryFlag();
        /**
            @brief Figuring this one out...

            @return New accumulator value.
        */
       RegVal_8 DecimalAdjustAcc();
        /**
            @brief Flips all the bits in reg A and sets the N/H flags.

            @return New accumulator value.
        */
       RegVal_8 CompAcc();
};