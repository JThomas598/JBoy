#include "gameboy.h"
#include <vector>
#include <stdbool.h>
#include <chrono>
#include "signal.h"

class Debugger{
    private:
        std::vector<Regval16> breakPts;
        Gameboy& gb;
        Signal signal;

    public:
        Debugger(Gameboy& rgb);
        void runToBreakpoint();
        void runToSignal();
        void enableSignal(Regval16 mask);
        void step(int numSteps);
        Regval8 readMem(Regval16 addr);
        bool addBreakpoint(const Regval16 addr);
        void printStatus();
};