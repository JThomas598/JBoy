#include "memory.h"
#include <algorithm>
#include <stdexcept>
#include <iostream>
#include <fstream>

//static vars
std::array<Regval8,UINT16_MAX+1> Memory::mem;
std::array<std::array<Regval8, RAM_BANK_BANK_SIZE>, MBC1_NUM_RAM_BANKS> Memory::ramBanks;
std::array<std::array<Regval8, ROM_BANK_SIZE>, MBC1_NUM_ROM_BANKS> Memory::romBanks;
Regval8 Memory::currRamBank;
Regval8 Memory::currRomBank;
Regval8 Memory::joypadBuff;
CartType Memory::cartType;
BankingMode Memory::mode;
bool Memory::ramEnabled = false;

constexpr Regval8 PAD_READ_MASK = 0x10; 
constexpr Regval8 BUTTON_READ_MASK = 0x20;

constexpr Regval16 CARTRIDGE_TYPE_BYTE_ADDR= 0x0147;

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
    return true;
}

void Memory::saveRamState(std::string file){
    std::ofstream outFile(file, std::ios_base::out | std::ios_base::binary);
    int bankBytesWritten = 0;
    int bankNum = 0;
    while(bankNum < MBC1_NUM_RAM_BANKS){
        while(bankBytesWritten < RAM_BANK_BANK_SIZE){
            outFile.write((char*)ramBanks[bankNum].data(), RAM_BANK_BANK_SIZE);
            long currBytesWritten = outFile.tellp(); 
            bankBytesWritten += currBytesWritten - bankBytesWritten;
        }
        bankBytesWritten = 0;
        bankNum++;
    }
}

void Memory::prepJoypadRead(const Regval8 byte) const{
    if(!(byte & (BUTTON_READ_MASK | PAD_READ_MASK))){
        return;
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
    else if(inRange(addr, RAM_BANK_ENABLE_START, RAM_BANK_ENABLE_END)){
        ramEnabled = (byte & 0x0A) == 0x0A ? mode = ADVANCED : mode = SIMPLE;
    }
    else if(inRange(addr, RAM_BANK_SELECT_START, RAM_BANK_SELECT_END)){
        currRamBank = byte & 0x03;
    }
    else if(inRange(addr, ROM_BANK_SELECT_START, ROM_BANK_SELECT_END)){
        switch(cartType){
            case ROM_ONLY:
            case MBC1:
            case MBC1_RAM:
            case MBC1_RAM_BATTERY:
                currRomBank = byte & 0x1F;
            case MBC3_TIMER_BATTERY:
            case MBC3_TIMER_RAM_BATTERY:
            case MBC3:
            case MBC3_RAM:
            case MBC3_RAM_BATTERY:
                currRomBank = byte & 0x7F;
                break;
            default:
                break;
        }
        if(currRomBank == 0){currRomBank = 1;}
    }
    else if(inRange(addr, RAM_BANK_START, RAM_BANK_END) && ramEnabled){
        ramBanks[currRamBank][addr - RAM_BANK_START] = byte;
    }
    else{
        mem[addr] = byte;
    }
    return true;
}

Regval8 Memory::read(const Regval16 addr) const{
    /*
    if(inRange(addr, ROM_BANK_0_START, ROM_BANK_0_END)){
        return romBanks[currRomBank][addr - ROM_BANK_N_START];
    }
    */
    if(inRange(addr, ROM_BANK_N_START, ROM_BANK_N_END)){
        return romBanks[currRomBank][addr - ROM_BANK_N_START];
    }
    else if(inRange(addr, RAM_BANK_START, RAM_BANK_END)){
        if(mode != ADVANCED && currRamBank > 0){
            throw std::logic_error("Memory::read(): Attempted access to ram bank other than bank 0 in simple mode.");
        }
        return ramBanks[currRamBank][addr - RAM_BANK_START];
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

Regval16 Memory::copyRamBank(const Regval8* buf, const int bankNum){
    size_t bytes_written = 0;
    while(bytes_written < RAM_BANK_BANK_SIZE){
        ramBanks[bankNum][bytes_written] = buf[bytes_written];
        bytes_written++;
    }
    return bytes_written;
}

Register Memory::getRegister(Regval16 addr){
    return mem[addr];
}

void Memory::resolveCartridgeType(){
    switch(mem[CARTRIDGE_TYPE_BYTE_ADDR]){
        case ROM_ONLY:
        case MBC1:
        case MBC1_RAM:
        case MBC1_RAM_BATTERY:
            cartType = MBC1;
            break;
        case MBC3_TIMER_BATTERY:
        case MBC3_TIMER_RAM_BATTERY:
        case MBC3:
        case MBC3_RAM:
        case MBC3_RAM_BATTERY:
            cartType = MBC3;
            break;
        default:
            throw std::logic_error("Memory::resolveCartridgeType(): Cartridge type not yet supported.");
    }
}

void Memory::printStatus(){
    std::cout << "current rom bank: " << (int)currRomBank << std::endl;
    std::cout << "current ram bank: " << (int)currRamBank << std::endl;
}