#include "cpu.h"
#include "memory.h"
#include <stdbool.h>
#include <stdexcept>
#include <iostream>

CPU::CPU() : mem(CPU_PERM){
    regs_8[A] = 0x01;
    regs_8[B] = 0x00;
    regs_8[C] = 0x13;
    regs_8[D] = 0x00;
    regs_8[E] = 0xD8;
    regs_8[H] = 0x01;
    regs_8[L] = 0x4D;
    regs_8[F] = 0x80;
    regs_16[PC] = 0x0100;
    regs_16[SP] = 0xFFFE;
}

bool CPU::fullCarry(Regval8 val1, Regval8 val2, Operation op, bool withCarry){
    int res;
    switch(op){
        case ADD:
            res = val1 + val2;
            if((regs_8[F] & CARRY_FLAG) && withCarry){
                res += 1;
            }
            if(res > UINT8_MAX){
                return true;
            }
            break;
        case SUB:
            res = val1 - val2;
            if((regs_8[F] & CARRY_FLAG) && withCarry){
                res -= 1;
            }
            if(res < 0){
                return true;
            }
            break;
    }
    return false;
}

bool CPU::halfCarry(Regval8 val1, Regval8 val2, Operation op, bool withCarry){
    int res;
    switch(op){
        case ADD:
            res = (val1 % 16) + (val2 % 16);
            if(((regs_8[F] & CARRY_FLAG) > 0) && withCarry){
                res += 1;
            }
            if(res > 15)
                return true;
            break;
        case SUB:
            res = (val1 % 16) - (val2 % 16);
            if((regs_8[F] & CARRY_FLAG) && withCarry)
                res -= 1;
            if(res < 0)
                return true; 
            break;
    }
    return false;
}

void CPU::addRegRegCarry(RegIndex_8 reg1, RegIndex_8 reg2){
    Regval8 prev0 = regs_8[reg1];
    Regval8 prev1 = regs_8[reg2];
    regs_8[reg1] += regs_8[reg2];
    regs_8[reg1] += regs_8[F] & CARRY_FLAG ? 1 : 0;
    if(halfCarry(prev0, prev1, ADD, true))
        regs_8[F] |= HALF_CARRY_FLAG;
    else
        regs_8[F] &= ~HALF_CARRY_FLAG;
    if(fullCarry(prev0, prev1, ADD, true)) 
        regs_8[F] |= CARRY_FLAG;
    else 
        regs_8[F] &= ~CARRY_FLAG;
    regs_8[F] &= ~SUBTRACT_FLAG;
}

Regval16 CPU::getRegPair(RegIndex_8 msr, RegIndex_8 lsr){
    Regval16 retval = 0x0000;
    retval |= regs_8[msr];
    retval = retval << 8;
    retval |= regs_8[lsr];
    return retval;
}

void CPU::setRegPair(RegIndex_8 msr, RegIndex_8 lsr, Regval16 val){
    regs_8[msr] = val >> 8;
    regs_8[lsr] = val & 0x00FF;
}


void CPU::pushPC(){
    mem.write(--regs_16[SP], regs_16[PC] >> 8);
    mem.write(--regs_16[SP], regs_16[PC] & 0x00FF);
}

void CPU::popPC(){
    Regval8 msb = 0x00;
    Regval8 lsb = 0x00;
    regs_16[PC] = 0x0000;
    lsb = mem.read(regs_16[SP]++);
    msb = mem.read(regs_16[SP]++);
    regs_16[PC] = msb;
    regs_16[PC] <<= 8;
    regs_16[PC] |= lsb;
}

Regval8 CPU::addReg(RegIndex_8 reg){
    if(fullCarry(regs_8[A], regs_8[reg], ADD, false))
        regs_8[F] |= CARRY_FLAG;
    else 
        regs_8[F] &= ~CARRY_FLAG;
    if(halfCarry(regs_8[A], regs_8[reg], ADD, false))
        regs_8[F] |= HALF_CARRY_FLAG;
    else
        regs_8[F] &= ~HALF_CARRY_FLAG;
    regs_8[A] += regs_8[reg];
    if(regs_8[A] == 0)
        regs_8[F] |= ZERO_FLAG;
    else
        regs_8[F] &= ~ZERO_FLAG;
    regs_8[F] &= ~SUBTRACT_FLAG;
    return regs_8[A];
}

Regval8 CPU::addIndirect(){
    Regval8 val = mem.read(getRegPair(H,L));
    if(fullCarry(regs_8[A], val, ADD, false)) 
        regs_8[F] |= CARRY_FLAG;
    else 
        regs_8[F] &= ~CARRY_FLAG;
    if(halfCarry(regs_8[A], val, ADD, false))
        regs_8[F] |= HALF_CARRY_FLAG;
    else
        regs_8[F] &= ~HALF_CARRY_FLAG;
    regs_8[A] += val;
    if(regs_8[A] == 0)
        regs_8[F] |= ZERO_FLAG;
    else
        regs_8[F] &= ~ZERO_FLAG;
    regs_8[F] &= ~SUBTRACT_FLAG;
    return regs_8[A];
}

Regval8 CPU::addImm(Regval8 imm){
    if(fullCarry(regs_8[A], imm, ADD, false)) 
        regs_8[F] |= CARRY_FLAG;
    else 
        regs_8[F] &= ~CARRY_FLAG;
    if(halfCarry(regs_8[A], imm, ADD, false))
        regs_8[F] |= HALF_CARRY_FLAG;
    else
        regs_8[F] &= ~HALF_CARRY_FLAG;
    regs_8[A] += imm;
    if(regs_8[A] == 0)
        regs_8[F] |= ZERO_FLAG;
    else
        regs_8[F] &= ~ZERO_FLAG;
    regs_8[F] &= ~SUBTRACT_FLAG;
    return regs_8[A];
}

Regval8 CPU::addRegCarry(RegIndex_8 reg){
    Regval8 prev0 = regs_8[A];
    Regval8 prev1 = regs_8[reg];
    regs_8[A] += regs_8[reg];
    regs_8[A] += regs_8[F] & CARRY_FLAG ? 1 : 0;
    if(halfCarry(prev0, prev1, ADD, true))
        regs_8[F] |= HALF_CARRY_FLAG;
    else
        regs_8[F] &= ~HALF_CARRY_FLAG;
    if(fullCarry(prev0, prev1, ADD, true)) 
        regs_8[F] |= CARRY_FLAG;
    else 
        regs_8[F] &= ~CARRY_FLAG;
    if(regs_8[A] == 0)
        regs_8[F] |= ZERO_FLAG;
    else
        regs_8[F] &= ~ZERO_FLAG;
    regs_8[F] &= ~SUBTRACT_FLAG;
    return regs_8[A];
}

Regval8 CPU::addIndirectCarry(){
    Regval8 prev = regs_8[A];
    Regval8 val = mem.read(getRegPair(H,L));
    regs_8[A] += val;
    regs_8[A] += regs_8[F] & CARRY_FLAG ? 1 : 0;
    if(halfCarry(prev, val, ADD, true))
        regs_8[F] |= HALF_CARRY_FLAG;
    else
        regs_8[F] &= ~HALF_CARRY_FLAG;
    if(fullCarry(prev, val, ADD, true)) 
        regs_8[F] |= CARRY_FLAG;
    else 
        regs_8[F] &= ~CARRY_FLAG;
    if(regs_8[A] == 0)
        regs_8[F] |= ZERO_FLAG;
    else
        regs_8[F] &= ~ZERO_FLAG;
    regs_8[F] &= ~SUBTRACT_FLAG;
    return regs_8[A];
}

Regval8 CPU::addImmCarry(Regval8 imm){
    Regval8 prev = regs_8[A];
    regs_8[A] += imm;
    regs_8[A] += regs_8[F] & CARRY_FLAG ? 1 : 0;
    if(halfCarry(prev, imm, ADD, true))
        regs_8[F] |= HALF_CARRY_FLAG;
    else
        regs_8[F] &= ~HALF_CARRY_FLAG;
    if(fullCarry(prev, imm, ADD, true)) 
        regs_8[F] |= CARRY_FLAG;
    else 
        regs_8[F] &= ~CARRY_FLAG;
    if(regs_8[A] == 0)
        regs_8[F] |= ZERO_FLAG;
    else
        regs_8[F] &= ~ZERO_FLAG;
    regs_8[F] &= ~SUBTRACT_FLAG;
    return regs_8[A];
}

Regval8 CPU::subReg(RegIndex_8 reg){
    Regval8 prev = regs_8[A];
    regs_8[A] -= regs_8[reg];
    if(fullCarry(prev, regs_8[reg], SUB, false)) 
        regs_8[F] |= CARRY_FLAG;
    else 
        regs_8[F] &= ~CARRY_FLAG;
    if(halfCarry(prev, regs_8[reg], SUB, false)) 
        regs_8[F] |= HALF_CARRY_FLAG;
    else
        regs_8[F] &= ~HALF_CARRY_FLAG;
    if(regs_8[A] == 0)
        regs_8[F] |= ZERO_FLAG;
    else
        regs_8[F] &= ~ZERO_FLAG;
    regs_8[F] |= SUBTRACT_FLAG;
    return regs_8[A];
}

Regval8 CPU::subIndirect(){
    Regval8 prev = regs_8[A];
    Regval8 val = mem.read(getRegPair(H,L));
    regs_8[A] -= val;
    if(fullCarry(prev, val, SUB, false)) 
        regs_8[F] |= CARRY_FLAG;
    else 
        regs_8[F] &= ~CARRY_FLAG;
    if(halfCarry(prev, val, SUB, false))
        regs_8[F] |= HALF_CARRY_FLAG;
    else
        regs_8[F] &= ~HALF_CARRY_FLAG;
    if(regs_8[A] == 0)
        regs_8[F] |= ZERO_FLAG;
    else
        regs_8[F] &= ~ZERO_FLAG;
    regs_8[F] |= SUBTRACT_FLAG;
    return regs_8[A];
}

Regval8 CPU::subImm(Regval8 imm){
    Regval8 prev = regs_8[A];
    regs_8[A] -= imm;
    if(fullCarry(prev, imm, SUB, false)) 
        regs_8[F] |= CARRY_FLAG;
    else 
        regs_8[F] &= ~CARRY_FLAG;
    if(halfCarry(prev, imm, SUB, false)) 
        regs_8[F] |= HALF_CARRY_FLAG;
    else
        regs_8[F] &= ~HALF_CARRY_FLAG;
    if(regs_8[A] == 0)
        regs_8[F] |= ZERO_FLAG;
    else
        regs_8[F] &= ~ZERO_FLAG;
    regs_8[F] |= SUBTRACT_FLAG;
    return regs_8[A];
}

Regval8 CPU::subRegCarry(RegIndex_8 reg){
    Regval8 prev = regs_8[A];
    regs_8[A] -= regs_8[reg];
    regs_8[A] -= regs_8[F] & CARRY_FLAG ? 1 : 0;
    if(halfCarry(prev, regs_8[reg], SUB, true)) 
        regs_8[F] |= HALF_CARRY_FLAG;
    else
        regs_8[F] &= ~HALF_CARRY_FLAG;
    if(fullCarry(prev, regs_8[reg], SUB, true)) 
        regs_8[F] |= CARRY_FLAG;
    else 
        regs_8[F] &= ~CARRY_FLAG;
    if(regs_8[A] == 0)
        regs_8[F] |= ZERO_FLAG;
    else
        regs_8[F] &= ~ZERO_FLAG;
    regs_8[F] |= SUBTRACT_FLAG;
    return regs_8[A];
}

Regval8 CPU::subIndirectCarry(){
    Regval8 prev = regs_8[A];
    Regval8 val = mem.read(getRegPair(H,L));
    regs_8[A] -= val;
    regs_8[A] -= regs_8[F] & CARRY_FLAG ? 1 : 0;
    if(halfCarry(prev, val, SUB, true)) 
        regs_8[F] |= HALF_CARRY_FLAG;
    else
        regs_8[F] &= ~HALF_CARRY_FLAG;
    if(fullCarry(prev, val, SUB, true)) 
        regs_8[F] |= CARRY_FLAG;
    else 
        regs_8[F] &= ~CARRY_FLAG;
    if(regs_8[A] == 0)
        regs_8[F] |= ZERO_FLAG;
    else
        regs_8[F] &= ~ZERO_FLAG;
    regs_8[F] |= SUBTRACT_FLAG;
    return regs_8[A];
}

Regval8 CPU::subImmCarry(Regval8 imm){
    Regval8 prev = regs_8[A];
    regs_8[A] -= imm;
    regs_8[A] -= regs_8[F] & CARRY_FLAG ? 1 : 0;
    if(halfCarry(prev, imm, SUB, true)) 
        regs_8[F] |= HALF_CARRY_FLAG;
    else
        regs_8[F] &= ~HALF_CARRY_FLAG;
    if(fullCarry(prev, imm, SUB, true)) 
        regs_8[F] |= CARRY_FLAG;
    else 
        regs_8[F] &= ~CARRY_FLAG;
    if(regs_8[A] == 0)
        regs_8[F] |= ZERO_FLAG;
    else
        regs_8[F] &= ~ZERO_FLAG;
    regs_8[F] |= SUBTRACT_FLAG;
    return regs_8[A];
}

Regval8 CPU::compareReg(RegIndex_8 reg){
    Regval8 prev = regs_8[A];
    Regval8 res = regs_8[A] - regs_8[reg];
    if(fullCarry(prev, regs_8[reg], SUB, false)) 
        regs_8[F] |= CARRY_FLAG;
    else 
        regs_8[F] &= ~CARRY_FLAG;
    if(halfCarry(prev, regs_8[reg], SUB, false)) 
        regs_8[F] |= HALF_CARRY_FLAG;
    else
        regs_8[F] &= ~HALF_CARRY_FLAG;
    if(res == 0)
        regs_8[F] |= ZERO_FLAG;
    else
        regs_8[F] &= ~ZERO_FLAG;
    regs_8[F] |= SUBTRACT_FLAG;
    return regs_8[F];
}

Regval8 CPU::compareIndirect(){
    Regval8 prev = regs_8[A];
    Regval8 val = mem.read(getRegPair(H,L));
    Regval8 res = regs_8[A] - val;
    if(fullCarry(prev, val, SUB, false)) 
        regs_8[F] |= CARRY_FLAG;
    else 
        regs_8[F] &= ~CARRY_FLAG;
    if(halfCarry(prev, val, SUB, false)) 
        regs_8[F] |= HALF_CARRY_FLAG;
    else
        regs_8[F] &= ~HALF_CARRY_FLAG;
    if(res == 0)
        regs_8[F] |= ZERO_FLAG;
    else
        regs_8[F] &= ~ZERO_FLAG;
    regs_8[F] |= SUBTRACT_FLAG;
    return regs_8[F];
}

Regval8 CPU::compareImm(Regval8 imm){
    Regval8 prev = regs_8[A];
    Regval8 res = regs_8[A] - imm;
    if(fullCarry(prev, imm, SUB, false)) 
        regs_8[F] |= CARRY_FLAG;
    else 
        regs_8[F] &= ~CARRY_FLAG;
    if(halfCarry(prev, imm, SUB, false)) 
        regs_8[F] |= HALF_CARRY_FLAG;
    else
        regs_8[F] &= ~HALF_CARRY_FLAG;
    if(res == 0)
        regs_8[F] |= ZERO_FLAG;
    else
        regs_8[F] &= ~ZERO_FLAG;
    regs_8[F] |= SUBTRACT_FLAG;
    return regs_8[F];
}

Regval8 CPU::incReg(RegIndex_8 reg){
    Regval8 prev = regs_8[reg];
    regs_8[reg] += 1;
    if(halfCarry(prev, 1, ADD, false))
        regs_8[F] |= HALF_CARRY_FLAG;
    else
        regs_8[F] &= ~HALF_CARRY_FLAG;
    if(regs_8[reg] == 0)
        regs_8[F] |= ZERO_FLAG;
    else
        regs_8[F] &= ~ZERO_FLAG;
    regs_8[F] &= ~SUBTRACT_FLAG;
    return regs_8[reg];
}

Regval8 CPU::incIndirect(){
    Regval8 val = mem.read(getRegPair(H,L));
    Regval8 prev = val++;
    mem.write(getRegPair(H,L), val);
    if(halfCarry(prev, 1, ADD, false))
        regs_8[F] |= HALF_CARRY_FLAG;
    else
        regs_8[F] &= ~HALF_CARRY_FLAG;
    if(val == 0)
        regs_8[F] |= ZERO_FLAG;
    else
        regs_8[F] &= ~ZERO_FLAG;
    regs_8[F] &= ~SUBTRACT_FLAG;
    return val;
}

Regval8 CPU::decReg(RegIndex_8 reg){
    Regval8 prev = regs_8[reg];
    regs_8[reg] -= 1;
    if(halfCarry(prev, 1, SUB, false))
        regs_8[F] |= HALF_CARRY_FLAG;
    else
        regs_8[F] &= ~HALF_CARRY_FLAG;
    if(regs_8[reg] == 0)
        regs_8[F] |= ZERO_FLAG;
    else
        regs_8[F] &= ~ZERO_FLAG;
    regs_8[F] |= SUBTRACT_FLAG;
    return regs_8[reg];
}

Regval8 CPU::decIndirect(){
    Regval8 val = mem.read(getRegPair(H,L));
    Regval8 prev = val--;
    mem.write(getRegPair(H,L), val);
    if(halfCarry(prev, 1, SUB, false))
        regs_8[F] |= HALF_CARRY_FLAG;
    else
        regs_8[F] &= ~HALF_CARRY_FLAG;
    if(val == 0)
        regs_8[F] |= ZERO_FLAG;
    else
        regs_8[F] &= ~ZERO_FLAG;
    regs_8[F] |= SUBTRACT_FLAG;
    return val;
}

Regval8 CPU::andReg(RegIndex_8 reg){
    regs_8[A] &= regs_8[reg];
    if(regs_8[A] == 0)
        regs_8[F] |= ZERO_FLAG;
    else
        regs_8[F] &= ~ZERO_FLAG;
    regs_8[F] &= ~CARRY_FLAG;
    regs_8[F] &= ~SUBTRACT_FLAG;
    regs_8[F] |= HALF_CARRY_FLAG;
    return regs_8[A];
}

Regval8 CPU::andIndirect(){
    Regval8 val = mem.read(getRegPair(H,L));
    regs_8[A] &= val;
    if(regs_8[A] == 0)
        regs_8[F] |= ZERO_FLAG;
    else
        regs_8[F] &= ~ZERO_FLAG;
    regs_8[F] &= ~CARRY_FLAG;
    regs_8[F] &= ~SUBTRACT_FLAG;
    regs_8[F] |= HALF_CARRY_FLAG;
    return regs_8[A];
}

Regval8 CPU::andImm(Regval8 imm){
    regs_8[A] &= imm; 
    if(regs_8[A] == 0)
        regs_8[F] |= ZERO_FLAG;
    else
        regs_8[F] &= ~ZERO_FLAG;
    regs_8[F] &= ~CARRY_FLAG;
    regs_8[F] &= ~SUBTRACT_FLAG;
    regs_8[F] |= HALF_CARRY_FLAG;
    return regs_8[A];
}

Regval8 CPU::orReg(RegIndex_8 reg){
    regs_8[A] |= regs_8[reg];
    if(regs_8[A] == 0)
        regs_8[F] |= ZERO_FLAG;
    else
        regs_8[F] &= ~ZERO_FLAG;
    regs_8[F] &= ~CARRY_FLAG;
    regs_8[F] &= ~SUBTRACT_FLAG;
    regs_8[F] &= ~HALF_CARRY_FLAG;
    return regs_8[A];
}

Regval8 CPU::orIndirect(){
    Regval8 val = mem.read(getRegPair(H,L));
    regs_8[A] |= val;
    if(regs_8[A] == 0)
        regs_8[F] |= ZERO_FLAG;
    else
        regs_8[F] &= ~ZERO_FLAG;
    regs_8[F] &= ~CARRY_FLAG;
    regs_8[F] &= ~SUBTRACT_FLAG;
    regs_8[F] &= ~HALF_CARRY_FLAG;
    return regs_8[A];
}

Regval8 CPU::orImm(Regval8 imm){
    regs_8[A] |= imm; 
    if(regs_8[A] == 0)
        regs_8[F] |= ZERO_FLAG;
    else
        regs_8[F] &= ~ZERO_FLAG;
    regs_8[F] &= ~CARRY_FLAG;
    regs_8[F] &= ~SUBTRACT_FLAG;
    regs_8[F] &= ~HALF_CARRY_FLAG;
    return regs_8[A];
}

Regval8 CPU::xorReg(RegIndex_8 reg){
    regs_8[A] ^= regs_8[reg];
    if(regs_8[A] == 0)
        regs_8[F] |= ZERO_FLAG;
    else
        regs_8[F] &= ~ZERO_FLAG;
    regs_8[F] &= ~CARRY_FLAG;
    regs_8[F] &= ~SUBTRACT_FLAG;
    regs_8[F] &= ~HALF_CARRY_FLAG;
    return regs_8[A];
}

Regval8 CPU::xorIndirect(){
    Regval8 val = mem.read(getRegPair(H,L));
    regs_8[A] ^= val;
    if(regs_8[A] == 0)
        regs_8[F] |= ZERO_FLAG;
    else
        regs_8[F] &= ~ZERO_FLAG;
    regs_8[F] &= ~CARRY_FLAG;
    regs_8[F] &= ~SUBTRACT_FLAG;
    regs_8[F] &= ~HALF_CARRY_FLAG;
    return regs_8[A];
}

Regval8 CPU::xorImm(Regval8 imm){
    regs_8[A] ^= imm; 
    if(regs_8[A] == 0)
        regs_8[F] |= ZERO_FLAG;
    else
        regs_8[F] &= ~ZERO_FLAG;
    regs_8[F] &= ~CARRY_FLAG;
    regs_8[F] &= ~SUBTRACT_FLAG;
    regs_8[F] &= ~HALF_CARRY_FLAG;
    return regs_8[A];
}

Regval8 CPU::compCarryFlag(){
    regs_8[F] ^= CARRY_FLAG;
    regs_8[F] &= ~SUBTRACT_FLAG;
    regs_8[F] &= ~HALF_CARRY_FLAG;
    return regs_8[F];
}

Regval8 CPU::setCarryFlag(){
    regs_8[F] |= CARRY_FLAG;
    regs_8[F] &= ~SUBTRACT_FLAG;
    regs_8[F] &= ~HALF_CARRY_FLAG;
    return regs_8[F];
}

Regval8 CPU::decimalAdjustAcc(){
    if(!(regs_8[F] & SUBTRACT_FLAG)){
        if((regs_8[F] & CARRY_FLAG) || (regs_8[A] > 0x99)){
            regs_8[A] += 0x60;
            regs_8[F] |= CARRY_FLAG;
        }
        if((regs_8[F] & HALF_CARRY_FLAG) || (regs_8[A] & 0x0f) > 0x09){
            regs_8[A] += 0x6;
        }
    }
    else{
        if(regs_8[F] & CARRY_FLAG)
            regs_8[A] -= 0x60;
        if(regs_8[F] & HALF_CARRY_FLAG)
            regs_8[A] -= 0x6;
    }
    if(regs_8[A] == 0){
        regs_8[F] |= ZERO_FLAG;
    }
    else{
        regs_8[F] &= ~ZERO_FLAG;
    }
    regs_8[F] &= ~HALF_CARRY_FLAG;
    return regs_8[A];
}

Regval8 CPU::compAcc(){
    regs_8[A] ^= 0xFF;
    regs_8[F] |= SUBTRACT_FLAG;
    regs_8[F] |= HALF_CARRY_FLAG;
    return regs_8[A];
}

Regval8 CPU::loadRegReg(RegIndex_8 dst,RegIndex_8 src){
    regs_8[dst] = regs_8[src];
    return regs_8[dst];
}

Regval8 CPU::loadRegImm(RegIndex_8 reg, Regval8 imm){
    regs_8[reg] = imm;
    return regs_8[reg];
}

Regval8 CPU::loadRegIndirect(RegIndex_8 reg){
    regs_8[reg] = mem.read(getRegPair(H,L));
    return regs_8[reg];
}

Regval8 CPU::loadIndirectReg(RegIndex_8 reg){
    return mem.write(getRegPair(H,L), regs_8[reg]);
}

Regval8 CPU::loadIndirectImm(Regval8 imm){
    return mem.write(getRegPair(H,L), imm);
}

Regval8 CPU::loadABC(){
    regs_8[A] = mem.read(getRegPair(B,C));
    return regs_8[A];
}

Regval8 CPU::loadADE(){
    regs_8[A] = mem.read(getRegPair(D,E));
    return regs_8[A];
}

Regval8 CPU::loadBCA(){
    return mem.write(getRegPair(B,C), regs_8[A]);
}

Regval8 CPU::loadDEA(){
    return mem.write(getRegPair(D,E), regs_8[A]);
}

Regval8 CPU::loadAImmDirect(Regval16 addr){
    regs_8[A] = mem.read(addr);
    return regs_8[A];
}

Regval8 CPU::loadImmADirect(Regval16 addr){
    return mem.write(addr, regs_8[A]);
}

Regval8 CPU::loadHighAC(){
    regs_8[A] = mem.read(0xFF00 + regs_8[C]);
    return regs_8[A];
}

Regval8 CPU::loadHighCA(){
    return mem.write(0xFF00 + regs_8[C], regs_8[A]);
}

Regval8 CPU::loadHighAImm(Regval8 imm){
    regs_8[A] = mem.read(0xFF00 + imm);
    return regs_8[A]; 
}

Regval8 CPU::loadHighImmA(Regval8 imm){
    return mem.write(0xFF00 + imm, regs_8[A]);
}

Regval8 CPU::loadAIndirectDec(){
    Regval16 addr = getRegPair(H,L); 
    regs_8[A] = mem.read(addr--);
    setRegPair(H,L,addr);
    return regs_8[A];
}

Regval8 CPU::loadIndirectADec(){
    Regval16 addr = getRegPair(H,L); 
    mem.write(addr--, regs_8[A]);
    setRegPair(H,L,addr);
    return mem.read(addr + 1);
}

Regval8 CPU::loadAIndirectInc(){
    Regval16 addr = getRegPair(H,L); 
    regs_8[A] = mem.read(addr++);
    setRegPair(H,L,addr);
    return regs_8[A];
}

Regval8 CPU::loadIndirectAInc(){
    Regval16 addr = getRegPair(H,L); 
    mem.write(addr++,regs_8[A]);
    setRegPair(H,L,addr);
    return mem.read(addr - 1);
}

Regval16 CPU::loadRegPairImm(RegIndex_8 msr, RegIndex_8 lsr, Regval16 imm){
    setRegPair(msr, lsr, imm);
    return getRegPair(msr, lsr);
}

Regval16 CPU::loadSPImm(Regval16 imm){
    regs_16[SP] = imm;
    return regs_16[SP];
}

Regval16 CPU::loadDirectSP(Regval16 addr){
    Regval8 msb = regs_16[SP] >> 8;
    Regval8 lsb = regs_16[SP] & 0x00FF;
    mem.write(addr,lsb);
    mem.write(addr + 1, msb);
    return mem.read(addr);
}

Regval16 CPU::loadSPHL(){
    regs_16[SP] = getRegPair(H,L);
    return regs_16[SP];
}

Regval16 CPU::push(RegIndex_8 msr, RegIndex_8 lsr){
    mem.write(--regs_16[SP], regs_8[msr]);
    mem.write(--regs_16[SP], regs_8[lsr]);
    return regs_16[SP];
}

Regval16 CPU::pop(RegIndex_8 msr, RegIndex_8 lsr){
    if(regs_16[SP] == (Regval16)0xFFFE){
        throw std::runtime_error("CPU::Pop(): Attempt to pop off empty stack.");
    }
    regs_8[lsr] = mem.read(regs_16[SP]++);
    regs_8[msr] = mem.read(regs_16[SP]++);
    if(lsr == F){ //temp fix
        regs_8[F] &= 0xF0;
    }
    return regs_16[SP];
}

Regval16 CPU::jump(Regval16 addr){
    regs_16[PC] = addr;
    return regs_16[PC];
}

Regval16 CPU::jumpHL(){
    regs_16[PC] = getRegPair(H,L);
    return regs_16[PC];
}

Regval16 CPU::jumpRel(int8_t imm){
    regs_16[PC] += imm + 1; //relative to the NEXT instruction.
    return regs_16[PC];
}

bool CPU::jumpRelCond(int8_t imm, Condition cond){
    imm++; //relative to the NEXT instruction.
    switch(cond){
        case ZERO:
            if(regs_8[F] & ZERO_FLAG){
                regs_16[PC] += imm;
                return true;
            }
            break;
        case NOT_ZERO:
            if(!(regs_8[F] & ZERO_FLAG)){
                regs_16[PC] += imm;
                return true;
            }
            break;
        case CARRY:
            if(regs_8[F] & CARRY_FLAG){
                regs_16[PC] += imm;
                return true;
            }
            break;
        case NO_CARRY:
            if(!(regs_8[F] & CARRY_FLAG)){
                regs_16[PC] += imm;
                return true;
            }
            break;
    }
    return false;
}

bool CPU::jumpCond(Regval16 addr, Condition cond){
    switch(cond){
        case ZERO:
            if(regs_8[F] & ZERO_FLAG){
                regs_16[PC] = addr;
                return true;
            }
            break;
        case NOT_ZERO:
            if(!(regs_8[F] & ZERO_FLAG)){
                regs_16[PC] = addr;
                return true;
            }
            break;
        case CARRY:
            if(regs_8[F] & CARRY_FLAG){
                regs_16[PC] = addr;
                return true;
            }
            break;
        case NO_CARRY:
            if(!(regs_8[F] & CARRY_FLAG)){
                regs_16[PC] = addr;
                return true;
            }
            break;
        default:
            return false;
            break; 
    }
    return false;
}

Regval16 CPU::call(Regval16 addr){
    regs_16[PC]++;
    pushPC();
    regs_16[PC] = addr;
    return regs_16[PC];
}

Regval16 CPU::callInt(Regval16 addr){
    pushPC();
    regs_16[PC] = addr;
    return regs_16[PC];
}

bool CPU::callCond(Regval16 addr, Condition cond){
    switch(cond){
        case ZERO:
            if(regs_8[F] & ZERO_FLAG){
                call(addr);
                return true;
            }
            break;
        case NOT_ZERO:
            if(!(regs_8[F] & ZERO_FLAG)){
                call(addr);
                return true;
            }
            break;
        case CARRY:
            if(regs_8[F] & CARRY_FLAG){
                call(addr);
                return true;
            }
            break;
        case NO_CARRY:
            if(!(regs_8[F] & CARRY_FLAG)){
                call(addr);
                return true;
            }
            break;
        default:
            return false;
            break; 
    }
    return false;
}

Regval16 CPU::ret(){
    popPC();
    return regs_16[PC];
}

bool CPU::retCond(Condition cond){
    switch(cond){
        case ZERO:
            if(regs_8[F] & ZERO_FLAG){
                ret();
                return true;
            }
            break;
        case NOT_ZERO:
            if(!(regs_8[F] & ZERO_FLAG)){
                ret();
                return true;
            }
            break;
        case CARRY:
            if(regs_8[F] & CARRY_FLAG){
                ret();
                return true;
            }
            break;
        case NO_CARRY:
            if(!(regs_8[F] & CARRY_FLAG)){
                ret();
                return true;
            }
            break;
        default:
            return false;
            break; 
    }
    return false;;
}

Regval16 CPU::reti(){
    ret();
    //turn interrupts on
    return regs_16[PC];
}

Regval16 CPU::rst(Regval8 addr){
    call(addr);
    return regs_16[PC];
}

Regval8 CPU::rlc(RegIndex_8 reg, bool cb){
    regs_8[reg] = (regs_8[reg] << 1) | (regs_8[reg] >> 7);
    if(regs_8[reg] & 0x01)
        regs_8[F] |= CARRY_FLAG;
    else
        regs_8[F] &= ~CARRY_FLAG;
    if(cb){
        if(regs_8[reg] == 0)
            regs_8[F] |= ZERO_FLAG;
        else
            regs_8[F] &= ~ZERO_FLAG;
    }
    else{
        regs_8[F] &= ~ZERO_FLAG;
    }
    regs_8[F] &= ~SUBTRACT_FLAG;
    regs_8[F] &= ~HALF_CARRY_FLAG;
    return regs_8[reg]; 
}

Regval8 CPU::rlcInd(){
    Regval16 addr = getRegPair(H,L);
    Regval8 val = mem.read(addr);
    val = val << 1 | val >> 7;
    if(val & 0x01)
        regs_8[F] |= CARRY_FLAG;
    else
        regs_8[F] &= ~CARRY_FLAG;
    if(val == 0)
        regs_8[F] |= ZERO_FLAG;
    else
        regs_8[F] &= ~ZERO_FLAG;
    regs_8[F] &= ~SUBTRACT_FLAG;
    regs_8[F] &= ~HALF_CARRY_FLAG;
    return mem.write(addr, val);
}

Regval8 CPU::rrc(RegIndex_8 reg, bool cb){
    regs_8[reg] = (regs_8[reg] >> 1) | (regs_8[reg] << 7);
    if(regs_8[reg] & 0x80)
        regs_8[F] |= CARRY_FLAG;
    else
        regs_8[F] &= ~CARRY_FLAG;
    if(cb){
        if(regs_8[reg] == 0)
            regs_8[F] |= ZERO_FLAG;
        else
            regs_8[F] &= ~ZERO_FLAG;
    }
    else{
        regs_8[F] &= ~ZERO_FLAG;
    }
    regs_8[F] &= ~SUBTRACT_FLAG;
    regs_8[F] &= ~HALF_CARRY_FLAG;
    return regs_8[reg]; 
}

Regval8 CPU::rrcInd(){
    Regval16 addr = getRegPair(H,L);
    Regval8 val = mem.read(addr);
    val = val >> 1 | val << 7;
    if(val & 0x80)
        regs_8[F] |= CARRY_FLAG;
    else
        regs_8[F] &= ~CARRY_FLAG;
    if(val == 0)
        regs_8[F] |= ZERO_FLAG;
    else
        regs_8[F] &= ~ZERO_FLAG;
    regs_8[F] &= ~SUBTRACT_FLAG;
    regs_8[F] &= ~HALF_CARRY_FLAG;
    return mem.write(addr, val);
}

Regval8 CPU::rr(RegIndex_8 reg, bool cb){
    if(regs_8[F] & CARRY_FLAG){
        if(!(regs_8[reg] & 0x01)){
            regs_8[F] &= ~CARRY_FLAG;
        }
        regs_8[reg] = regs_8[reg] >> 1 | 0x80;
    }
    else{
        if(regs_8[reg] & 0x01){
            regs_8[F] |= CARRY_FLAG;
        }
        regs_8[reg] = regs_8[reg] >> 1;
    }
    if(cb){
        if(regs_8[reg] == 0)
            regs_8[F] |= ZERO_FLAG;
        else
            regs_8[F] &= ~ZERO_FLAG;
    }
    else{
        regs_8[F] &= ~ZERO_FLAG;
    }
    regs_8[F] &= ~SUBTRACT_FLAG;
    regs_8[F] &= ~HALF_CARRY_FLAG;
    return regs_8[reg]; 
}

Regval8 CPU::rrInd(){
    Regval16 addr = getRegPair(H,L);
    Regval8 val = mem.read(addr);
    if(regs_8[F] & CARRY_FLAG){
        if(!(val & 0x01)){
            regs_8[F] &= ~CARRY_FLAG;
        }
        val = val >> 1 | 0x80;
    }
    else{
        if(val & 0x01){
            regs_8[F] |= CARRY_FLAG;
        }
        val = val >> 1;
    }
    if(val == 0)
        regs_8[F] |= ZERO_FLAG;
    else
        regs_8[F] &= ~ZERO_FLAG;
    regs_8[F] &= ~SUBTRACT_FLAG;
    regs_8[F] &= ~HALF_CARRY_FLAG;
    return mem.write(addr, val);
}

Regval8 CPU::rl(RegIndex_8 reg, bool cb){
    if(regs_8[F] & CARRY_FLAG){
        if(!(regs_8[reg] & 0x80)){
            regs_8[F] &= ~CARRY_FLAG;
        }
        regs_8[reg] = regs_8[reg] << 1 | 0x01;
    }
    else{
        if(regs_8[reg] & 0x80){
            regs_8[F] |= CARRY_FLAG;
        }
        regs_8[reg] = regs_8[reg] << 1;
    }
    if(cb){
        if(regs_8[reg] == 0)
            regs_8[F] |= ZERO_FLAG;
        else
            regs_8[F] &= ~ZERO_FLAG;
    }
    else{
        regs_8[F] &= ~ZERO_FLAG;
    }
    regs_8[F] &= ~SUBTRACT_FLAG;
    regs_8[F] &= ~HALF_CARRY_FLAG;
    return regs_8[reg]; 
}

Regval8 CPU::rlInd(){
    Regval16 addr = getRegPair(H,L);
    Regval8 val = mem.read(addr);
    if(regs_8[F] & CARRY_FLAG){
        if(!(val & 0x80)){
            regs_8[F] &= ~CARRY_FLAG;
        }
        val = val << 1 | 0x01;
    }
    else{
        if(val & 0x80){
            regs_8[F] |= CARRY_FLAG;
        }
        val = val << 1;
    }
    if(val == 0)
        regs_8[F] |= ZERO_FLAG;
    else
        regs_8[F] &= ~ZERO_FLAG;
    regs_8[F] &= ~SUBTRACT_FLAG;
    regs_8[F] &= ~HALF_CARRY_FLAG;
    return mem.write(addr, val);
}

Regval8 CPU::sla(RegIndex_8 reg){
    if(regs_8[reg] & 0x80){
        regs_8[F] |= CARRY_FLAG;
    }
    else{
        regs_8[F] &= ~CARRY_FLAG;
    }
    regs_8[reg] <<= 1;
    if(regs_8[reg] == 0)
        regs_8[F] |= ZERO_FLAG;
    else
        regs_8[F] &= ~ZERO_FLAG;
    regs_8[F] &= ~SUBTRACT_FLAG;
    regs_8[F] &= ~HALF_CARRY_FLAG;
    return regs_8[reg]; 
}

Regval8 CPU::slaInd(){
    Regval16 addr = getRegPair(H,L);
    Regval8 val = mem.read(addr);
    if(val & 0x80){
        regs_8[F] |= CARRY_FLAG;
    }
    else{
        regs_8[F] &= ~CARRY_FLAG;
    }
    val <<= 1;
    if(val == 0)
        regs_8[F] |= ZERO_FLAG;
    else
        regs_8[F] &= ~ZERO_FLAG;
    regs_8[F] &= ~SUBTRACT_FLAG;
    regs_8[F] &= ~HALF_CARRY_FLAG;
    return mem.write(addr, val); 
}

Regval8 CPU::sra(RegIndex_8 reg){
    if(regs_8[reg] & 0x01){
        regs_8[F] |= CARRY_FLAG;
    }
    else{
        regs_8[F] &= ~CARRY_FLAG;
    }
    //check MSB to shift properly
    if(regs_8[reg] & 0x80){
        regs_8[reg] >>= 1;
        regs_8[reg] |= 0x80;
    }
    else{
        regs_8[reg] >>= 1;
    }
    if(regs_8[reg] == 0)
        regs_8[F] |= ZERO_FLAG;
    else
        regs_8[F] &= ~ZERO_FLAG;
    regs_8[F] &= ~SUBTRACT_FLAG;
    regs_8[F] &= ~HALF_CARRY_FLAG;
    return regs_8[reg]; 
}

Regval8 CPU::sraInd(){
    Regval16 addr = getRegPair(H,L);
    Regval8 val = mem.read(addr);
    if(val & 0x01){
        regs_8[F] |= CARRY_FLAG;
    }
    else{
        regs_8[F] &= ~CARRY_FLAG;
    }
    //check MSB to shift properly
    if(val & 0x80){
        val >>= 1;
        val |= 0x80;
    }
    else{
        val >>= 1;
    }
    if(val == 0)
        regs_8[F] |= ZERO_FLAG;
    else
        regs_8[F] &= ~ZERO_FLAG;
    regs_8[F] &= ~SUBTRACT_FLAG;
    regs_8[F] &= ~HALF_CARRY_FLAG;
    return mem.write(addr, val); 
}

Regval8 CPU::srl(RegIndex_8 reg){
    if(regs_8[reg] & 0x01){
        regs_8[F] |= CARRY_FLAG;
    }
    else{
        regs_8[F] &= ~CARRY_FLAG;
    }
    regs_8[reg] >>= 1;
    if(regs_8[reg] == 0)
        regs_8[F] |= ZERO_FLAG;
    else
        regs_8[F] &= ~ZERO_FLAG;
    regs_8[F] &= ~SUBTRACT_FLAG;
    regs_8[F] &= ~HALF_CARRY_FLAG;
    return regs_8[reg]; 
}

Regval8 CPU::srlInd(){
    Regval16 addr = getRegPair(H,L);
    Regval8 val = mem.read(addr);
    if(val & 0x01){
        regs_8[F] |= CARRY_FLAG;
    }
    else{
        regs_8[F] &= ~CARRY_FLAG;
    }
    val >>= 1;
    if(val == 0)
        regs_8[F] |= ZERO_FLAG;
    else
        regs_8[F] &= ~ZERO_FLAG;
    regs_8[F] &= ~SUBTRACT_FLAG;
    regs_8[F] &= ~HALF_CARRY_FLAG;
    return mem.write(addr, val); 
}

Regval8 CPU::swap(RegIndex_8 reg){
    regs_8[reg] = regs_8[reg] >> 4 | regs_8[reg] << 4;
    if(regs_8[reg] == 0)
        regs_8[F] |= ZERO_FLAG;
    else
        regs_8[F] &= ~ZERO_FLAG;
    regs_8[F] &= ~SUBTRACT_FLAG;
    regs_8[F] &= ~HALF_CARRY_FLAG;
    regs_8[F] &= ~CARRY_FLAG;
    return regs_8[reg];
}

Regval8 CPU::swapInd(){
    Regval16 addr = getRegPair(H,L);
    Regval8 val = mem.read(addr);
    val = val >> 4 | val << 4;
    if(val == 0)
        regs_8[F] |= ZERO_FLAG;
    else
        regs_8[F] &= ~ZERO_FLAG;
    regs_8[F] &= ~SUBTRACT_FLAG;
    regs_8[F] &= ~HALF_CARRY_FLAG;
    regs_8[F] &= ~CARRY_FLAG;
    return mem.write(addr, val);
}

Regval8 CPU::bit(RegIndex_8 reg, BitIndex index){
    if(regs_8[reg] & index){
        regs_8[F] &= ~ZERO_FLAG;
    }
    else{
        regs_8[F] |= ZERO_FLAG;
    }
    regs_8[F] &= ~SUBTRACT_FLAG;
    regs_8[F] |= HALF_CARRY_FLAG;
    return regs_8[reg];
}

Regval8 CPU::bitInd(BitIndex index){
    Regval16 addr = getRegPair(H,L);
    Regval8 val = mem.read(addr);
    if(val & index){
        regs_8[F] &= ~ZERO_FLAG;
    }
    else{
        regs_8[F] |= ZERO_FLAG;
    }
    regs_8[F] &= ~SUBTRACT_FLAG;
    regs_8[F] |= HALF_CARRY_FLAG;
    return val;
}

Regval8 CPU::res(RegIndex_8 reg, BitIndex index){
    regs_8[reg] &= ~index; //clear bit
    return regs_8[reg];
}

Regval8 CPU::resInd(BitIndex index){
    Regval16 addr = getRegPair(H,L);
    Regval8 val = mem.read(addr);
    val &= ~index; //clear bit
    return mem.write(addr, val); 
}

Regval8 CPU::set(RegIndex_8 reg, BitIndex index){
    regs_8[reg] |= index; //clear bit
    return regs_8[reg];
}

Regval8 CPU::setInd(BitIndex index){
    Regval16 addr = getRegPair(H,L);
    Regval8 val = mem.read(addr);
    val |= index; //clear bit
    return mem.write(addr, val);
}

void CPU::halt(){/*do i need a func for this?*/}

void CPU::stop(){/*or this?*/}

void CPU::di(){
}

void CPU::ei(){
}

Regval16 CPU::incBC(){
    Regval16 val = getRegPair(B,C);
    val++;
    setRegPair(B,C,val);
    return val;
}

Regval16 CPU::incDE(){
    Regval16 val = getRegPair(D,E);
    val++;
    setRegPair(D,E,val);
    return val;
}

Regval16 CPU::incHL(){
    Regval16 val = getRegPair(H,L);
    val++;
    setRegPair(H,L,val);
    return val;
}

Regval16 CPU::incSP(){
    regs_16[SP]++;
    return regs_16[SP];
}

Regval16 CPU::decBC(){
    Regval16 val = getRegPair(B,C);
    val--;
    setRegPair(B,C,val);
    return val;
}

Regval16 CPU::decDE(){
    Regval16 val = getRegPair(D,E);
    val--;
    setRegPair(D,E,val);
    return val;
}

Regval16 CPU::decHL(){
    Regval16 val = getRegPair(H,L);
    val--;
    setRegPair(H,L,val);
    return val;
}

Regval16 CPU::decSP(){
    regs_16[SP]--;
    return regs_16[SP];
}

Regval16 CPU::addHLRegPair(RegIndex_8 msr, RegIndex_8 lsr){
    regs_8[F] &= ~CARRY_FLAG;
    addRegRegCarry(L, lsr);
    addRegRegCarry(H, msr);
    return getRegPair(H,L);
}

Regval16 CPU::addHLSP(){
    Regval8 prevH = regs_8[H];
    Regval16 val = getRegPair(H,L) + regs_16[SP];
    if(val < getRegPair(H,L))
        regs_8[F] |= CARRY_FLAG;
    else
        regs_8[F] &= ~CARRY_FLAG;
    setRegPair(H,L,val);
    if(halfCarry(prevH,regs_16[SP] >> 8, ADD, false))
        regs_8[F] |= HALF_CARRY_FLAG;
    else
        regs_8[F] &= ~HALF_CARRY_FLAG;
    regs_8[F] &= ~SUBTRACT_FLAG;
    return val;
}

Regval16 CPU::loadHLSPOffset(int8_t imm){
    Regval16 val = regs_16[SP] + imm;
    setRegPair(H,L,val);
    return val;
}

Regval16 CPU::addSPImm(int8_t imm){
    Regval8 prevLsb = regs_16[SP] & 0x00FF;
    regs_16[SP] += imm;
    Regval8 currLsb = regs_16[SP] & 0x00FF;
    if(halfCarry(prevLsb, currLsb, ADD, false))
        regs_8[F] |= HALF_CARRY_FLAG;
    else
        regs_8[F] &= ~HALF_CARRY_FLAG;
    return regs_16[SP];
}

Regval16 CPU::getPC(){
    return regs_16[PC];
}

Regval16 CPU::incPC(){
    regs_16[PC]++;
    return regs_16[PC];
}

void CPU::printStatus(){
    printf("----REGS----\n");
    printf("ACC:   0x%02x\n", regs_8[A]);
    printf("B:     0x%02x\n", regs_8[B]);
    printf("C:     0x%02x\n", regs_8[C]);
    printf("D:     0x%02x\n", regs_8[D]);
    printf("E:     0x%02x\n", regs_8[E]);
    printf("H:     0x%02x\n", regs_8[H]);
    printf("L:     0x%02x\n", regs_8[L]);
    printf("SP:    0x%04x\n", regs_16[SP]);
    printf("PC:    0x%04x\n", regs_16[PC]);
    printf("IX:    0x%04x\n", regs_16[IX]);
    printf("IY:    0x%04x\n", regs_16[IY]);
    printf("----FLAGS----\n");
    printf("Zero:       ");
    if(ZERO_FLAG & regs_8[F]){
        printf("1\n");
    }
    else{
        printf("0\n");
    }
    printf("Subtract:   ");
    if(SUBTRACT_FLAG & regs_8[F]){
        printf("1\n");
    }
    else{
        printf("0\n");
    }
    printf("Half Carry: ");
    if(HALF_CARRY_FLAG & regs_8[F]){
        printf("1\n");
    }
    else{
        printf("0\n");
    }
    printf("Carry:      ");
    if(CARRY_FLAG & regs_8[F]){
        printf("1\n");
    }
    else{
        printf("0\n");
    }
}    

Regval8 CPU::getReg(RegIndex_8 reg){
    return regs_8[reg];
}

Regval16 CPU::getReg(RegIndex_16 reg){
    return regs_16[reg];
}

void CPU::printReg(RegIndex_8 reg){
    printf("0x%02x\n", regs_8[reg]);
}

void CPU::printRegPair(RegIndex_8 msr, RegIndex_8 lsr){
    printf("0x%02x%02x\n", regs_8[msr],regs_8[lsr]);
}

void CPU::printReg(RegIndex_16 reg){
    printf("0x%04x\n", regs_16[reg]);
}