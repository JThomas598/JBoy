#include "memory.h"
#include "dma.h"
#include <iostream>

DMA::DMA() : mem(DMA_PERM), dmaReg(mem.getRegister(DMA_REG)){
    state = POLLING;
    cyclesLeft = DMA_CYCLES;
}

void DMA::emulateCycle(){
    switch (state){
    case POLLING:
        if(dmaReg){
            regVal = dmaReg;
            dmaReg = 0x00; //reset the register for next request
            state = TRANSFERING;
            cyclesLeft = DMA_CYCLES;
            break;
        }
        break;
    case TRANSFERING:
        if(!cyclesLeft){
            Regval16 srcPtr = regVal * 0x0100;
            Regval16 dstPtr = OAM_START;
            for(int i = 0; i < 160; i++){
                mem.write(dstPtr, mem.read(srcPtr));
                srcPtr++;
                dstPtr++;
            }
            state = POLLING;
            break;
        }
        cyclesLeft--;
        break;
    default:
        break;
    }

}