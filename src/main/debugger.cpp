#include "debugger.h"
#include "gameboy.h"
#include <algorithm>
#include <chrono>

Debugger::Debugger(Gameboy& rgb) : gb(rgb){
}

void Debugger::runToBreakpoint(){
    while(true){
        RegVal_16 addr = gb.emulateCycle();
        std::vector<RegVal_16>::iterator it = std::find(breakPts.begin(), breakPts.end(), addr);
        if(it != breakPts.end()){
            break;
        }
    }
}

bool Debugger::addBreakpoint(RegVal_16 addr){
    std::vector<RegVal_16>::iterator it = std::find(breakPts.begin(), breakPts.end(), addr);
    if(it != breakPts.end()){
        return false;
    }
    breakPts.push_back(addr);
    return true;
}

void Debugger::step(){
    gb.emulateCycle();
}

void Debugger::printStatus(){
    gb.printStatus();
}
