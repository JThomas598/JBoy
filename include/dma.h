#ifndef DMA_H
#define DMA_H
#include "memory.h"

constexpr int DMA_CYCLES = 640;

typedef enum DmaState{
    POLLING,
    TRANSFERING,
}DmaState;

class DMA{
    private:
        Memory mem;
        Register dmaReg;
        Regval8 regVal;
        DmaState state;
        int cyclesLeft;
    
    public:
        DMA();
        void emulateCycle();
};
#endif