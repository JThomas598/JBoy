#include "util.h"
#include <stdexcept>

bool util::checkBit(Regval8 val, int index){
    if(index < 0 || index > 7){
        throw std::invalid_argument("Util::checkBit(): passed bit index is not valid.");
    }
    val >>= index;
    if(val & 0x01){
        return true;
    }
    return false;
}