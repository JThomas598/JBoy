#include "memory.h"
#include <algorithm>
#include <stdexcept>

std::array<RegVal_8,UINT16_MAX+1> Memory::mem;
bool Memory::ppuLock = false;
bool Memory::dmaLock = false;

Memory::Memory(Permission perm) : perm(perm){
}

bool Memory::inRange(RegVal_16 addr, RegVal_16 low, RegVal_16 hi) const{
    if(addr >= low && addr <= hi)
        return true;
    return false;
}

bool Memory::checkPerm(const RegVal_16 addr, const Access acc) const{
    if(inRange(addr, ECHO_START, ECHO_END) ||
        inRange(addr, BAD_ZONE_START, BAD_ZONE_END)){
        return false;
    }
    else if(acc == WRITE && inRange(addr, ROM_BANK_0_START, ROM_BANK_1_END)){
        return false;
    }
    else if(addr == 0xFF00){
        return false;
    }
    else if(perm == CPU_PERM){
        if(dmaLock && !inRange(addr, HRAM_START, HRAM_END)){
            throw std::logic_error("Memory::checkPerm(): Illegal CPU access during DMA transfer");
        }
        else if(ppuLock && inRange(addr, VRAM_START, VRAM_END)){
            throw std::logic_error("Memory::checkPerm(): Illegal CPU access to VRAM during pixel transfer");
        }
    }
    return true;
}

bool Memory::write(const RegVal_16 addr, const RegVal_8 byte) const{
    if(!checkPerm(addr, WRITE)){
        return false;
    }
    mem[addr] = byte;
    return true;
}

RegVal_8 Memory::read(const RegVal_16 addr) const{
    if(!checkPerm(addr, READ)){
        return 0xFF;
    }
    return mem[addr];
}

RegVal_16 Memory::dump(const RegVal_16 addr, const RegVal_8* buf, const size_t n) const{
    if(perm != SYS_PERM){
        return 0;
    }
    size_t bytes_written = 0;
    while(bytes_written < n){
        mem[addr + bytes_written] = buf[bytes_written];
        bytes_written++;
    }
    return bytes_written;
}

bool Memory::lockVram(){
    if(perm == PPU_PERM){
        ppuLock = true;
        return true;
    }
    return false;
}

bool Memory::unlockVram(){
    if(perm == PPU_PERM){
        ppuLock = false;
        return true;
    }
    return false;
}