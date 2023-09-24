#include "signal.h"
#include <stdexcept>
#include <iostream>

Regval16 Signal::flags = 0;
Regval16 Signal::flagEnable = 0;
Signal::Signal(){
}

bool Signal::checkFlag(Regval16 mask){
    if(flags & mask){
        return true;
    }
    return false;
}

void Signal::clearSignal(Regval16 mask){
    flags &= ~mask;
}

void Signal::raiseSignal(Regval16 mask){
    if(flagEnable & mask){
        flags |= mask;
    }
}

void Signal::enableSignal(Regval16 mask){
    flagEnable |= mask;
}

bool Signal::signalRaised(Regval16 mask){
    if(flags & flagEnable & mask){
        clearSignal(mask);
        return true;
    }
    return false;
}
