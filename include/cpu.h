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

//Condition enum
typedef enum Condition{
    ZERO,
    NOT_ZERO,
    CARRY,
    NO_CARRY,
    HALF_CARRY,
    SUBTRACT,
}Condition;

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
            @brief  Adds a GP register's value to reg A.

            @param reg register to add.

            @return New accumulator value.
        */
        RegVal_8 addAReg(RegIndex_8 reg);
        /**
            @brief  Adds the contents of the address in HL to reg A.

            @return New accumulator value.
        */
        RegVal_8 addIndirect();
        /**
            @brief  Adds an unsigned immediate to reg A.

            @param reg Immediate to add.

            @return New accumulator value.
        */
        RegVal_8 addImm(RegVal_8 imm);
        /**
            @brief  Adds a GP register's value and the carry flag to reg A.

            @param reg register to add.

            @return New accumulator value.
        */
        RegVal_8 addRegCarry(RegIndex_8 reg);
        /**
            @brief  Adds the contents of the address in HL,
            along with the carry flag, to reg A.

            @return New accumulator value.
        */
        RegVal_8 addIndirectCarry();
        /**
            @brief  Adds an unsigned immediate and the carry flag to reg A.

            @param imm Immediate to add.

            @return New accumulator value.
        */
        RegVal_8 addImmCarry(RegVal_8 imm);
        /**
            @brief  Subtracts a GP register's value from reg A.

            @param reg register to subtract.

            @return New accumulator value.
        */
        RegVal_8 subReg(RegIndex_8 reg);
        /**
            @brief  Subtracts the contents of the address in HL from reg A.

            @return New accumulator value.
        */
        RegVal_8 subIndirect();
        /**
            @brief  Subtracts an unsigned immediate from reg A.

            @param imm Immediate to subtract.

            @return New accumulator value.
        */
        RegVal_8 subImm(RegVal_8 imm);
        /**
            @brief  Subtracts a GP register's value and the carry flag to reg A.

            @param reg register to subtract.

            @return New accumulator value.
        */
        RegVal_8 subRegCarry(RegIndex_8 reg);
        /**
            @brief  Subtracts the contents of the address in HL,
            along with the carry flag, from reg A.

            @return New accumulator value.
        */
        RegVal_8 subIndirectCarry();
        /**
            @brief  Subtracts an unsigned immediate and the carry flag from reg A.

            @param imm Immediate to subtract.

            @return New accumulator value.
        */
        RegVal_8 subImmCarry(RegVal_8 imm);
        /**
            @brief Subtracts a GP register from reg A (without modifying it) and sets flags
            accordingly.

            @param reg Register to compare.

            @return New Accumulator value.
        */
       RegVal_8 compareReg(RegIndex_8 reg);
        /**
            @brief Subtracts the contents of the address in HL from reg A 
            (without modifying it) and sets flags accordingly.

            @return Flag register state
        */
       RegVal_8 compareIndirect();
        /**
            @brief Subtracts an immediate from reg A (without modifying it) and sets flags accordingly.

            @param imm Immediate to compare.

            @return Flag register state
        */
       RegVal_8 compareImmediate(RegIndex_8 imm);
        /**
            @brief Increments a GP register.

            @param reg register to increment.

            @return New register value.
        */
       RegVal_8 incrementReg(RegIndex_8 reg);
        /**
            @brief Increments the contents of the address in HL.

            @return New data value.
        */
       RegVal_8 incrementIndirect();
        /**
            @brief Decrements a GP register.

            @param reg register to Decrement.

            @return New register value.
        */
       RegVal_8 decrementReg(RegIndex_8 reg);
        /**
            @brief Decrements the contents of the address in HL.

            @return New data value.
        */
       RegVal_8 decrementIndirect();
        /**
            @brief Perform bitwise AND on reg A with GP register.

            @param reg register to AND.

            @return New accumulator value.
        */
       RegVal_8 andReg(RegIndex_8 reg);
        /**
            @brief Perform bitwise AND on reg A with contents of address in HL register pair.

            @return New accumulator value.
        */
       RegVal_8 andIndirect();
        /**
            @brief Perform bitwise AND on reg A with immediate.

            @param imm immediate to AND.

            @return New accumulator value.
        */
       RegVal_8 andImm(RegVal_8 imm);
        /**
            @brief Perform bitwise OR on reg A with GP register.

            @param reg register to OR.

            @return New accumulator value.
        */
       RegVal_8 orReg(RegIndex_8 reg);
        /**
            @brief Perform bitwise OR on reg A with contents of address in HL register pair.

            @return New accumulator value.
        */
       RegVal_8 orIndirect();
        /**
            @brief Perform bitwise OR on reg A with immediate.

            @param imm immediate to OR.

            @return New accumulator value.
        */
       RegVal_8 orImm(RegVal_8 imm);
        /**
            @brief Perform bitwise XOR on reg A with GP register.

            @param reg register to XOR.

            @return New accumulator value.
        */
       RegVal_8 xorReg(RegIndex_8 reg);
        /**
            @brief Perform bitwise XOR on reg A with contents of address in HL register pair.

            @return New accumulator value.
        */
       RegVal_8 xorIndirect();
        /**
            @brief Perform bitwise XOR on reg A with immediate.

            @param imm immediate to XOR.

            @return New accumulator value.
        */
       RegVal_8 xorImm(RegVal_8 imm);
        /**
            @brief Perform bitwise XOR on reg A with contents of address in HL register pair.

            @return New accumulator value.
        */
        /**
            @brief Flips the carry flag, and clears the N/H flags.

            @return New flag value.
        */
       RegVal_8 compCarryFlag();
        /**
            @brief Sets the carry flag, and clears the N/H flags.

            @return New flag value.
        */
       RegVal_8 setCarryFlag();
        /**
            @brief Figuring this one out...

            @return New accumulator value.
        */
       RegVal_8 decimalAdjustAcc();
        /**
            @brief Flips all the bits in reg A and sets the N/H flags.

            @return New accumulator value.
        */
       RegVal_8 compAcc();
       /**
        * @brief Load a GP register's value into another GP register.
        * 
        * @param dst destination register
        *
        * @param src source register
        * 
        * @return Value in destination register
        */
       RegVal_8 loadRegReg(RegIndex_8 dst, RegIndex_8 src);
       /**
        * @brief Loads an immediate value to a GP register.
        * 
        * @param reg destination register
        *
        * @param imm immediate value
        * 
        * @return Value in destination register
        */
       RegVal_8 loadRegImm(RegIndex_8 reg, RegVal_8 imm);
       /**
        * @brief Load the contents of the address in HL to a GP register.
        * 
        * @param reg destination register
        * 
        * @return Value in destination register
        */
       RegVal_8 loadRegIndirect(RegIndex_8 reg);
       /**
        * @brief Stores a GP register at the address in HL.
        * 
        * @param reg source register
        * 
        * @return Value in specified location
        */
       RegVal_8 loadIndirectReg(RegIndex_8 reg);
       /**
        * @brief Stores an immediate value at the address in HL.
        * 
        * @param imm immediate value
        * 
        * @return Value in specified location
        */
       RegVal_8 loadIndirectImm(RegVal_8 imm);
       /**
        * @brief Loads the contents of the address in the BC register pair into A.
        * 
        * @return Value in register A
        */
       RegVal_8 loadABC();
       /**
        * @brief Loads the contents of the address in the DE register pair into A.
        * 
        * @return Value in register A
        */
       RegVal_8 loadADE();
       /**
        * @brief Loads register A to the address in the BC register pair
        * 
        * @return Value in specified location
        */
       RegVal_8 loadBCA();
       /**
        * @brief Loads register A to the address in the DE register pair
        * 
        * @return Value in specified location
        */
       RegVal_8 loadDEA();
       /**
        * @brief Loads the contents of the address specified by a 16 bit immediate into 
        * register A.
        * 
        * @param imm immediate value
        * 
        * @return Value in register A
        */
       RegVal_8 loadAImmIndirect(RegVal_16 imm);
       /**
        * @brief Loads register A to the address specified by a 16 bit immediate.
        * 
        * @param imm immediate value
        * 
        * @return Value in specified location
        */
       RegVal_8 loadImmAIndirect(RegVal_16 imm);
       /**
        * @brief Loads the contents of the address 0xFF00 + C into A.
        * 
        * @return Value in register A
        */
       RegVal_8 loadHighAC();
       /**
        * @brief Loads register A into the address 0xFF00 + C.
        * 
        * @return Value in specified location
        */
       RegVal_8 loadHighCA();
       /**
        * @brief Loads the contents of the address 0xFF00 + an immediate into A.
        * 
        * @param imm immediate value
        *
        * @return Value in register A
        */
       RegVal_8 loadHighAImm(RegVal_8 imm);
       /**
        * @brief Loads register A into the address 0xFF00 + an immediate.
        * 
        * @param imm immediate value
        * 
        * @return Value in specified location
        */
       RegVal_8 loadHighImmA(RegVal_8 imm);
       /**
        * @brief Loads the contents of the address in HL to A and decrements
        * HL.
        *
        * @return Value in specified location
        */
       RegVal_8 loadAIndirectDec();
       /**
        * @brief Loads A into the address in HL and decrements HL
        *
        * @return Value in register A
        */
       RegVal_8 loadIndirectADec();
       /**
        * @brief Loads the contents of the address in HL to A and increments
        * HL.
        * 
        * @return Value in specified location
        */
       RegVal_8 loadAIndirectInc();
       /**
        * @brief Loads a 16 bit immediate to a GP register pair.
        * 
        * @param upper_reg register containing the upper byte
        * 
        * @param lower_reg register containing the lower byte
        * 
        * @return Value in destination register
        */
       RegVal_16 loadRegPairImm(RegIndex_8 upper_reg, RegIndex_8 lower_reg, RegVal_16 imm);
       /**
        * @brief Loads stack pointer to the address specified by a 16-bit immediate.
        * 
        * @param imm immediate value
        * 
        * @return Value in destination register
        */
       RegVal_16 loadSpIndirect(RegVal_16 imm);
       /**
        * @brief Loads stack pointer with HL.
        * 
        * @param imm immediate value
        * 
        * @return Value in destination register
        */
       RegVal_16 loadSp();
       /**
        * @brief Pushes 16-bit value in GP register pair to stack, also moving
        * stack pointer accordingly.
        * 
        * @param upper_reg register containing upper byte
        * 
        * @param lower_reg register containing lower byte
        * 
        * @return New stack value
        */
       RegVal_16 push(RegIndex_8 upper_reg, RegIndex_8 lower_reg);
       /**
        * @brief Pops a 16-bit value off the stack into GP register pair 
        * ,also moving the stack pointer accordingly.
        * 
        * @param upper_reg register containing upper byte
        * 
        * @param lower_reg register containing lower byte
        * 
        * @return Value in destination register
        */
       RegVal_16 pop(RegIndex_8 upper_reg, RegIndex_8 lower_reg);
       /**
        * @brief Jump to an immediate address.
        * 
        * @param imm immediate address
        * 
        * @return New PC value
        */
       RegVal_16 jumpImm(RegVal_16 imm);
       /**
        * @brief Jump imm addresses away from the current address. (imm is signed)
        * 
        * @param imm signed immediate offset
        * 
        * @return New PC value
        */
       RegVal_16 jumpRel(int8_t imm);
       /**
        * @brief Jump imm addresses away from the current address. (imm is signed)
        * 
        * @param imm signed immediate offset
        * 
        * @param cond condition to be checked for
        * 
        * @return New PC value
        */
       RegVal_16 jumpRelCond(int8_t imm, Condition cond);
       /**
        * @brief Conditional jump to immediate address.
        * 
        * @param imm immediate address
        * 
        * @param cond condition to be checked for
        * 
        * @return New PC value.
        */
       RegVal_16 jumpCond(RegVal_16 imm, Condition cond);
       /**
        * @brief Call a function at an immediate address.
        * 
        * @param imm immediate address
        * 
        * @return New PC value.
        */
       RegVal_16 call(RegVal_16 imm);
       /**
        * @brief Conditionally call a function at an immediate address.
        * 
        * @param imm immediate address
        * 
        * @param cond condition to be checked for
        * 
        * @return New PC value.
        */
       RegVal_16 callCond(RegVal_16 imm, Condition cond);
       /**
        * @brief Return from function.
        * 
        * @return New PC value.
        */
       RegVal_16 ret();
       /**
        * @brief Conditionally return from function.
        * 
        * @param imm immediate address
        * 
        * @return New PC value.
        */
       RegVal_16 retCond(Condition cond);
       /**
        * @brief Return from interrupt handler
        * 
        * @return New PC value.
        */
       RegVal_16 reti();
       /**
        * @brief Call function at address specified by opcode
        * 
        * @return New PC value.
        */
       RegVal_16 rst(RegVal_8 addr);
       /**
        * @brief Halt system clock.
        */
       void halt();
       /**
        * @brief Halt system and main clock
        */
       void stop();
       /**
        * @brief Disable Interrupts
        */
       void di();
       /**
        * @brief Enable Interrupts
        */
       void ei();
       /**
        * @brief Increment BC.
        * 
        * @return New BC value
        */
       RegVal_16 incBC();
       /**
        * @brief Increment DE.
        * 
        * @return New DE value
        */
       RegVal_16 incDE();
       /**
        * @brief Increment HL.
        * 
        * @return New HL value
        */
       RegVal_16 incHL();
       /**
        * @brief Increment Stack Pointer.
        * 
        * @return New stack pointer value
        */
       RegVal_16 incSP();
       /**
        * @brief Decrement BC.
        * 
        * @return New BC value
        */
       RegVal_16 decBC();
       /**
        * @brief Decrement DE.
        * 
        * @return New DE value
        */
       RegVal_16 decDE();
       /**
        * @brief Decrement HL.
        * 
        * @return New HL value
        */
       RegVal_16 decHL();
       /**
        * @brief Decrement Stack Pointer.
        * 
        * @return New stack pointer value
        */
       RegVal_16 decSP();
       /**
        * @brief Add BC to HL.
        * 
        * @return New HL value.
        */
       RegVal_16 addHLBC();
       /**
        * @brief Add DE to HL.
        * 
        * @return New HL value.
        */
       RegVal_16 addHLDE();
       /**
        * @brief Add HL to HL.
        * 
        * @return New HL value.
        */
       RegVal_16 addHLHL();
       /**
        * @brief Add SP to HL.
        * 
        * @return New HL value.
        */
       RegVal_16 addHLSP();
       /**
        * @brief Add signed immediate to stack pointer.
        * 
        * @return New HL value.
        */
       RegVal_16 addSPImm(int8_t imm);
       /**
        * @brief Increment program counter.
        * 
        * @return New PC value.
        */
       RegVal_16 incPC(RegVal_8 imm);
       /**
        * @brief Get Program Counter Value
        * 
        * @return New PC value.
        */
       RegVal_16 getPC();
};