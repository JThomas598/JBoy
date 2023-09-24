#include "memory.h"

class Counters{
    private:
        Memory mem;
        Register divReg;
        Register timaReg;
        Register tmaReg; 
        Register tacReg;
        Register intFlagReg;
        int divCycleCount;
        int timaCycleCount;
    public:
        Counters();
        void emulateCycle();
};