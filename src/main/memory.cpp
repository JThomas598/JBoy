#include "memory.h"

RegVal_8 Memory::write(RegVal_16 addr, RegVal_8 byte){
    mem[addr] = byte;
    return mem[addr];
}

RegVal_8 Memory::read(RegVal_16 addr){
    return mem[addr];
}

RegVal_16 Memory::dump(RegVal_16 addr, size_t n, RegVal_8* buf){
    size_t i = 0;
    while(i < n){
        mem[addr + i] = buf[i];
        i++;
    }
    return i;
}