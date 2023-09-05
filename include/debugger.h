#include "gameboy.h"
#include <vector>
#include <stdbool.h>
#include <chrono>

class Debugger{
    private:
        std::vector<RegVal_16> breakPts;
        Gameboy& gb;

    public:
        Debugger(Gameboy& rgb);
        void runToBreakpoint();
        void step();
        bool addBreakpoint(const RegVal_16 addr);
        void printStatus();
};