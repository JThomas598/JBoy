#include "memory.h"
#include <algorithm>
#include <stdexcept>
#include <iostream>

//static vars
std::array<Regval8,UINT16_MAX+1> Memory::mem;
std::array<std::array<Regval8, EXT_RAM_BANK_SIZE>, MBC1_NUM_RAM_BANKS> Memory::ramBanks;
std::array<std::array<Regval8, ROM_BANK_SIZE>, MBC1_NUM_ROM_BANKS> Memory::romBanks;
Regval8 Memory::currRamBank;
Regval8 Memory::currRomBank;
Regval8 Memory::joypadBuff;
bool Memory::ramEnabled = false;
bool Memory::ppuLock = false;
bool Memory::dmaLock = false;
bool Memory::vramAltered = false;

constexpr Regval8 PAD_READ_MASK = 0x10; 
constexpr Regval8 BUTTON_READ_MASK = 0x20;

Memory::Memory(Permission perm) : perm(perm){
    mem[JOYP_REG_ADDR] = 0xCF;
    joypadBuff = 0xFF;
    currRamBank = 0;
    currRomBank = 1;
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
    else if(inRange(addr, EXT_RAM_ENABLE_START, EXT_RAM_ENABLE_END) && (byte & 0x0A) == 0x0A){
        ramEnabled = true;
    }
    else if(inRange(addr, RAM_BANK_SELECT_START, RAM_BANK_SELECT_END)){
        currRamBank = byte & 0x03;
    }
    else if(inRange(addr, ROM_BANK_SELECT_START, ROM_BANK_SELECT_END)){
        currRomBank = byte & 0x1F;
        if(currRomBank == 0){currRomBank = 1;}
    }
    else if(inRange(addr, EXT_RAM_START, EXT_RAM_END) && ramEnabled){
        ramBanks[currRamBank][addr - EXT_RAM_START] = byte;
    }
    else{
        mem[addr] = byte;
    }
    return true;
}

Regval8 Memory::read(const Regval16 addr) const{
    if(!checkPerm(addr, READ)){
        return 0xFF;
    }
    else if(inRange(addr, EXT_RAM_START, EXT_RAM_END) && ramEnabled){
        return ramBanks[currRamBank][addr - EXT_RAM_START];
        std::cout << "reading ram bank" << std::endl;
    }
    else if(inRange(addr, ROM_BANK_N_START, ROM_BANK_N_END)){
        return romBanks[currRomBank][addr - ROM_BANK_N_START];
    }
    return mem[addr];
}

Regval16 Memory::dump(const Regval16 addr, const Regval8* buf, const size_t n) const{
    size_t bytes_written = 0;
    while(bytes_written < n){
        mem[addr + bytes_written] = buf[bytes_written];
        bytes_written++;
    }
    return bytes_written;
}

Regval16 Memory::copyRomBank(const Regval8* buf, const int bankNum){
    size_t bytes_written = 0;
    while(bytes_written < ROM_BANK_SIZE){
        romBanks[bankNum][bytes_written] = buf[bytes_written];
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

void Memory::printStatus(){
    std::cout << "current rom bank: " << (int)currRomBank << std::endl;
    std::cout << "current ram bank: " << (int)currRamBank << std::endl;
}