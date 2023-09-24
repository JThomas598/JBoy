#include "oam.h"
#include "lcd.h"
#include "util.h"
#include <algorithm>
#include <iostream>
#include <stdexcept>

bool comp(const Object& obj1, const Object& obj2){
    return obj1.x_pos > obj2.x_pos;
}

OAM::OAM() :
mem(OAM_PERM),
lcdcReg(mem.getRegister(LCDC_REG_ADDR))
{}

Regval8 OAM::searchLine(const Regval8 lineNum){
    //remember to include logic for 8 x 16 sprites later
    Regval16 objEntryPtr = OAM_START;
    int numDetections = 0;
    for(int i = 0; i < OBJECT_MAX; i++){
        //yPos is first byte of obj entry
        const Regval8 yPos = mem.read(objEntryPtr);  
        const Regval8 xPos = mem.read(objEntryPtr + 1);
        const Regval8 objHeight = util::checkBit(lcdcReg, LCDC_OBJ_SIZE) ? 16 : 8;
        bool horVisible = xPos > 0 && xPos < 168;
        bool vertVisible = lineNum + 16 >= yPos && lineNum + 16 < yPos + objHeight;
        if(horVisible && vertVisible){
            Object obj;
            obj.y_pos = mem.read(objEntryPtr),
            obj.x_pos = mem.read(objEntryPtr + 1),
            obj.tileIndex = mem.read(objEntryPtr + 2),
            obj.flags = mem.read(objEntryPtr + 3);
            visibleObjs.push_back(obj);
            if(visibleObjs.size() == 10){
                break;
            }
            numDetections++;
        }
        objEntryPtr += OBJECT_SIZE;
    }
    //sort objects by their x positions
    std::sort(visibleObjs.begin(), visibleObjs.end(), comp);
    return numDetections;
}

Regval8 OAM::getMinX(){
    if(visibleObjs.size() == 0){
        return 0xFF;
    }
    return visibleObjs.back().x_pos;
}

Object OAM::popObj(){
    if(visibleObjs.size() == 0){
        throw std::logic_error("OAM::popObj: attempted pop with no visible objects on line.");
    }
    Object retval = visibleObjs.back();
    visibleObjs.pop_back();
    return retval;
}

void OAM::clearQueue(){
    std::vector<Object> empty;
    std::swap(visibleObjs, empty);
}