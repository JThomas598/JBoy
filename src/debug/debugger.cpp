#include "debugger.h"
#include <algorithm>
#include <chrono>

Debugger::Debugger(Gameboy& rgb) : gb(rgb){
}

void Debugger::runToBreakpoint(){
    while(true){
        Regval16 addr = gb.emulateCycle();
        std::vector<Regval16>::iterator it = std::find(breakPts.begin(), breakPts.end(), addr);
        if(it != breakPts.end() || signal.signalRaised(ALL_SIGNALS)){
            break;
        }
    }
}

void Debugger::enableSignal(Regval16 mask){
    signal.enableSignal(mask);
}

void Debugger::runToSignal(){
    while(true){
        gb.emulateCycle(); 
    }
}

Regval8 Debugger::readMem(Regval16 addr){
    return gb.readMem(addr);
}

bool Debugger::addBreakpoint(Regval16 addr){
    std::vector<Regval16>::iterator it = std::find(breakPts.begin(), breakPts.end(), addr);
    if(it != breakPts.end()){
        return false;
    }
    breakPts.push_back(addr);
    return true;
}

void Debugger::step(int numSteps){
    for(int i = 0; i < numSteps; i++){
        gb.emulateCycle();
    }
}

void Debugger::printStatus(){
    gb.printStatus();
}
