#include "memory.h"
#include "counters.h"
#include <iostream>

constexpr int DIV_CYCLES_PER_INC = 16384;

constexpr Regval8 INPUT_CLOCK_MASK = 0x03; 
constexpr Regval8 TIMER_ENABLE_MASK= 0x04;

typedef enum InputClockSpeed{
    SPEED_0,
    SPEED_1,
    SPEED_2,
    SPEED_3
}InputClockSPEED;

Counters::Counters() : 
mem(COUNTER_PERM),
divReg(mem.getRegister(DIV_REG_ADDR)),
timaReg(mem.getRegister(TIMA_REG_ADDR)),
tmaReg(mem.getRegister(TMA_REG_ADDR)),
tacReg(mem.getRegister(TAC_REG_ADDR)),
intFlagReg(mem.getRegister(IF_REG_ADDR))
{
    divReg = 0x18;
    timaReg = 0x00;
    tmaReg = 0x00;
    tacReg = 0xF8;
    divCycleCount = 0;
    timaCycleCount = 0;
}

void Counters::emulateCycle(){
    if(divCycleCount == DIV_CYCLES_PER_INC){
        divReg++;
        divCycleCount = 0;
    }
    else
        divCycleCount++;
    int intervalLen = 0;
    switch(tacReg & INPUT_CLOCK_MASK){
        case SPEED_0:
            intervalLen = 1024;
            break;
        case SPEED_1:
            intervalLen = 16;
            break;
        case SPEED_2:
            intervalLen = 64;
            break;
        case SPEED_3:
            intervalLen = 256;
            break;
        default: break;
    }
    if(tacReg & TIMER_ENABLE_MASK){
        if(timaCycleCount == intervalLen){
            timaReg++;
            if(!timaReg){
                intFlagReg |= TIMER_INT;
                timaReg = tmaReg;
            }
            timaCycleCount = 0;
        }
        else
            timaCycleCount++;
    }
}