#include "cpu.h"
#include "gameboy.h"
#include <stdbool.h>

char mem[MEM_SIZE];

CPU::CPU(){
    for(int i = 0; i < NUM_REGS_8; i++){
        regs_8[i] = 0;
    }
    for(int i = 0; i < NUM_REGS_16; i++){
        regs_16[i] = 0;
    }
    regs_16[PC] = 0x0100;
}

bool CPU::fullCarry(RegVal_8 val1, RegVal_8 val2, Operation op, bool withCarry){
    int res;
    switch(op){
        case ADD:
            res = val1 + val2;
            if(regs_8[F] & CARRY_FLAG && withCarry){
                res += 1;
            }
            if(res > UINT8_MAX){
                return true;
            }
            return false;
            break;
        case SUB:
            res = val1 - val2;
            if(regs_8[F] & CARRY_FLAG && withCarry){
                res -= 1;
            }
            if(res < 0){
                return true;
            }
            return false;
            break;
        default:
            return false;
            break;
    }
}

bool CPU::halfCarry(RegVal_8 prev, RegVal_8 curr){
    if(prev >> 4 != curr >> 4){
        return true;
    }
    return false;
}


RegVal_8 CPU::addReg(RegIndex_8 reg){
    RegVal_8 prev = regs_8[A];
    regs_8[A] += regs_8[reg];
    if(fullCarry(prev, regs_8[reg], ADD, false))
        regs_8[F] |= CARRY_FLAG;
    else 
        regs_8[F] &= ~CARRY_FLAG;
    if(halfCarry(prev, regs_8[A]))
        regs_8[F] |= HALF_CARRY_FLAG;
    else
        regs_8[F] &= ~HALF_CARRY_FLAG;
    if(regs_8[A] == 0)
        regs_8[F] |= ZERO_FLAG;
    else
        regs_8[F] &= ~ZERO_FLAG;
    regs_8[F] &= ~SUBTRACT_FLAG;
    return regs_8[A];
}

RegVal_8 CPU::addIndirect(){
    RegVal_8 prev = regs_8[A];
    RegVal_16 addr = 0x0000;
    addr |= regs_8[H];
    addr = addr << 8;
    addr |= regs_8[L];
    regs_8[A] += mem[addr];
    if(fullCarry(prev, mem[addr], ADD, false)) 
        regs_8[F] |= CARRY_FLAG;
    else 
        regs_8[F] &= ~CARRY_FLAG;
    if(halfCarry(prev, regs_8[A]))
        regs_8[F] |= HALF_CARRY_FLAG;
    else
        regs_8[F] &= ~HALF_CARRY_FLAG;
    if(regs_8[A] == 0)
        regs_8[F] |= ZERO_FLAG;
    else
        regs_8[F] &= ~ZERO_FLAG;
    regs_8[F] &= ~SUBTRACT_FLAG;
    return regs_8[A];
}

RegVal_8 CPU::addImm(RegVal_8 imm){
    RegVal_8 prev = regs_8[A];
    regs_8[A] += imm;
    if(fullCarry(prev, imm, ADD, false)) 
        regs_8[F] |= CARRY_FLAG;
    else 
        regs_8[F] &= ~CARRY_FLAG;
    if(halfCarry(prev, regs_8[A]))
        regs_8[F] |= HALF_CARRY_FLAG;
    else
        regs_8[F] &= ~HALF_CARRY_FLAG;
    if(regs_8[A] == 0)
        regs_8[F] |= ZERO_FLAG;
    else
        regs_8[F] &= ~ZERO_FLAG;
    regs_8[F] &= ~SUBTRACT_FLAG;
    return regs_8[A];
}

RegVal_8 CPU::addRegCarry(RegIndex_8 reg){
    RegVal_8 prev = regs_8[A];
    regs_8[A] += regs_8[reg];
    regs_8[A] += regs_8[F] & CARRY_FLAG ? 1 : 0;
    if(fullCarry(prev, regs_8[reg], ADD, true)) 
        regs_8[F] |= CARRY_FLAG;
    else 
        regs_8[F] &= ~CARRY_FLAG;
    if(halfCarry(prev, regs_8[A]))
        regs_8[F] |= HALF_CARRY_FLAG;
    else
        regs_8[F] &= ~HALF_CARRY_FLAG;
    if(regs_8[A] == 0)
        regs_8[F] |= ZERO_FLAG;
    else
        regs_8[F] &= ~ZERO_FLAG;
    regs_8[F] &= ~SUBTRACT_FLAG;
    return regs_8[A];
}

RegVal_8 CPU::addIndirectCarry(){
    RegVal_8 prev = regs_8[A];
    RegVal_16 addr = 0x0000;
    addr |= regs_8[H];
    addr = addr << 8;
    addr |= regs_8[L];
    regs_8[A] += mem[addr];
    regs_8[A] += regs_8[F] & CARRY_FLAG ? 1 : 0;
    if(fullCarry(prev, mem[addr], ADD, true)) 
        regs_8[F] |= CARRY_FLAG;
    else 
        regs_8[F] &= ~CARRY_FLAG;
    if(halfCarry(prev, regs_8[A]))
        regs_8[F] |= HALF_CARRY_FLAG;
    else
        regs_8[F] &= ~HALF_CARRY_FLAG;
    if(regs_8[A] == 0)
        regs_8[F] |= ZERO_FLAG;
    else
        regs_8[F] &= ~ZERO_FLAG;
    regs_8[F] &= ~SUBTRACT_FLAG;
    return regs_8[A];
}

RegVal_8 CPU::addImmCarry(RegVal_8 imm){
    RegVal_8 prev = regs_8[A];
    regs_8[A] += imm;
    regs_8[A] += regs_8[F] & CARRY_FLAG ? 1 : 0;
    if(fullCarry(prev, imm, ADD, true)) 
        regs_8[F] |= CARRY_FLAG;
    else 
        regs_8[F] &= ~CARRY_FLAG;
    if(halfCarry(prev, regs_8[A]))
        regs_8[F] |= HALF_CARRY_FLAG;
    else
        regs_8[F] &= ~HALF_CARRY_FLAG;
    if(regs_8[A] == 0)
        regs_8[F] |= ZERO_FLAG;
    else
        regs_8[F] &= ~ZERO_FLAG;
    regs_8[F] &= ~SUBTRACT_FLAG;
    return regs_8[A];
}

RegVal_8 CPU::subReg(RegIndex_8 reg){
    RegVal_8 prev = regs_8[A];
    regs_8[A] -= regs_8[reg];
    if(fullCarry(prev, regs_8[reg], SUB, false)) 
        regs_8[F] |= CARRY_FLAG;
    else 
        regs_8[F] &= ~CARRY_FLAG;
    if(halfCarry(prev, regs_8[A]))
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

RegVal_8 CPU::subIndirect(){
    RegVal_8 prev = regs_8[A];
    RegVal_16 addr = 0x0000;
    addr |= regs_8[H];
    addr = addr << 8;
    addr |= regs_8[L];
    regs_8[A] -= mem[addr];
    if(fullCarry(prev, mem[addr], SUB, false)) 
        regs_8[F] |= CARRY_FLAG;
    else 
        regs_8[F] &= ~CARRY_FLAG;
    if(halfCarry(prev, regs_8[A]))
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

RegVal_8 CPU::subImm(RegVal_8 imm){
    RegVal_8 prev = regs_8[A];
    regs_8[A] -= imm;
    if(fullCarry(prev, imm, SUB, false)) 
        regs_8[F] |= CARRY_FLAG;
    else 
        regs_8[F] &= ~CARRY_FLAG;
    if(halfCarry(prev, regs_8[A]))
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

RegVal_8 CPU::subRegCarry(RegIndex_8 reg){
    RegVal_8 prev = regs_8[A];
    regs_8[A] -= regs_8[reg];
    regs_8[A] -= regs_8[F] & CARRY_FLAG ? 1 : 0;
    if(fullCarry(prev, regs_8[reg], SUB, true)) 
        regs_8[F] |= CARRY_FLAG;
    else 
        regs_8[F] &= ~CARRY_FLAG;
    if(halfCarry(prev, regs_8[A]))
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

RegVal_8 CPU::subIndirectCarry(){
    RegVal_8 prev = regs_8[A];
    RegVal_16 addr = 0x0000;
    addr |= regs_8[H];
    addr = addr << 8;
    addr |= regs_8[L];
    regs_8[A] -= mem[addr];
    regs_8[A] -= regs_8[F] & CARRY_FLAG ? 1 : 0;
    if(fullCarry(prev, mem[addr], SUB, true)) 
        regs_8[F] |= CARRY_FLAG;
    else 
        regs_8[F] &= ~CARRY_FLAG;
    if(halfCarry(prev, regs_8[A]))
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

RegVal_8 CPU::subImmCarry(RegVal_8 imm){
    RegVal_8 prev = regs_8[A];
    regs_8[A] -= imm;
    regs_8[A] -= regs_8[F] & CARRY_FLAG ? 1 : 0;
    if(fullCarry(prev, imm, SUB, true)) 
        regs_8[F] |= CARRY_FLAG;
    else 
        regs_8[F] &= ~CARRY_FLAG;
    if(halfCarry(prev, regs_8[A]))
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

RegVal_8 CPU::compareReg(RegIndex_8 reg){
    RegVal_8 prev = regs_8[A];
    RegVal_8 res = regs_8[A] - regs_8[reg];
    if(fullCarry(prev, regs_8[reg], SUB, false)) 
        regs_8[F] |= CARRY_FLAG;
    else 
        regs_8[F] &= ~CARRY_FLAG;
    if(halfCarry(prev, res))
        regs_8[F] |= HALF_CARRY_FLAG;
    else
        regs_8[F] &= ~HALF_CARRY_FLAG;
    if(regs_8[A] == 0)
        regs_8[F] |= ZERO_FLAG;
    else
        regs_8[F] &= ~ZERO_FLAG;
    regs_8[F] |= SUBTRACT_FLAG;
    return regs_8[F];
}

RegVal_8 CPU::compareIndirect(){
    RegVal_8 prev = regs_8[A];
    RegVal_16 addr = 0x0000;
    addr |= regs_8[H];
    addr = addr << 8;
    addr |= regs_8[L];
    RegVal_8 res = regs_8[A] - mem[addr];
    if(fullCarry(prev, mem[addr], SUB, false)) 
        regs_8[F] |= CARRY_FLAG;
    else 
        regs_8[F] &= ~CARRY_FLAG;
    if(halfCarry(prev,res)) 
        regs_8[F] |= HALF_CARRY_FLAG;
    else
        regs_8[F] &= ~HALF_CARRY_FLAG;
    if(regs_8[A] == 0)
        regs_8[F] |= ZERO_FLAG;
    else
        regs_8[F] &= ~ZERO_FLAG;
    regs_8[F] |= SUBTRACT_FLAG;
    return regs_8[F];
}

RegVal_8 CPU::compareImmediate(RegVal_8 imm){
    RegVal_8 prev = regs_8[A];
    RegVal_8 res = regs_8[A] - imm;
    if(fullCarry(prev, imm, SUB, false)) 
        regs_8[F] |= CARRY_FLAG;
    else 
        regs_8[F] &= ~CARRY_FLAG;
    if(halfCarry(prev, res))
        regs_8[F] |= HALF_CARRY_FLAG;
    else
        regs_8[F] &= ~HALF_CARRY_FLAG;
    if(regs_8[A] == 0)
        regs_8[F] |= ZERO_FLAG;
    else
        regs_8[F] &= ~ZERO_FLAG;
    regs_8[F] |= SUBTRACT_FLAG;
    return regs_8[F];
}

RegVal_8 CPU::incrementReg(RegIndex_8 reg){
    RegVal_8 prev = regs_8[reg];
    regs_8[reg] += 1;
    if(halfCarry(prev, regs_8[reg]))
        regs_8[F] |= HALF_CARRY_FLAG;
    else
        regs_8[F] &= ~HALF_CARRY_FLAG;
    if(regs_8[A] == 0)
        regs_8[F] |= ZERO_FLAG;
    else
        regs_8[F] &= ~ZERO_FLAG;
    regs_8[F] &= ~SUBTRACT_FLAG;
    return regs_8[reg];
}

RegVal_8 CPU::incrementIndirect(){
    RegVal_16 addr = 0x0000;
    addr |= regs_8[H];
    addr = addr << 8;
    addr |= regs_8[L];
    RegVal_8 prev = mem[addr];
    mem[addr] += 1;
    if(halfCarry(prev, mem[addr]))
        regs_8[F] |= HALF_CARRY_FLAG;
    else
        regs_8[F] &= ~HALF_CARRY_FLAG;
    if(regs_8[A] == 0)
        regs_8[F] |= ZERO_FLAG;
    else
        regs_8[F] &= ~ZERO_FLAG;
    regs_8[F] &= ~SUBTRACT_FLAG;
    return mem[addr];
}

RegVal_8 CPU::decrementReg(RegIndex_8 reg){
    RegVal_8 prev = regs_8[reg];
    regs_8[reg] -= 1;
    if(halfCarry(prev, regs_8[reg]))
        regs_8[F] |= HALF_CARRY_FLAG;
    else
        regs_8[F] &= ~HALF_CARRY_FLAG;
    if(regs_8[A] == 0)
        regs_8[F] |= ZERO_FLAG;
    else
        regs_8[F] &= ~ZERO_FLAG;
    regs_8[F] |= SUBTRACT_FLAG;
    return regs_8[reg];
}

RegVal_8 CPU::decrementIndirect(){
    RegVal_16 addr = 0x0000;
    addr |= regs_8[H];
    addr = addr << 8;
    addr |= regs_8[L];
    RegVal_8 prev = mem[addr];
    mem[addr] -= 1;
    if(halfCarry(prev, mem[addr]))
        regs_8[F] |= HALF_CARRY_FLAG;
    else
        regs_8[F] &= ~HALF_CARRY_FLAG;
    if(regs_8[A] == 0)
        regs_8[F] |= ZERO_FLAG;
    else
        regs_8[F] &= ~ZERO_FLAG;
    regs_8[F] |= SUBTRACT_FLAG;
    return mem[addr];
}

RegVal_8 CPU::andReg(RegIndex_8 reg){
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

RegVal_8 CPU::andIndirect(){
    RegVal_16 addr = 0x0000;
    addr |= regs_8[H];
    addr = addr << 8;
    addr |= regs_8[L];
    regs_8[A] &= mem[addr];
    if(regs_8[A] == 0)
        regs_8[F] |= ZERO_FLAG;
    else
        regs_8[F] &= ~ZERO_FLAG;
    regs_8[F] &= ~CARRY_FLAG;
    regs_8[F] &= ~SUBTRACT_FLAG;
    regs_8[F] |= HALF_CARRY_FLAG;
    return regs_8[A];
}

RegVal_8 CPU::andImm(RegVal_8 imm){
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

RegVal_8 CPU::orReg(RegIndex_8 reg){
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

RegVal_8 CPU::orIndirect(){
    RegVal_16 addr = 0x0000;
    addr |= regs_8[H];
    addr = addr << 8;
    addr |= regs_8[L];
    regs_8[A] |= mem[addr];
    if(regs_8[A] == 0)
        regs_8[F] |= ZERO_FLAG;
    else
        regs_8[F] &= ~ZERO_FLAG;
    regs_8[F] &= ~CARRY_FLAG;
    regs_8[F] &= ~SUBTRACT_FLAG;
    regs_8[F] &= ~HALF_CARRY_FLAG;
    return regs_8[A];
}

RegVal_8 CPU::orImm(RegVal_8 imm){
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

RegVal_8 CPU::xorReg(RegIndex_8 reg){
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

RegVal_8 CPU::xorIndirect(){
    RegVal_16 addr = 0x0000;
    addr |= regs_8[H];
    addr = addr << 8;
    addr |= regs_8[L];
    regs_8[A] ^= mem[addr];
    if(regs_8[A] == 0)
        regs_8[F] |= ZERO_FLAG;
    else
        regs_8[F] &= ~ZERO_FLAG;
    regs_8[F] &= ~CARRY_FLAG;
    regs_8[F] &= ~SUBTRACT_FLAG;
    regs_8[F] &= ~HALF_CARRY_FLAG;
    return regs_8[A];
}

RegVal_8 CPU::xorImm(RegVal_8 imm){
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

RegVal_8 CPU::compCarryFlag(){
    regs_8[F] ^= CARRY_FLAG;
    regs_8[F] &= ~SUBTRACT_FLAG;
    regs_8[F] &= ~HALF_CARRY_FLAG;
    return regs_8[F];
}

RegVal_8 CPU::setCarryFlag(){
    regs_8[F] |= CARRY_FLAG;
    regs_8[F] &= ~SUBTRACT_FLAG;
    regs_8[F] &= ~HALF_CARRY_FLAG;
    return regs_8[F];
}

RegVal_8 CPU::decimalAdjustAcc(){
    RegVal_8 ret = 0;
    return ret;
}

RegVal_8 CPU::compAcc(){
    regs_8[A] ^= 0xFF;
    regs_8[F] |= SUBTRACT_FLAG;
    regs_8[F] |= HALF_CARRY_FLAG;
    return regs_8[A];
}


RegVal_8 CPU::loadRegImm(RegIndex_8 reg, RegVal_8 imm){
    regs_8[reg] = imm;
    return regs_8[reg];
}
    
void CPU::Print(){
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