#include "gameboy.h"
#include "instructions.h"
#include "cpu.h"
#include <iostream>
#include <fstream>

using namespace std;

Gameboy::Gameboy(){
}

size_t Gameboy::LoadGame(std::string filename){
    ifstream game(filename, ios_base::in | ios_base::binary);
    size_t bytes_read = 0;
    while(!game.eof()){
        game.read(mem + bytes_read, BUFSIZ);
        bytes_read += game.gcount();
    }
    printf("rom size: %lld\n bytes", bytes_read);
    return bytes_read;
}

void Gameboy::EmulateCycle(){
    char opcode = mem[cpu.getPC()];
    switch(opcode){
        case NOP:
            cpu.incPC(1);
            break; 
        case ADD_A:
            cpu.addAReg(A);
            cpu.incPC(1);
            break;
        case ADD_B:
            cpu.addAReg(B);
            cpu.incPC(1);
            break;
        case ADD_C:
            cpu.addAReg(C);
            cpu.incPC(1);
            break;
        case ADD_D:
            cpu.addAReg(D);
            cpu.incPC(1);
            break;
        case ADD_E:
            cpu.addAReg(E);
            cpu.incPC(1);
            break;
        case ADD_H:
            cpu.addAReg(H);
            cpu.incPC(1);
            break;
        case ADD_L:
            cpu.addAReg(L);
            cpu.incPC(1);
            break;
        case ADD_HL:
            cpu.addIndirect();
            cpu.incPC(1);
            break;
    }
}