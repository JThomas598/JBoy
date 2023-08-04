#include "gameboy.h"
#include "instructions.h"
#include "cpu.h"
#include <time.h>
#include <iostream>
#include <fstream>
#include <stdexcept>

using namespace std;

Gameboy::Gameboy(){
    state = FETCH_OP;
}

void Gameboy::printDebug(char*  s){
    time_t raw_time = time(NULL);
    struct tm* tm = localtime(&raw_time);
    printf("[DEBUG][%d:%d:%d] %s\n", tm->tm_hour, tm->tm_min, tm->tm_sec, s);
}

void Gameboy::printStatus(){
    cpu.printStatus();
}

size_t Gameboy::LoadGame(std::string filename){
    ifstream game(filename, ios_base::in | ios_base::binary);
    if(game.fail()){
        throw invalid_argument("[ERROR] Gameboy::loadGame(): Failed to open requested gb file\n");
    }
    size_t bytes_read = 0;
    while(!game.eof()){
        game.read(mem + bytes_read, BUFSIZ);
        bytes_read += game.gcount();
    }
    printf("[INFO] ROM read successfully. Size: %ld bytes\n", bytes_read);
    return bytes_read;
}

void Gameboy::EmulateCycle(){
    static RegVal_8 imm_8 = 0x00;
    static RegVal_16 imm_16 = 0x0000;
    static RegVal_8 msb = 0x00;
    static RegVal_8 lsb = 0x00;
    if(state == FETCH_OP){
        opcode = mem[cpu.getPC()];
        printf("[INFO] Fetched new opcode. Value: 0x%02x\n", opcode);
    }
    switch(opcode){
        case NOP:
            break; 
        case STOP:
            //TODO
            break; 
        case HALT:
            //TODO
            break;
        case DI:
            cpu.di();
            cpu.incPC();
            break;
        case EI:
            cpu.ei();
            cpu.incPC();
            break;
        case LD_B_B:
            cpu.loadRegReg(B,B);
            cpu.incPC();
            break;
        case LD_B_C:
            cpu.loadRegReg(B,C);
            cpu.incPC();
            break;
        case LD_B_D:
            cpu.loadRegReg(B,D);
            cpu.incPC();
            break;
        case LD_B_E:
            cpu.loadRegReg(B,E);
            cpu.incPC();
            break;
        case LD_B_H:
            cpu.loadRegReg(B,H);
            cpu.incPC();
            break;
        case LD_B_L:
            cpu.loadRegReg(B,L);
            cpu.incPC();
            break;
        case LD_B_A:
            cpu.loadRegReg(B,A);
            cpu.incPC();
            break;
        case LD_C_B:
            cpu.loadRegReg(C,B);
            cpu.incPC();
            break;
        case LD_C_C:
            cpu.loadRegReg(C,C);
            cpu.incPC();
            break;
        case LD_C_D:
            cpu.loadRegReg(C,D);
            cpu.incPC();
            break;
        case LD_C_E:
            cpu.loadRegReg(C,E);
            cpu.incPC();
            break;
        case LD_C_H:
            cpu.loadRegReg(C,H);
            cpu.incPC();
            break;
        case LD_C_L:
            cpu.loadRegReg(C,L);
            cpu.incPC();
            break;
        case LD_C_A:
            cpu.loadRegReg(C,A);
            cpu.incPC();
            break;
        case LD_D_B:
            cpu.loadRegReg(D,B);
            cpu.incPC();
            break;
        case LD_D_C:
            cpu.loadRegReg(D,C);
            cpu.incPC();
            break;
        case LD_D_D:
            cpu.loadRegReg(D,D);
            cpu.incPC();
            break;
        case LD_D_E:
            cpu.loadRegReg(D,E);
            cpu.incPC();
            break;
        case LD_D_H:
            cpu.loadRegReg(D,H);
            cpu.incPC();
            break;
        case LD_D_L:
            cpu.loadRegReg(D,L);
            cpu.incPC();
            break;
        case LD_D_A:
            cpu.loadRegReg(D,A);
            cpu.incPC();
            break;
        case LD_E_B:
            cpu.loadRegReg(E,B);
            cpu.incPC();
            break;
        case LD_E_C:
            cpu.loadRegReg(E,C);
            cpu.incPC();
            break;
        case LD_E_D:
            cpu.loadRegReg(E,D);
            cpu.incPC();
            break;
        case LD_E_E:
            cpu.loadRegReg(E,E);
            cpu.incPC();
            break;
        case LD_E_H:
            cpu.loadRegReg(E,H);
            cpu.incPC();
            break;
        case LD_E_L:
            cpu.loadRegReg(E,L);
            cpu.incPC();
            break;
        case LD_E_A:
            cpu.loadRegReg(E,A);
            cpu.incPC();
            break;
        case LD_H_B:
            cpu.loadRegReg(H,B);
            cpu.incPC();
            break;
        case LD_H_C:
            cpu.loadRegReg(H,C);
            cpu.incPC();
            break;
        case LD_H_D:
            cpu.loadRegReg(H,D);
            cpu.incPC();
            break;
        case LD_H_E:
            cpu.loadRegReg(H,E);
            cpu.incPC();
            break;
        case LD_H_H:
            cpu.loadRegReg(H,H);
            cpu.incPC();
            break;
        case LD_H_L:
            cpu.loadRegReg(H,L);
            cpu.incPC();
            break;
        case LD_H_A:
            cpu.loadRegReg(H,A);
            cpu.incPC();
            break;
        case LD_L_B:
            cpu.loadRegReg(L,B);
            cpu.incPC();
            break;
        case LD_L_C:
            cpu.loadRegReg(L,C);
            cpu.incPC();
            break;
        case LD_L_D:
            cpu.loadRegReg(L,D);
            cpu.incPC();
            break;
        case LD_L_E:
            cpu.loadRegReg(L,E);
            cpu.incPC();
            break;
        case LD_L_H:
            cpu.loadRegReg(L,H);
            cpu.incPC();
            break;
        case LD_L_L:
            cpu.loadRegReg(L,L);
            cpu.incPC();
            break;
        case LD_L_A:
            cpu.loadRegReg(L,A);
            cpu.incPC();
            break;
        case LD_A_B:
            cpu.loadRegReg(A,B);
            cpu.incPC();
            break;
        case LD_A_C:
            cpu.loadRegReg(A,C);
            cpu.incPC();
            break;
        case LD_A_D:
            cpu.loadRegReg(A,D);
            cpu.incPC();
            break;
        case LD_A_E:
            cpu.loadRegReg(A,E);
            cpu.incPC();
            break;
        case LD_A_H:
            cpu.loadRegReg(A,H);
            cpu.incPC();
            break;
        case LD_A_L:
            cpu.loadRegReg(A,L);
            cpu.incPC();
            break;
        case LD_A_A:
            cpu.loadRegReg(A,A);
            cpu.incPC();
            break;
        case LD_B_IMM:
            switch(state){
                case FETCH_OP:
                    state = EXECUTE_1;
                    cpu.incPC();
                    break;
                case EXECUTE_1:
                    imm_8 = mem[cpu.getPC()];
                    cpu.loadRegImm(B, imm_8);
                    state = FETCH_OP;
                    cpu.incPC();
                    break;
                default:
                    break;
            }
            break;
        case LD_C_IMM:
            switch(state){
                case FETCH_OP:
                    state = EXECUTE_1;
                    cpu.incPC();
                    break;
                case EXECUTE_1:
                    imm_8 = mem[cpu.getPC()];
                    cpu.loadRegImm(C, imm_8);
                    state = FETCH_OP;
                    cpu.incPC();
                    break;
                default:
                    break;
            }
            break;
        case LD_D_IMM:
            switch(state){
                case FETCH_OP:
                    state = EXECUTE_1;
                    cpu.incPC();
                    break;
                case EXECUTE_1:
                    imm_8 = mem[cpu.getPC()];
                    cpu.loadRegImm(D, imm_8);
                    state = FETCH_OP;
                    cpu.incPC();
                    break;
                default:
                    break;
            }
            break;
        case LD_E_IMM:
            switch(state){
                case FETCH_OP:
                    state = EXECUTE_1;
                    cpu.incPC();
                    break;
                case EXECUTE_1:
                    imm_8 = mem[cpu.getPC()];
                    cpu.loadRegImm(E, imm_8);
                    state = FETCH_OP;
                    cpu.incPC();
                    break;
                default:
                    break;
            }
            break;
        case LD_H_IMM:
            switch(state){
                case FETCH_OP:
                    state = EXECUTE_1;
                    cpu.incPC();
                    break;
                case EXECUTE_1:
                    imm_8 = mem[cpu.getPC()];
                    cpu.loadRegImm(H, imm_8);
                    state = FETCH_OP;
                    cpu.incPC();
                    break;
                default:
                    break;
            }
            break;
        case LD_L_IMM:
            switch(state){
                case FETCH_OP:
                    state = EXECUTE_1;
                    cpu.incPC();
                    break;
                case EXECUTE_1:
                    imm_8 = mem[cpu.getPC()];
                    cpu.loadRegImm(L, imm_8);
                    state = FETCH_OP;
                    cpu.incPC();
                    break;
                default:
                    break;
            }
            break;
        case LD_A_IMM:
            switch(state){
                case FETCH_OP:
                    state = EXECUTE_1;
                    cpu.incPC();
                    break;
                case EXECUTE_1:
                    imm_8 = mem[cpu.getPC()];
                    cpu.loadRegImm(A, imm_8);
                    state = FETCH_OP;
                    cpu.incPC();
                    break;
                default:
                    break;
            }
            break;
        case LD_B_HL:
            switch(state){
                case FETCH_OP:
                    state = EXECUTE_1;
                    break;
                case EXECUTE_1:
                    cpu.loadRegIndirect(B);
                    state = FETCH_OP;
                    cpu.incPC();
                    break;
                default:
                    break;
            }
            break;
        case LD_C_HL:
            switch(state){
                case FETCH_OP:
                    state = EXECUTE_1;
                    break;
                case EXECUTE_1:
                    cpu.loadRegIndirect(C);
                    state = FETCH_OP;
                    cpu.incPC();
                    break;
                default:
                    break;
            }
            break;
        case LD_D_HL:
            switch(state){
                case FETCH_OP:
                    state = EXECUTE_1;
                    break;
                case EXECUTE_1:
                    cpu.loadRegIndirect(D);
                    state = FETCH_OP;
                    cpu.incPC();
                    break;
                default:
                    break;
            }
            break;
        case LD_E_HL:
            switch(state){
                case FETCH_OP:
                    state = EXECUTE_1;
                    break;
                case EXECUTE_1:
                    cpu.loadRegIndirect(E);
                    state = FETCH_OP;
                    cpu.incPC();
                    break;
                default:
                    break;
            }
            break;
        case LD_H_HL:
            switch(state){
                case FETCH_OP:
                    state = EXECUTE_1;
                    break;
                case EXECUTE_1:
                    cpu.loadRegIndirect(H);
                    state = FETCH_OP;
                    cpu.incPC();
                    break;
                default:
                    break;
            }
            break;
        case LD_L_HL:
            switch(state){
                case FETCH_OP:
                    state = EXECUTE_1;
                    break;
                case EXECUTE_1:
                    cpu.loadRegIndirect(L);
                    state = FETCH_OP;
                    cpu.incPC();
                    break;
                default:
                    break;
            }
            break;
        case LD_A_HL:
            switch(state){
                case FETCH_OP:
                    state = EXECUTE_1;
                    break;
                case EXECUTE_1:
                    cpu.loadRegIndirect(A);
                    state = FETCH_OP;
                    cpu.incPC();
                    break;
                default:
                    break;
            }
            break;
        case LD_HL_B:
            switch(state){
                case FETCH_OP:
                    state = EXECUTE_1;
                    break;
                case EXECUTE_1:
                    cpu.loadIndirectReg(B);
                    state = FETCH_OP;
                    cpu.incPC();
                    break;
                default:
                    break;
            }
            break;
        case LD_HL_C:
            switch(state){
                case FETCH_OP:
                    state = EXECUTE_1;
                    break;
                case EXECUTE_1:
                    cpu.loadIndirectReg(C);
                    state = FETCH_OP;
                    cpu.incPC();
                    break;
                default:
                    break;
            }
            break;
        case LD_HL_D:
            switch(state){
                case FETCH_OP:
                    state = EXECUTE_1;
                    break;
                case EXECUTE_1:
                    cpu.loadIndirectReg(D);
                    state = FETCH_OP;
                    cpu.incPC();
                    break;
                default:
                    break;
            }
            break;
        case LD_HL_E:
            switch(state){
                case FETCH_OP:
                    state = EXECUTE_1;
                    break;
                case EXECUTE_1:
                    cpu.loadIndirectReg(E);
                    state = FETCH_OP;
                    cpu.incPC();
                    break;
                default:
                    break;
            }
            break;
        case LD_HL_H:
            switch(state){
                case FETCH_OP:
                    state = EXECUTE_1;
                    break;
                case EXECUTE_1:
                    cpu.loadIndirectReg(H);
                    state = FETCH_OP;
                    cpu.incPC();
                    break;
                default:
                    break;
            }
            break;
        case LD_HL_L:
            switch(state){
                case FETCH_OP:
                    state = EXECUTE_1;
                    break;
                case EXECUTE_1:
                    cpu.loadIndirectReg(L);
                    state = FETCH_OP;
                    cpu.incPC();
                    break;
                default:
                    break;
            }
            break;
        case LD_HL_A:
            switch(state){
                case FETCH_OP:
                    state = EXECUTE_1;
                    break;
                case EXECUTE_1:
                    cpu.loadIndirectReg(A);
                    state = FETCH_OP;
                    cpu.incPC();
                    break;
                default:
                    break;
            }
            break;
        case LD_HL_IMM_IND:
            switch(state){
                case FETCH_OP:
                    state = FETCH_1;
                    cpu.incPC();
                    break;
                case FETCH_1:
                    state = EXECUTE_1;
                    imm_8 = mem[cpu.getPC()];
                    break;
                case EXECUTE_1:
                    cpu.loadIndirectImm(imm_8);
                    state = FETCH_OP;
                    cpu.incPC();
                    break;
                default:
                    break;
            }
            break;
        case LD_A_BC:
            switch(state){
                case FETCH_OP:
                    state = EXECUTE_1;
                    break;
                case EXECUTE_1:
                    cpu.loadABC();
                    state = FETCH_OP;
                    cpu.incPC();
                    break;
                default:
                    break;
            }
            break;
        case LD_A_DE:
            switch(state){
                case FETCH_OP:
                    state = EXECUTE_1;
                    break;
                case EXECUTE_1:
                    cpu.loadADE();
                    state = FETCH_OP;
                    cpu.incPC();
                    break;
                default:
                    break;
            }
            break;
        case LD_BC_A:
            switch(state){
                case FETCH_OP:
                    state = EXECUTE_1;
                    break;
                case EXECUTE_1:
                    cpu.loadBCA();
                    state = FETCH_OP;
                    cpu.incPC();
                    break;
                default:
                    break;
            }
            break;
        case LD_DE_A:
            switch(state){
                case FETCH_OP:
                    state = EXECUTE_1;
                    break;
                case EXECUTE_1:
                    cpu.loadDEA();
                    state = FETCH_OP;
                    cpu.incPC();
                    break;
                default:
                    break;
            }
            break;
        case LD_A_ADDR:
            switch(state){
                case FETCH_OP:
                    state = FETCH_1;
                    cpu.incPC(); 
                    break;
                case FETCH_1:
                    lsb = mem[cpu.getPC()];
                    state = FETCH_2;
                    cpu.incPC(); 
                    break;
                case FETCH_2:
                    msb = mem[cpu.getPC()];
                    state = EXECUTE_1;
                    break;
                case EXECUTE_1:
                    imm_16 = 0x0000;
                    imm_16 |= msb;
                    imm_16 = imm_16 << 8;
                    imm_16 |= lsb;
                    cpu.loadAImmIndirect(imm_16);
                    state = FETCH_OP;
                    cpu.incPC();
                    break;
                default:
                    break;
            }
            break;
        case LD_ADDR_A:
            switch(state){
                case FETCH_OP:
                    state = FETCH_1;
                    cpu.incPC(); 
                    break;
                case FETCH_1:
                    lsb = mem[cpu.getPC()];
                    state = FETCH_2;
                    cpu.incPC(); 
                    break;
                case FETCH_2:
                    msb = mem[cpu.getPC()];
                    state = EXECUTE_1;
                    break;
                case EXECUTE_1:
                    imm_16 = 0x0000;
                    imm_16 |= msb;
                    imm_16 = imm_16 << 8;
                    imm_16 |= lsb;
                    cpu.loadImmAIndirect(imm_16);
                    state = FETCH_OP;
                    cpu.incPC();
                    break;
                default:
                    break;
            }
            break;
        case LDH_A_C:
            switch(state){
                case FETCH_OP:
                    state = EXECUTE_1;
                    break;
                case EXECUTE_1:
                    cpu.loadHighAC();
                    state = FETCH_OP;
                    cpu.incPC();
                    break;
                default:
                    break;
            }
            break;
        case LDH_C_A:
            switch(state){
                case FETCH_OP:
                    state = EXECUTE_1;
                    break;
                case EXECUTE_1:
                    cpu.loadHighCA();
                    state = FETCH_OP;
                    cpu.incPC();
                    break;
                default:
                    break;
            }
            break;
        case LDH_A_IMM:
            switch(state){
                case FETCH_OP:
                    state = FETCH_1;
                    cpu.incPC();
                    break;
                case FETCH_1:
                    imm_8 = mem[cpu.getPC()];
                    state = EXECUTE_1;
                    break;
                case EXECUTE_1:
                    cpu.loadHighAImm(imm_8);
                    state = FETCH_OP;
                    cpu.incPC();
                    break;
                default:
                    break;
            }
            break;
        case LDH_IMM_A:
            switch(state){
                case FETCH_OP:
                    state = FETCH_1;
                    cpu.incPC();
                    break;
                case FETCH_1:
                    imm_8 = mem[cpu.getPC()];
                    state = EXECUTE_1;
                    break;
                case EXECUTE_1:
                    cpu.loadHighImmA(imm_8);
                    state = FETCH_OP;
                    cpu.incPC();
                    break;
                default:
                    break;
            }
            break;
        case LD_A_HL_DEC:
            switch(state){
                case FETCH_OP:
                    state = EXECUTE_1;
                    break;
                case EXECUTE_1:
                    cpu.loadAIndirectDec();
                    state = FETCH_OP;
                    cpu.incPC();
                    break;
                default:
                    break;
            }
            break;
        case LD_HL_A_DEC:
            switch(state){
                case FETCH_OP:
                    state = EXECUTE_1;
                    break;
                case EXECUTE_1:
                    cpu.loadIndirectADec();
                    state = FETCH_OP;
                    cpu.incPC();
                    break;
                default:
                    break;
            }
            break;
        case LD_A_HL_INC:
            switch(state){
                case FETCH_OP:
                    state = EXECUTE_1;
                    break;
                case EXECUTE_1:
                    cpu.loadAIndirectInc();
                    state = FETCH_OP;
                    cpu.incPC();
                    break;
                default:
                    break;
            }
            break;
        case LD_HL_A_INC:
            switch(state){
                case FETCH_OP:
                    state = EXECUTE_1;
                    break;
                case EXECUTE_1:
                    cpu.loadIndirectAInc();
                    state = FETCH_OP;
                    cpu.incPC();
                    break;
                default:
                    break;
            }
            break;
        case LD_BC_IMM:
            switch(state){
                case FETCH_OP:
                    state = FETCH_1;
                    cpu.incPC();
                    break;
                case FETCH_1:
                    lsb = mem[cpu.getPC()];
                    cpu.incPC();
                    state = EXECUTE_1;
                    break;
                case EXECUTE_1:
                    msb = mem[cpu.getPC()];
                    imm_16 = 0x0000;
                    imm_16 |= msb;
                    imm_16 = imm_16 << 8;
                    imm_16 |= lsb;
                    cpu.loadRegPairImm(B,C,imm_16);
                    state = FETCH_OP;
                    cpu.incPC();
                    break;
                default:
                    break;
            }
            break;
        case LD_DE_IMM:
            switch(state){
                case FETCH_OP:
                    state = FETCH_1;
                    cpu.incPC();
                    break;
                case FETCH_1:
                    lsb = mem[cpu.getPC()];
                    state = EXECUTE_1;
                    cpu.incPC();
                    break;
                case EXECUTE_1:
                    msb = mem[cpu.getPC()];
                    imm_16 = 0x0000;
                    imm_16 |= msb;
                    imm_16 = imm_16 << 8;
                    imm_16 |= lsb;
                    cpu.loadRegPairImm(D,E,imm_16);
                    state = FETCH_OP;
                    cpu.incPC();
                    break;
                default:
                    break;
            }
            break;
        case LD_HL_IMM:
            switch(state){
                case FETCH_OP:
                    state = FETCH_1;
                    cpu.incPC();
                    break;
                case FETCH_1:
                    lsb = mem[cpu.getPC()];
                    state = EXECUTE_1;
                    cpu.incPC();
                    break;
                case EXECUTE_1:
                    msb = mem[cpu.getPC()];
                    imm_16 = 0x0000;
                    imm_16 |= msb;
                    imm_16 = imm_16 << 8;
                    imm_16 |= lsb;
                    cpu.loadRegPairImm(H,L,imm_16);
                    state = FETCH_OP;
                    cpu.incPC();
                    break;
                default:
                    break;
            }
            break;
        case LD_SP_IMM:
            switch(state){
                case FETCH_OP:
                    state = FETCH_1;
                    cpu.incPC();
                    break;
                case FETCH_1:
                    lsb = mem[cpu.getPC()];
                    state = EXECUTE_1;
                    cpu.incPC();
                    break;
                case EXECUTE_1:
                    msb = mem[cpu.getPC()];
                    imm_16 = 0x0000;
                    imm_16 |= msb;
                    imm_16 = imm_16 << 8;
                    imm_16 |= lsb;
                    cpu.loadSPImm(imm_16);
                    state = FETCH_OP;
                    cpu.incPC();
                    break;
                default:
                    break;
            }
            break;
        case LD_DIR_SP:
            switch(state){
                case FETCH_OP:
                    state = FETCH_1;
                    cpu.incPC();
                    break;
                case FETCH_1:
                    lsb = mem[cpu.getPC()];
                    state = FETCH_2;
                    cpu.incPC();
                    break;
                case FETCH_2:
                    msb = mem[cpu.getPC()];
                    state = EXECUTE_1;
                    break;
                case EXECUTE_1:
                    imm_16 = 0x0000;
                    imm_16 |= msb;
                    imm_16 = imm_16 << 8;
                    state = EXECUTE_2;
                    break;
                case EXECUTE_2:
                    imm_16 |= lsb;
                    cpu.loadDirectSP(imm_16);
                    state = FETCH_OP;
                    cpu.incPC();
                    break;
                default:
                    break;
            }
            break;
        case LD_SP_HL:
            switch(state){
                case FETCH_OP:
                    state = EXECUTE_1;
                    break;
                case EXECUTE_1:
                    cpu.loadSPHL();
                    state = FETCH_OP;
                    cpu.incPC();
                    break;
                default:
                    break;
            }
            break;
        case PUSH_BC:
            switch(state){
                case FETCH_OP:
                    state = EXECUTE_1;
                    break;
                case EXECUTE_1:
                    state = EXECUTE_2;
                    break;
                case EXECUTE_2:
                    state = EXECUTE_3;
                    break;
                case EXECUTE_3:
                    cpu.push(B,C);
                    state = FETCH_OP;
                    cpu.incPC();
                    break;
                default:
                    break;
            }
            break;
        case PUSH_DE:
            switch(state){
                case FETCH_OP:
                    state = EXECUTE_1;
                    break;
                case EXECUTE_1:
                    state = EXECUTE_2;
                    break;
                case EXECUTE_2:
                    state = EXECUTE_3;
                    break;
                case EXECUTE_3:
                    cpu.push(D,E);
                    state = FETCH_OP;
                    cpu.incPC();
                    break;
                default:
                    break;
            }
            break;
        case PUSH_HL:
            switch(state){
                case FETCH_OP:
                    state = EXECUTE_1;
                    break;
                case EXECUTE_1:
                    state = EXECUTE_2;
                    break;
                case EXECUTE_2:
                    state = EXECUTE_3;
                    break;
                case EXECUTE_3:
                    cpu.push(H,L);
                    state = FETCH_OP;
                    cpu.incPC();
                    break;
                default:
                    break;
            }
            break;
        case PUSH_AF:
            switch(state){
                case FETCH_OP:
                    state = EXECUTE_1;
                    break;
                case EXECUTE_1:
                    state = EXECUTE_2;
                    break;
                case EXECUTE_2:
                    state = EXECUTE_3;
                    break;
                case EXECUTE_3:
                    cpu.push(A,F);
                    state = FETCH_OP;
                    cpu.incPC();
                    break;
                default:
                    break;
            }
            break;
        case POP_BC:
            switch(state){
                case FETCH_OP:
                    state = EXECUTE_1;
                    break;
                case EXECUTE_1:
                    state = EXECUTE_2;
                    break;
                case EXECUTE_2:
                    state = EXECUTE_3;
                    break;
                case EXECUTE_3:
                    cpu.pop(B,C);
                    state = FETCH_OP;
                    cpu.incPC();
                    break;
                default:
                    break;
            }
            break;
        case POP_DE:
            switch(state){
                case FETCH_OP:
                    state = EXECUTE_1;
                    break;
                case EXECUTE_1:
                    state = EXECUTE_2;
                    break;
                case EXECUTE_2:
                    state = EXECUTE_3;
                    break;
                case EXECUTE_3:
                    cpu.pop(D,E); 
                    state = FETCH_OP;
                    cpu.incPC();
                    break;
                default:
                    break;
            }
            break;
        case POP_HL:
            switch(state){
                case FETCH_OP:
                    state = EXECUTE_1;
                    break;
                case EXECUTE_1:
                    state = EXECUTE_2;
                    break;
                case EXECUTE_2:
                    state = EXECUTE_3;
                    break;
                case EXECUTE_3:
                    cpu.pop(H,L); 
                    state = FETCH_OP;
                    cpu.incPC();
                    break;
                default:
                    break;
            }
            break;
        case POP_AF:
            switch(state){
                case FETCH_OP:
                    state = EXECUTE_1;
                    break;
                case EXECUTE_1:
                    state = EXECUTE_2;
                    break;
                case EXECUTE_2:
                    state = EXECUTE_3;
                    break;
                case EXECUTE_3:
                    cpu.pop(A,F); 
                    state = FETCH_OP;
                    cpu.incPC();
                    break;
                default:
                    break;
            }
            break;
        case ADD_A:
            cpu.addReg(A);
            cpu.incPC();
            break;
        case ADD_B:
            cpu.addReg(B);
            cpu.incPC();
            break;
        case ADD_C:
            cpu.addReg(C);
            cpu.incPC();
            break;
        case ADD_D:
            cpu.addReg(D);
            cpu.incPC();
            break;
        case ADD_E:
            cpu.addReg(E);
            cpu.incPC();
            break;
        case ADD_H:
            cpu.addReg(H);
            cpu.incPC();
            break;
        case ADD_L:
            cpu.addReg(L);
            cpu.incPC();
            break;
        case ADD_HL:
            cpu.addIndirect();
            cpu.incPC();
            break;
        case JP:
            switch(state){
                case FETCH_OP:
                    state = FETCH_1;
                    cpu.incPC(); 
                    break;
                case FETCH_1:
                    lsb = mem[cpu.getPC()];
                    state = FETCH_2;
                    cpu.incPC(); 
                    break;
                case FETCH_2:
                    msb = mem[cpu.getPC()];
                    state = EXECUTE_1;
                    break;
                case EXECUTE_1:
                    imm_16 = 0x0000;
                    imm_16 |= msb;
                    imm_16 = imm_16 << 8;
                    imm_16 |= lsb;
                    cpu.jump(imm_16);
                    state = FETCH_OP;
                    break;
                default:
                    break;
            }
            break;
        default:
            throw runtime_error("[ERROR] Opcode could not be resolved.\n");
            break;
    }
}