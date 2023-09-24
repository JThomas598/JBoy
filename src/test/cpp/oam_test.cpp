#include "memory.h"
#include  "oam.h"
#include <iostream>
#include <string>


using namespace std;

void createObject(Regval8 entryNum, Regval8 yPos, Regval8 xPos, Memory& mem);

int main(int argc, char** argv){
    Memory mem(SYS_PERM);
    OAM oam;
    //test 1
    int xPos;
    int yPos;
    int entry;
    int lineNum;
    string command;
    while(true){
        std::cin >> command;
        if(command == "create"){
            std::cout << "enter the oam entry index, x pos and y pos, separated by spaces: ";
            std::cin >> entry >> xPos >> yPos; 
            std::cout << "entry = " << entry << " x pos = " << xPos << " y pos = " << yPos << std::endl; 
            createObject(entry, yPos, xPos, mem);
        }
        else if(command == "scan"){
            std::cout << "enter desired line number to scan: ";
            std::cin >> lineNum;
            std::cout << "detected " << (int)oam.searchLine(lineNum) << " objects during scan." << std::endl; 
        }
        else if(command == "min"){
            std::cout << "leftmost x pos: " << (int)oam.getMinX() << std:: endl;
        }
    }
    return 0;
}

void createObject(Regval8 entryNum, Regval8 yPos, Regval8 xPos, Memory& mem){
    Regval16 addr = OAM_START + (entryNum * OBJECT_SIZE);
    mem.write(addr, yPos);
    mem.write(addr+1, xPos);
    mem.write(addr+2, 0);
    mem.write(addr+2, 0);
}