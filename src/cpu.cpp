#include "cpu.h"
#include "gameboy.h"
#include <stdbool.h>

char mem[MEM_SIZE];
bool IME = true;

CPU::CPU(){
    regs_8[A] = 0x01;
    regs_8[B] = 0x00;
    regs_8[C] = 0x13;
    regs_8[D] = 0x00;
    regs_8[E] = 0xD8;
    regs_8[H] = 0x01;
    regs_8[L] = 0x4D;
    regs_16[PC] = 0x0100;
    regs_16[SP] = 0xFFFE;
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

RegVal_16 CPU::getRegPair(RegIndex_8 msr, RegIndex_8 lsr){
    RegVal_16 retval = 0x0000;
    retval |= regs_8[msr];
    retval = retval << 8;
    retval |= regs_8[lsr];
    return retval;
}

void CPU::setRegPair(RegIndex_8 msr, RegIndex_8 lsr, RegVal_16 val){
    regs_8[msr] = val >> 8;
    regs_8[lsr] = val & 0x00FF;
}

bool CPU::halfCarry(RegVal_8 prev, RegVal_8 curr){
    if(prev >> 4 != curr >> 4){
        return true;
    }
    return false;
}

void CPU::pushPC(){
    mem[regs_16[SP]] = regs_16[PC] >> 8;
    regs_16[SP]--;
    mem[regs_16[SP]] = regs_16[PC] & 0x00FF;
    regs_16[SP]--;
}

void CPU::popPC(){
    RegVal_8 msb = 0x00;
    RegVal_8 lsb = 0x00;
    regs_16[PC] = 0x0000;
    regs_16[SP]++;
    lsb = mem[regs_16[SP]];
    regs_16[SP]++;
    msb = mem[regs_16[SP]];
    regs_16[PC] |= msb;
    regs_16[PC] = regs_16[PC] << 8;
    regs_16[PC] |= lsb;
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
    RegVal_16 addr = getRegPair(H,L);
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
    RegVal_16 addr = getRegPair(H,L);
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
    RegVal_16 addr = getRegPair(H,L);
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
    RegVal_16 addr = getRegPair(H,L);
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
    if(res == 0)
        regs_8[F] |= ZERO_FLAG;
    else
        regs_8[F] &= ~ZERO_FLAG;
    regs_8[F] |= SUBTRACT_FLAG;
    return regs_8[F];
}

RegVal_8 CPU::compareIndirect(){
    RegVal_8 prev = regs_8[A];
    RegVal_16 addr = getRegPair(H,L);
    RegVal_8 res = regs_8[A] - mem[addr];
    if(fullCarry(prev, mem[addr], SUB, false)) 
        regs_8[F] |= CARRY_FLAG;
    else 
        regs_8[F] &= ~CARRY_FLAG;
    if(halfCarry(prev,res)) 
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

RegVal_8 CPU::compareImm(RegVal_8 imm){
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
    if(res == 0)
        regs_8[F] |= ZERO_FLAG;
    else
        regs_8[F] &= ~ZERO_FLAG;
    regs_8[F] |= SUBTRACT_FLAG;
    return regs_8[F];
}

RegVal_8 CPU::incReg(RegIndex_8 reg){
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

RegVal_8 CPU::incIndirect(){
    RegVal_16 addr = getRegPair(H,L);
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

RegVal_8 CPU::decReg(RegIndex_8 reg){
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

RegVal_8 CPU::decIndirect(){
    RegVal_16 addr = getRegPair(H,L);
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
    RegVal_16 addr = getRegPair(H,L);
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
    RegVal_16 addr = getRegPair(H,L);
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
    RegVal_16 addr = getRegPair(H,L);
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

RegVal_8 CPU::loadRegReg(RegIndex_8 dst,RegIndex_8 src){
    regs_8[dst] = regs_8[src];
    return regs_8[dst];
}

RegVal_8 CPU::loadRegImm(RegIndex_8 reg, RegVal_8 imm){
    regs_8[reg] = imm;
    return regs_8[reg];
}

RegVal_8 CPU::loadRegIndirect(RegIndex_8 reg){
    RegVal_16 addr = getRegPair(H,L);
    regs_8[reg] = mem[addr];
    return regs_8[reg];
}

RegVal_8 CPU::loadIndirectReg(RegIndex_8 reg){
    RegVal_16 addr = getRegPair(H,L);
    mem[addr] = regs_8[reg];
    return regs_8[A];
}

RegVal_8 CPU::loadIndirectImm(RegVal_8 imm){
    RegVal_16 addr = getRegPair(H,L);
    mem[addr] = imm;
    return regs_8[A];
}

RegVal_8 CPU::loadABC(){
    RegVal_16 addr = getRegPair(B,C);
    regs_8[A] = mem[addr];
    return regs_8[A];
}

RegVal_8 CPU::loadADE(){
    RegVal_16 addr = getRegPair(D,E);
    regs_8[A] = mem[addr];
    return regs_8[A];
}

RegVal_8 CPU::loadBCA(){
    RegVal_16 addr = getRegPair(B,C);
    mem[addr] = regs_8[A];
    return mem[addr];
}

RegVal_8 CPU::loadDEA(){
    RegVal_16 addr = getRegPair(D,E);
    mem[addr] = regs_8[A];
    return mem[addr];
}

RegVal_8 CPU::loadAImmIndirect(RegVal_16 addr){
    regs_8[A] = mem[addr];
    return mem[addr];
}

RegVal_8 CPU::loadImmAIndirect(RegVal_16 addr){
    mem[addr] = regs_8[A];
    return mem[addr];
}

RegVal_8 CPU::loadHighAC(){
    regs_8[A] = mem[0xFF00 + C];
    return regs_8[A];
}

RegVal_8 CPU::loadHighCA(){
    mem[0xFF00 + C] = regs_8[A];
    return mem[0xFF00 + C];
}

RegVal_8 CPU::loadHighAImm(RegVal_8 imm){
    regs_8[A] = mem[0xFF00 + imm];
    return regs_8[A];
}

RegVal_8 CPU::loadHighImmA(RegVal_8 imm){
    mem[0xFF00 + imm] = regs_8[A];
    return mem[0xFF00 + imm];
}

RegVal_8 CPU::loadAIndirectDec(){
    RegVal_16 addr = getRegPair(H,L); 
    regs_8[A] = mem[addr];
    addr -= 1;
    setRegPair(H,L,addr);
    return regs_8[A];
}

RegVal_8 CPU::loadIndirectADec(){
    RegVal_16 addr = getRegPair(H,L); 
    mem[addr] = regs_8[A]; 
    addr -= 1;
    setRegPair(H,L,addr);
    return mem[addr];
}

RegVal_8 CPU::loadAIndirectInc(){
    RegVal_16 addr = getRegPair(H,L); 
    regs_8[A] = mem[addr];
    addr += 1;
    setRegPair(H,L,addr);
    return regs_8[A];
}

RegVal_8 CPU::loadIndirectAInc(){
    RegVal_16 addr = getRegPair(H,L); 
    mem[addr] = regs_8[A]; 
    addr += 1;
    setRegPair(H,L,addr);
    return mem[addr];
}

RegVal_16 CPU::loadRegPairImm(RegIndex_8 msr, RegIndex_8 lsr, RegVal_16 imm){
    setRegPair(msr, lsr, imm);
    return getRegPair(msr, lsr);
}

RegVal_16 CPU::loadSPImm(RegVal_16 imm){
    regs_16[SP] = imm;
    return regs_16[SP];
}

RegVal_16 CPU::loadDirectSP(RegVal_16 addr){
    mem[addr] = regs_16[SP];
    return mem[addr];
}

RegVal_16 CPU::loadSPHL(){
    regs_16[SP] = getRegPair(H,L);
    return regs_16[SP];
}

RegVal_16 CPU::push(RegIndex_8 msr, RegIndex_8 lsr){
    mem[regs_16[SP]] = regs_8[msr];
    regs_16[SP]--;
    mem[regs_16[SP]] = regs_8[lsr];
    regs_16[SP]--;
    return regs_16[SP];
}

RegVal_16 CPU::pop(RegIndex_8 msr, RegIndex_8 lsr){
    regs_16[SP]++;
    regs_8[lsr] = mem[regs_16[SP]];
    regs_16[SP]++;
    regs_8[msr] = mem[regs_16[SP]];
    return regs_16[SP];
}

RegVal_16 CPU::jump(RegVal_16 addr){
    regs_16[PC] = addr;
    return regs_16[PC];
}

RegVal_16 CPU::jumpHL(){
    regs_16[PC] = getRegPair(H,L);
    return regs_16[PC];
}

RegVal_16 CPU::jumpRel(int8_t imm){
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

bool CPU::jumpCond(RegVal_16 addr, Condition cond){
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

RegVal_16 CPU::call(RegVal_16 addr){
    pushPC();
    regs_16[PC] = addr;
    return regs_16[PC];
}

bool CPU::callCond(RegVal_16 addr, Condition cond){
    switch(cond){
        case ZERO:
            if(regs_8[F] & ZERO_FLAG){
                pushPC();
                regs_16[PC] = addr;
                return true;
            }
            break;
        case NOT_ZERO:
            if(!(regs_8[F] & ZERO_FLAG)){
                pushPC();
                regs_16[PC] = addr;
                return true;
            }
            break;
        case CARRY:
            if(regs_8[F] & CARRY_FLAG){
                pushPC();
                regs_16[PC] = addr;
                return true;
            }
            break;
        case NO_CARRY:
            if(!(regs_8[F] & CARRY_FLAG)){
                pushPC();
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

RegVal_16 CPU::ret(){
    popPC();
    incPC();
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

RegVal_16 CPU::reti(){
    ret();
    IME = 1;
    return regs_16[PC];
}

RegVal_16 CPU::rst(RegVal_8 addr){
    call(addr);
    return regs_16[PC];
}

RegVal_8 CPU::RLCA(){
    regs_8[A] = (regs_8[A] << 1) | (regs_8[A] >> 7);
    if(regs_8[A] & 0x01)
        regs_8[F] |= CARRY_FLAG;
    else
        regs_8[F] &= ~CARRY_FLAG;
    regs_8[F] &= ~SUBTRACT_FLAG;
    regs_8[F] &= ~HALF_CARRY_FLAG;
    regs_8[F] &= ~ZERO_FLAG;
    return regs_8[A]; 
}

RegVal_8 CPU::RRCA(){
    regs_8[A] = (regs_8[A] >> 1) | (regs_8[A] << 7);
    if(regs_8[A] & 0x80)
        regs_8[F] |= CARRY_FLAG;
    else
        regs_8[F] &= ~CARRY_FLAG;
    regs_8[F] &= ~SUBTRACT_FLAG;
    regs_8[F] &= ~HALF_CARRY_FLAG;
    regs_8[F] &= ~ZERO_FLAG;
    return regs_8[A]; 
}

RegVal_8 CPU::RRA(){
    if(regs_8[A] & 0x01)
        regs_8[F] |= CARRY_FLAG;
    else
        regs_8[F] &= ~CARRY_FLAG;
    regs_8[A] = regs_8[A] >> 1;
    regs_8[F] &= ~SUBTRACT_FLAG;
    regs_8[F] &= ~HALF_CARRY_FLAG;
    regs_8[F] &= ~ZERO_FLAG;
    return regs_8[A]; 
}

RegVal_8 CPU::RLA(){
    if(regs_8[A] & 0x80)
        regs_8[F] |= CARRY_FLAG;
    else
        regs_8[F] &= ~CARRY_FLAG;
    regs_8[A] = regs_8[A] << 1;
    regs_8[F] &= ~SUBTRACT_FLAG;
    regs_8[F] &= ~HALF_CARRY_FLAG;
    regs_8[F] &= ~ZERO_FLAG;
    return regs_8[A]; 
}

void CPU::halt(){/*do i need a func for this?*/}

void CPU::stop(){/*or this?*/}

void CPU::di(){
    IME = 0;
}

void CPU::ei(){
    IME = 1;
}

RegVal_16 CPU::incBC(){
    RegVal_16 val = getRegPair(B,C);
    val++;
    setRegPair(B,C,val);
    return val;
}

RegVal_16 CPU::incDE(){
    RegVal_16 val = getRegPair(D,E);
    val++;
    setRegPair(D,E,val);
    return val;
}

RegVal_16 CPU::incHL(){
    RegVal_16 val = getRegPair(H,L);
    val++;
    setRegPair(H,L,val);
    return val;
}

RegVal_16 CPU::incSP(){
    regs_16[SP]++;
    return regs_16[SP];
}

RegVal_16 CPU::decBC(){
    RegVal_16 val = getRegPair(B,C);
    val--;
    setRegPair(B,C,val);
    return val;
}

RegVal_16 CPU::decDE(){
    RegVal_16 val = getRegPair(D,E);
    val--;
    setRegPair(D,E,val);
    return val;
}

RegVal_16 CPU::decHL(){
    RegVal_16 val = getRegPair(H,L);
    val--;
    setRegPair(H,L,val);
    return val;
}

RegVal_16 CPU::decSP(){
    regs_16[SP]--;
    return regs_16[SP];
}

RegVal_16 CPU::addHLRegPair(RegIndex_8 msr, RegIndex_8 lsr){
    RegVal_16 val = getRegPair(H,L) + getRegPair(msr,lsr);
    setRegPair(H,L,val);
    return val;
}

RegVal_16 CPU::addHLSP(){
    RegVal_16 val = getRegPair(H,L) + regs_16[SP];
    setRegPair(H,L,val);
    return val;
}

RegVal_16 CPU::addSPImm(int8_t imm){
    regs_16[SP] += imm;
    return regs_16[SP];
}

RegVal_16 CPU::getPC(){
    return regs_16[PC];
}

RegVal_16 CPU::incPC(){
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

void CPU::printReg(RegIndex_8 reg){
    printf("0x%02x\n", regs_8[reg]);
}

void CPU::printRegPair(RegIndex_8 msr, RegIndex_8 lsr){
    printf("0x%02x%02x\n", regs_8[msr],regs_8[lsr]);
}

void CPU::printReg(RegIndex_16 reg){
    printf("0x%04x\n", regs_16[reg]);
}