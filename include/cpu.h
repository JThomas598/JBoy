#ifndef CPU_H
#define CPU_H

#include "memory.h"
#include <stdlib.h>
#include <iostream>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

//Number of 8/16-bit Registers
constexpr int NUM_REGS_8 = 8;
constexpr int NUM_REGS_16 = 4;

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
}Condition;

typedef enum BitIndex{
    BIT_ZERO = 0x01,
    BIT_ONE = 0x02,
    BIT_TWO = 0x04,
    BIT_THREE = 0x08,
    BIT_FOUR = 0x10,
    BIT_FIVE = 0x20,
    BIT_SIX = 0x40,
    BIT_SEVEN = 0x80
}BitIndex;

typedef enum Operation{
    ADD,
    SUB,
}Operation;


typedef enum RegIndex_8{
    A,B,C,D,E,F,H,L
}RegIndex_8;

typedef enum RegIndex_16{
    SP,PC,IX,IY
}RegIndex_16;

class CPU{
    private:
        Regval8 regs_8[8];
        Regval16 regs_16[4];
        Memory mem;

        bool fullCarry(Regval8 val1, Regval8 val2, Operation op, bool withCarry);
        bool halfCarry(Regval8 prev, Regval8 curr, Operation op, bool withCarry);
        Regval16 getRegPair(RegIndex_8 msr, RegIndex_8 lsr);
        void setRegPair(RegIndex_8 msr, RegIndex_8 lsr, Regval16 val);
        void addRegRegCarry(RegIndex_8 reg1, RegIndex_8 reg2);
        void pushPC();
        void pushPCInc();
        void popPC();
        
    public:
        Regval16 callInt(Regval16 addr);
        /**
            @brief Constructor

            @param mem reference to memory handler
        */
        CPU();
        /**
            @brief  Adds a GP register's value to reg A.

            @param reg register to add.

            @return New accumulator value.
        */
        Regval8 addReg(RegIndex_8 reg);
        /**
            @brief  Adds the contents of the address in HL to reg A.

            @return New accumulator value.
        */
        Regval8 addIndirect();
        /**
            @brief  Adds an unsigned immediate to reg A.

            @param reg Immediate to add.

            @return New accumulator value.
        */
        Regval8 addImm(Regval8 imm);
        /**
            @brief  Adds a GP register's value and the carry flag to reg A.

            @param reg register to add.

            @return New accumulator value.
        */
        Regval8 addRegCarry(RegIndex_8 reg);
        /**
            @brief  Adds the contents of the address in HL,
            along with the carry flag, to reg A.

            @return New accumulator value.
        */
        Regval8 addIndirectCarry();
        /**
            @brief  Adds an unsigned immediate and the carry flag to reg A.

            @param imm Immediate to add.

            @return New accumulator value.
        */
        Regval8 addImmCarry(Regval8 imm);
        /**
            @brief  Subtracts a GP register's value from reg A.

            @param reg register to subtract.

            @return New accumulator value.
        */
        Regval8 subReg(RegIndex_8 reg);
        /**
            @brief  Subtracts the contents of the address in HL from reg A.

            @return New accumulator value.
        */
        Regval8 subIndirect();
        /**
            @brief  Subtracts an unsigned immediate from reg A.

            @param imm Immediate to subtract.

            @return New accumulator value.
        */
        Regval8 subImm(Regval8 imm);
        /**
            @brief  Subtracts a GP register's value and the carry flag to reg A.

            @param reg register to subtract.

            @return New accumulator value.
        */
        Regval8 subRegCarry(RegIndex_8 reg);
        /**
            @brief  Subtracts the contents of the address in HL,
            along with the carry flag, from reg A.

            @return New accumulator value.
        */
        Regval8 subIndirectCarry();
        /**
            @brief  Subtracts an unsigned immediate and the carry flag from reg A.

            @param imm Immediate to subtract.

            @return New accumulator value.
        */
        Regval8 subImmCarry(Regval8 imm);
        /**
            @brief Subtracts a GP register from reg A (without modifying it) and sets flags
            accordingly.

            @param reg Register to compare.

            @return Flag register state.
        */
       Regval8 compareReg(RegIndex_8 reg);
        /**
            @brief Subtracts the contents of the address in HL from reg A 
            (without modifying it) and sets flags accordingly.

            @return Flag register state
        */
       Regval8 compareIndirect();
        /**
            @brief Subtracts an immediate from reg A (without modifying it) and sets flags accordingly.

            @param imm Immediate to compare.

            @return Flag register state
        */
       Regval8 compareImm(Regval8 imm);
        /**
            @brief Increments a GP register.

            @param reg register to increment.

            @return New register value.
        */
       Regval8 incReg(RegIndex_8 reg);
        /**
            @brief Increments the contents of the address in HL.

            @return New data value.
        */
       Regval8 incIndirect();
        /**
            @brief Decrements a GP register.

            @param reg register to Decrement.

            @return New register value.
        */
       Regval8 decReg(RegIndex_8 reg);
        /**
            @brief Decrements the contents of the address in HL.

            @return New data value.
        */
       Regval8 decIndirect();
        /**
            @brief Perform bitwise AND on reg A with GP register.

            @param reg register to AND.

            @return New accumulator value.
        */
       Regval8 andReg(RegIndex_8 reg);
        /**
            @brief Perform bitwise AND on reg A with memory location in HL register pair.

            @return New accumulator value.
        */
       Regval8 andIndirect();
        /**
            @brief Perform bitwise AND on reg A with immediate.

            @param imm immediate to AND.

            @return New accumulator value.
        */
       Regval8 andImm(Regval8 imm);
        /**
            @brief Perform bitwise OR on reg A with GP register.

            @param reg register to OR.

            @return New accumulator value.
        */
       Regval8 orReg(RegIndex_8 reg);
        /**
            @brief Perform bitwise OR on reg A with memory location in HL register pair.

            @return New accumulator value.
        */
       Regval8 orIndirect();
        /**
            @brief Perform bitwise OR on reg A with immediate.

            @param imm immediate to OR.

            @return New accumulator value.
        */
       Regval8 orImm(Regval8 imm);
        /**
            @brief Perform bitwise XOR on reg A with GP register.

            @param reg register to XOR.

            @return New accumulator value.
        */
       Regval8 xorReg(RegIndex_8 reg);
        /**
            @brief Perform bitwise XOR on reg A with memory location in HL register pair.

            @return New accumulator value.
        */
       Regval8 xorIndirect();
        /**
            @brief Perform bitwise XOR on reg A with immediate.

            @param imm immediate to XOR.

            @return New accumulator value.
        */
       Regval8 xorImm(Regval8 imm);
        /**
            @brief Perform bitwise XOR on reg A with memory location in HL register pair.

            @return New accumulator value.
        */
        /**
            @brief Flips the carry flag, and clears the N/H flags.

            @return New flag value.
        */
       Regval8 compCarryFlag();
        /**
            @brief Sets the carry flag, and clears the N/H flags.

            @return New flag value.
        */
       Regval8 setCarryFlag();
        /**
            @brief Figuring this one out...

            @return New accumulator value.
        */
       Regval8 decimalAdjustAcc();
        /**
            @brief Flips all the bits in reg A and sets the N/H flags.

            @return New accumulator value.
        */
       Regval8 compAcc();
       /**
        * @brief Load a GP register's value into another GP register.
        * 
        * @param dst destination register
        *
        * @param src source register
        * 
        * @return Value in destination register
        */
       Regval8 loadRegReg(RegIndex_8 dst, RegIndex_8 src);
       /**
        * @brief Loads an immediate value to a GP register.
        * 
        * @param reg destination register
        *
        * @param imm immediate value
        * 
        * @return Value in destination register
        */
       Regval8 loadRegImm(RegIndex_8 reg, Regval8 imm);
       /**
        * @brief Load the contents of the address in HL to a GP register.
        * 
        * @param reg destination register
        * 
        * @return Value in destination register
        */
       Regval8 loadRegIndirect(RegIndex_8 reg);
       /**
        * @brief Stores a GP register at the address in HL.
        * 
        * @param reg source register
        * 
        * @return Value in specified location
        */
       Regval8 loadIndirectReg(RegIndex_8 reg);
       /**
        * @brief Stores an immediate value at the address in HL.
        * 
        * @param imm immediate value
        * 
        * @return Value in specified location
        */
       Regval8 loadIndirectImm(Regval8 imm);
       /**
        * @brief Loads the contents of the address in the BC register pair into A.
        * 
        * @return Value in register A
        */
       Regval8 loadABC();
       /**
        * @brief Loads the contents of the address in the DE register pair into A.
        * 
        * @return Value in register A
        */
       Regval8 loadADE();
       /**
        * @brief Loads register A to the address in the BC register pair
        * 
        * @return Value in specified location
        */
       Regval8 loadBCA();
       /**
        * @brief Loads register A to the address in the DE register pair
        * 
        * @return Value in specified location
        */
       Regval8 loadDEA();
       /**
        * @brief Loads the contents of the address specified by a 16 bit immediate into 
        * register A.
        * 
        * @param addr immediate address
        * 
        * @return Value in register A
        */
       Regval8 loadAImmDirect(Regval16 addr);
       /**
        * @brief Loads register A to the address specified by a 16 bit immediate.
        * 
        * @param imm immediate value
        * 
        * @return Value in specified location
        */
       Regval8 loadImmADirect(Regval16 imm);
       /**
        * @brief Loads the contents of the address 0xFF00 + C into A.
        * 
        * @return Value in register A
        */
       Regval8 loadHighAC();
       /**
        * @brief Loads register A into the address 0xFF00 + C.
        * 
        * @return Value in specified location
        */
       Regval8 loadHighCA();
       /**
        * @brief Loads the contents of the address 0xFF00 + an immediate into A.
        * 
        * @param imm immediate value
        *
        * @return Value in register A
        */
       Regval8 loadHighAImm(Regval8 imm);
       /**
        * @brief Loads register A into the address 0xFF00 + an immediate.
        * 
        * @param imm immediate value
        * 
        * @return Value in specified location
        */
       Regval8 loadHighImmA(Regval8 imm);
       /**
        * @brief Loads the contents of the address in HL to A and decrements
        * HL.
        *
        * @return Value in register A
        */
       Regval8 loadAIndirectDec();
       /**
        * @brief Loads A into the address in HL and decrements HL
        *
        * @return Value in specified location
        */
       Regval8 loadIndirectADec();
       /**
        * @brief Loads the contents of the address in HL to A and increments
        * HL.
        * 
        * @return Value in specified location
        */
       Regval8 loadAIndirectInc();
       /**
        * @brief Loads A into the address in HL and increments HL
        *
        * @return Value in specified location
        */
       Regval8 loadIndirectAInc();
       /**
        * @brief Loads a 16 bit immediate to a GP register pair.
        * 
        * @param msr register containing the upper byte
        * 
        * @param lsr register containing the lower byte
        * 
        * @param imm immediate 16-bit value
        * 
        * @return Value in register pair
        */
       Regval16 loadRegPairImm(RegIndex_8 msr, RegIndex_8 lsr, Regval16 imm);
       /**
        * @brief Loads a 16 bit immediate to the SP register.
        * 
        * @param imm immediate 16-bit value
        * 
        * @return Value in SP
        */
       Regval16 loadSPImm(Regval16 imm);
       /**
        * @brief Loads stack pointer to the address specified by a 16-bit immediate.
        * 
        * @param addr immediate addr
        * 
        * @return Value in specified location
        */
       Regval16 loadDirectSP(Regval16 addr);
       /**
        * @brief Loads stack pointer with HL.
        * 
        * @return Value in SP
        */
       Regval16 loadSPHL();
       /**
        * @brief Pushes 16-bit value in GP register pair to stack, also moving
        * stack pointer accordingly.
        * 
        * @param msr register containing upper byte
        * 
        * @param lsr register containing lower byte
        * 
        * @return New stack pointer value
        */
       Regval16 push(RegIndex_8 msr, RegIndex_8 lsr);
       /**
        * @brief Pops a 16-bit value off the stack into GP register pair 
        * ,also moving the stack pointer accordingly.
        * 
        * @param msr register containing upper byte
        * 
        * @param lsr register containing lower byte
        * 
        * @return New stack pointer value
        */
       Regval16 pop(RegIndex_8 msr, RegIndex_8 lsr);
       /**
        * @brief Jump to an immediate address.
        * 
        * @param addr immediate address
        * 
        * @return New PC value
        */
       Regval16 jump(Regval16 addr);
       /**
        * @brief Jump to address in HL.
        * 
        * @return New PC value
        */
       Regval16 jumpHL();
       /**
        * @brief Jump imm addresses away from the current address. (imm is signed)
        * 
        * @param imm signed immediate offset
        * 
        * @return New PC value
        */
       Regval16 jumpRel(int8_t imm);
       /**
        * @brief Conditionally Jump imm addresses away from the current address. (imm is signed)
        * 
        * @param imm signed immediate offset
        * 
        * @param cond condition to be checked for
        * 
        * @return true if condition passed, false if it didn't
        */
       bool jumpRelCond(int8_t imm, Condition cond);
       /**
        * @brief Conditional jump to immediate address.
        * 
        * @param addr immediate address
        * 
        * @param cond condition to be checked for
        * 
        * @return true if condition passed, false if it didn't
        */
       bool jumpCond(Regval16 addr, Condition cond);
       /**
        * @brief Call a function at an immediate address.
        * 
        * @param addr immediate address
        * 
        * @return New PC value.
        */
       Regval16 call(Regval16 addr);
       /**
        * @brief Conditionally call a function at an immediate address.
        * 
        * @param addr immediate address
        * 
        * @param cond condition to be checked for
        * 
        * @return true if condition passed, false if it didn't
        */
       bool callCond(Regval16 addr, Condition cond);
       /**
        * @brief Return from function.
        * 
        * @return New PC value.
        */
       Regval16 ret();
       /**
        * @brief Conditionally return from function.
        * 
        * @param imm immediate address
        * 
        * @return true if condition passed, false if it didn't
        */
       bool retCond(Condition cond);
       /**
        * @brief Return from interrupt handler
        * 
        * @return New PC value.
        */
       Regval16 reti();
       /**
        * @brief Call function at address specified by opcode
        * 
        * @return New PC value.
        */
       Regval16 rst(Regval8 addr);
       /**
        * @brief Perform a left circular rotation on a GP reg
        * 
        * @param reg register of interest
        * 
        * @return new register value
        */
       Regval8 rlc(RegIndex_8 reg, bool cb);
       /**
        * @brief Perform a left circular rotation on memory location in HL
        * 
        * @return new value at location
        */
       Regval8 rlcInd();
       /**
        * @brief Perform a right circular rotation on a GP reg
        * 
        * @param reg register of interest
        * 
        * @return new value at location
        */
       Regval8 rrc(RegIndex_8 reg, bool cb);
       /**
        * @brief Perform a right circular rotation on memory location in HL
        * 
        * @return new value at location
        */
       Regval8 rrcInd();
       /**
        * @brief Perform a right rotation through the carry flag on a GP reg
        * 
        * @param reg register of interest
        * 
        * @return new register value
        */
       Regval8 rr(RegIndex_8 reg, bool cb);
       /**
        * @brief Perform a right rotation through the carry flag on memory location in HL
        * 
        * @return new value at location
        */
       Regval8 rrInd();
       /**
        * @brief Perform a left rotation through the carry flag on a GP reg
        * 
        * @param reg register of interest
        * 
        * @return new register value
        */
       Regval8 rl(RegIndex_8 reg, bool cb);
       /**
        * @brief Perform a left rotation through the carry flag on memory location in HL
        * 
        * @return new value at location
        */
       Regval8 rlInd();
       /**
        * @brief Perform a left arithmetic shift on GP reg 
        * 
        * @param reg register of interest
        * 
        * @return new register value
        */
       Regval8 sla(RegIndex_8 reg);
       /**
        * @brief Perform a left arithmetic shift on memory location in HL
        * 
        * @return new value at location
        */
       Regval8 slaInd();
       /**
        * @brief Perform a right arithmetic shift on GP reg 
        * 
        * @param reg register of interest
        * 
        * @return new register value
        */
       Regval8 sra(RegIndex_8 reg);
       /**
        * @brief Perform a right arithmetic shift on memory location in HL
        * 
        * @return new value at location
        */
       Regval8 sraInd();
       /**
        * @brief perform a right logical shift on a GP reg
        * 
        * @param reg register of interest
        * 
        * @return new register value
        */
       Regval8 srl(RegIndex_8 reg);
       /**
        * @brief perform a right logical shift on memory location in HL
        * 
        * @return new value at location
        */
       Regval8 srlInd();
       /**
        * @brief swap the upper and lower nibbles of a GP reg
        * 
        * @param reg register of interest
        * 
        * @return new register value
        */
       Regval8 swap(RegIndex_8 reg);
       /**
        * @brief swap the upper and lower nibbles of memory location in HL
        * 
        * @return new value at location
        */
       Regval8 swapInd();
       /**
        * @brief test a bit in a GP reg and set zero flag if its cleared
        * 
        * @param reg register of interest
        * 
        * @param index bit to be tested
        * 
        * @return new register value
        */
       Regval8 bit(RegIndex_8 reg, BitIndex index);
       /**
        * @brief test a bit at memory location in HL and set zero flag if it's cleared
        * 
        * @param index bit to be tested
        * 
        * @return new value at location
        */
       Regval8 bitInd(BitIndex index);
       /**
        * @brief clear a bit in a GP reg
        * 
        * @param reg register of interest
        * 
        * @param index bit to be cleared
        * 
        * @return new register value
        */
       Regval8 res(RegIndex_8 reg, BitIndex index);
       /**
        * @brief clear a bit at memory location in HL
        * 
        * @param index bit to be cleared
        * 
        * @return new value at location
        */
       Regval8 resInd(BitIndex index);
       /**
        * @brief set a bit in a GP reg
        * 
        * @param reg register of interest
        * 
        * @param index bit to be set
        * 
        * @return new register value
        */
       Regval8 set(RegIndex_8 reg, BitIndex index);
       /**
        * @brief set a bit in a memory location 
        * 
        * @param index bit to be set
        * 
        * @return new value at location 
        */
       Regval8 setInd(BitIndex index);
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
       Regval16 incBC();
       /**
        * @brief Increment DE.
        * 
        * @return New DE value
        */
       Regval16 incDE();
       /**
        * @brief Increment HL.
        * 
        * @return New HL value
        */
       Regval16 incHL();
       /**
        * @brief Increment Stack Pointer.
        * 
        * @return New stack pointer value
        */
       Regval16 incSP();
       /**
        * @brief Decrement BC.
        * 
        * @return New BC value
        */
       Regval16 decBC();
       /**
        * @brief Decrement DE.
        * 
        * @return New DE value
        */
       Regval16 decDE();
       /**
        * @brief Decrement HL.
        * 
        * @return New HL value
        */
       Regval16 decHL();
       /**
        * @brief Decrement Stack Pointer.
        * 
        * @return New stack pointer value
        */
       Regval16 decSP();
       /**
        * @brief Add a GP register pair to HL
        * 
        * @param msr most significant register
        * 
        * @param lsr least significant register
        * 
        * @return New HL value.
        */
       Regval16 addHLRegPair(RegIndex_8 msr, RegIndex_8 lsr);
       /**
        * @brief Add SP to HL
        * 
        * @return New HL value.
        */
       Regval16 addHLSP();
       /**
        * @brief load SP plus an offset to HL
        * 
        * @return New HL value.
        */
       Regval16 loadHLSPOffset(int8_t imm);
       /**
        * @brief Add signed immediate to stack pointer.
        * 
        * @return New HL value.
        */
       Regval16 addSPImm(int8_t imm);
       /**
        * @brief Increment program counter.
        * 
        * @return New PC value.
        */
       Regval16 incPC();
       /**
        * @brief Get program counter value
        * 
        * @return New PC value.
        */
       Regval16 getPC();

       /**
        * @brief Get 8-bit register value
        * 
        * @param reg register of interest
        * 
        * @return Register value
        */
       Regval8 getReg(RegIndex_8 reg);

       /**
        * @brief Get 16-bit register value
        * 
        * @param reg register of interest
        * 
        * @return Register value
        */
       Regval16 getReg(RegIndex_16 reg);

       void printStatus();

       void printReg(RegIndex_8 reg);
       void printReg(RegIndex_16 reg);
       void printRegPair(RegIndex_8 msr, RegIndex_8 lsr);
};
#endif