#include "memory.h"
#include <algorithm>
#include <stdexcept>
#include <iostream>

//static vars
std::array<Regval8,UINT16_MAX+1> Memory::mem;
Regval8 Memory::joypadBuff;
bool Memory::ppuLock = false;
bool Memory::dmaLock = false;
bool Memory::vramAltered = false;

constexpr Regval8 PAD_READ_MASK = 0x10; 
constexpr Regval8 BUTTON_READ_MASK = 0x20;

Memory::Memory(Permission perm) : perm(perm){
    mem[JOYP_REG_ADDR] = 0xCF;
    joypadBuff = 0xFF;
}

bool Memory::inRange(Regval16 addr, Regval16 low, Regval16 hi) const{
    if(addr >= low && addr <= hi)
        return true;
    return false;
}

bool Memory::checkPerm(const Regval16 addr, const Access acc) const{
    if((inRange(addr, ECHO_START, ECHO_END) ||
        inRange(addr, BAD_ZONE_START, BAD_ZONE_END))){
        return false;
    }
    else if(acc == WRITE && inRange(addr, ROM_BANK_0_START, ROM_BANK_1_END)){
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

void Memory::prepJoypadRead(const Regval8 byte) const{
    if(!(byte & (BUTTON_READ_MASK | PAD_READ_MASK))){
        std::cout << "byte: " << (int)byte << std::endl;
        throw std::invalid_argument("Joypad::prepRead(): attempted read with invalid joypad selection value.");
    }
    if(byte & ~PAD_READ_MASK){
        mem[JOYP_REG_ADDR] = (joypadBuff & ALL_PAD_MASK);
        mem[JOYP_REG_ADDR] >>= 4;
    }
    if(byte & ~BUTTON_READ_MASK){
        mem[JOYP_REG_ADDR] = joypadBuff & ALL_BUTTON_MASK;
    }
    mem[IF_REG_ADDR] = mem[IF_REG_ADDR] | JOYPAD_INT;
}

void Memory::setJoypadBuff(Regval8 byte){
    joypadBuff = byte;
}

Regval8 Memory::getJoypadBuff(){
    return joypadBuff;
}

bool Memory::write(const Regval16 addr, const Regval8 byte) const{
    if(!checkPerm(addr, WRITE))
        return false;
    if(addr == JOYP_REG_ADDR)
        prepJoypadRead(byte);
    else{
        mem[addr] = byte;
    }
    return true;
}

Regval8 Memory::read(const Regval16 addr) const{
    if(!checkPerm(addr, READ)){
        return 0xFF;
    }
    return mem[addr];
}

Regval16 Memory::dump(const Regval16 addr, const Regval8* buf, const size_t n) const{
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

Register Memory::getRegister(Regval16 addr){
    return mem[addr];
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

bool Memory::vramChange(){
    return vramAltered;
}

void Memory::resetChange(){
    vramAltered = false;
}