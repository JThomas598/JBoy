#include "cpu.h"
#include <stdexcept>


CPU::CPU(){
    for(int i = 0; i <= NUM_REGS_8; i++){
        regs_8[i] = 0;
    }
    for(int i = 0; i <= NUM_REGS_16; i++){
        regs_16[i] = 0;
    }
}

void CPU::Print(){
    printf("A:   0x%02x\n", regs_8[A]);
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
    printf("----F----\n");
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