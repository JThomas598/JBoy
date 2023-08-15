#include "gameboy.h"
#include "instructions.h"
#include "cpu.h"
#include <time.h>
#include <iostream>
#include <fstream>
#include <stdexcept>

using namespace std;

RegVal_8 mem[MEM_SIZE];

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
        game.read((char*)mem + bytes_read, BUFSIZ);
        bytes_read += game.gcount();
    }
    printf("[INFO] ROM read successfully. Size: %ld bytes\n", bytes_read);
    return bytes_read;
}

void Gameboy::EmulateCycle(){
    if(state == FETCH_OP){
        if(IME){
            //handle interrupts
        }
        opcode = mem[cpu.getPC()];
        #ifdef debug
            printf("[DEBUG] Fetched new opcode. Value: 0x%02x\n", opcode);
        #endif
    }
    switch(opcode){
        case NOP:
            cpu.incPC(); 
            break; 
        case STOP:
            //TODO
            cpu.incPC();
            break; 
        case HALT:
            //TODO
            cpu.incPC();
            break;
        case DI:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: DI\n");
            #endif
            cpu.di();
            cpu.incPC();
            break;
        case EI:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: EI\n");
            #endif
            cpu.ei();
            cpu.incPC();
            break;
        case LD_B_B:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: LD B,B\n");
            #endif
            cpu.loadRegReg(B,B);
            cpu.incPC();
            break;
        case LD_B_C:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: LD B,C\n");
            #endif
                cpu.loadRegReg(B,C);
            cpu.incPC();
            break;
        case LD_B_D:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: LD B,D\n");
            #else
                cpu.loadRegReg(B,D);
            #endif
            cpu.incPC();
            break;
        case LD_B_E:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: LD B,E\n");
            #endif
            cpu.loadRegReg(B,E);
            cpu.incPC();
            break;
        case LD_B_H:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: LD B,H\n");
            #endif
            cpu.loadRegReg(B,H);
            cpu.incPC();
            break;
        case LD_B_L:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: LD B,L\n");
            #endif
            cpu.loadRegReg(B,L);
            cpu.incPC();
            break;
        case LD_B_A:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: LD B,A\n");
            #endif
            cpu.loadRegReg(B,A);
            cpu.incPC();
            break;
        case LD_C_B:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: LD C,B\n");
            #endif
            cpu.loadRegReg(C,B);
            cpu.incPC();
            break;
        case LD_C_C:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: LD C,C\n");
            #endif
            cpu.loadRegReg(C,C);
            cpu.incPC();
            break;
        case LD_C_D:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: LD C,D\n");
            #endif
            cpu.loadRegReg(C,D);
            cpu.incPC();
            break;
        case LD_C_E:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: LD C,E\n");
            #endif
            cpu.loadRegReg(C,E);
            cpu.incPC();
            break;
        case LD_C_H:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: LD C,H\n");
            #endif
            cpu.loadRegReg(C,H);
            cpu.incPC();
            break;
        case LD_C_L:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: LD C,L\n");
            #endif
            cpu.loadRegReg(C,L);
            cpu.incPC();
            break;
        case LD_C_A:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: LD C,A\n");
            #endif
            cpu.loadRegReg(C,A);
            cpu.incPC();
            break;
        case LD_D_B:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: LD D,B\n");
            #endif
            cpu.loadRegReg(D,B);
            cpu.incPC();
            break;
        case LD_D_C:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: LD D,C\n");
            #endif
            cpu.loadRegReg(D,C);
            cpu.incPC();
            break;
        case LD_D_D:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: LD D,D\n");
            #endif
            cpu.loadRegReg(D,D);
            cpu.incPC();
            break;
        case LD_D_E:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: LD D,E\n");
            #endif
            cpu.loadRegReg(D,E);
            cpu.incPC();
            break;
        case LD_D_H:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: LD D,H\n");
            #endif
            cpu.loadRegReg(D,H);
            cpu.incPC();
            break;
        case LD_D_L:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: LD D,L\n");
            #endif
            cpu.loadRegReg(D,L);
            cpu.incPC();
            break;
        case LD_D_A:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: LD D,A\n");
            #endif
            cpu.loadRegReg(D,A);
            cpu.incPC();
            break;
        case LD_E_B:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: LD E,B\n");
            #endif
            cpu.loadRegReg(E,B);
            cpu.incPC();
            break;
        case LD_E_C:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: LD E,C\n");
            #endif
            cpu.loadRegReg(E,C);
            cpu.incPC();
            break;
        case LD_E_D:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: LD E,D\n");
            #endif
            cpu.loadRegReg(E,D);
            cpu.incPC();
            break;
        case LD_E_E:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: LD E,E\n");
            #endif
            cpu.loadRegReg(E,E);
            cpu.incPC();
            break;
        case LD_E_H:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: LD E,H\n");
            #endif
            cpu.loadRegReg(E,H);
            cpu.incPC();
            break;
        case LD_E_L:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: LD E,L\n");
            #endif
            cpu.loadRegReg(E,L);
            cpu.incPC();
            break;
        case LD_E_A:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: LD E,A\n");
            #endif
            cpu.loadRegReg(E,A);
            cpu.incPC();
            break;
        case LD_H_B:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: LD H,B\n");
            #endif
            cpu.loadRegReg(H,B);
            cpu.incPC();
            break;
        case LD_H_C:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: LD H,C\n");
            #endif
            cpu.loadRegReg(H,C);
            cpu.incPC();
            break;
        case LD_H_D:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: LD H,D\n");
            #endif
            cpu.loadRegReg(H,D);
            cpu.incPC();
            break;
        case LD_H_E:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: LD H,E\n");
            #endif
            cpu.loadRegReg(H,E);
            cpu.incPC();
            break;
        case LD_H_H:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: LD H,H\n");
            #endif
            cpu.loadRegReg(H,H);
            cpu.incPC();
            break;
        case LD_H_L:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: LD H,L\n");
            #endif
            cpu.loadRegReg(H,L);
            cpu.incPC();
            break;
        case LD_H_A:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: LD H,A\n");
            #endif
            cpu.loadRegReg(H,A);
            cpu.incPC();
            break;
        case LD_L_B:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: LD L,B\n");
            #endif
            cpu.loadRegReg(L,B);
            cpu.incPC();
            break;
        case LD_L_C:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: LD L,C\n");
            #endif
            cpu.loadRegReg(L,C);
            cpu.incPC();
            break;
        case LD_L_D:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: LD L,D\n");
            #endif
            cpu.loadRegReg(L,D);
            cpu.incPC();
            break;
        case LD_L_E:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: LD L,E\n");
            #endif
            cpu.loadRegReg(L,E);
            cpu.incPC();
            break;
        case LD_L_H:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: LD L,H\n");
            #endif
            cpu.loadRegReg(L,H);
            cpu.incPC();
            break;
        case LD_L_L:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: LD L,L\n");
            #endif
            cpu.loadRegReg(L,L);
            cpu.incPC();
            break;
        case LD_L_A:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: LD L,A\n");
            #endif
            cpu.loadRegReg(L,A);
            cpu.incPC();
            break;
        case LD_A_B:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: LD A,B\n");
            #endif
            cpu.loadRegReg(A,B);
            cpu.incPC();
            break;
        case LD_A_C:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: LD A,C\n");
            #endif
            cpu.loadRegReg(A,C);
            cpu.incPC();
            break;
        case LD_A_D:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: LD A,D\n");
            #endif
            cpu.loadRegReg(A,D);
            cpu.incPC();
            break;
        case LD_A_E:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: LD A,E\n");
            #endif
            cpu.loadRegReg(A,E);
            cpu.incPC();
            break;
        case LD_A_H:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: LD A,H\n");
            #endif
            cpu.loadRegReg(A,H);
            cpu.incPC();
            break;
        case LD_A_L:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: LD A,L\n");
            #endif
            cpu.loadRegReg(A,L);
            cpu.incPC();
            break;
        case LD_A_A:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: LD A,A\n");
            #endif
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
                    #ifdef debug
                        printf("[DEBUG] instrunction resolved to: LD B,%u\n", imm_8);
                    #endif
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
                    #ifdef debug
                        printf("[DEBUG] instrunction resolved to: LD C,%u\n", imm_8);
                    #endif
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
                    #ifdef debug
                        printf("[DEBUG] instrunction resolved to: LD D,%u\n", imm_8);
                    #endif
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
                    #ifdef debug
                        printf("[DEBUG] instrunction resolved to: LD E,%u\n", imm_8);
                    #endif
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
                    #ifdef debug
                        printf("[DEBUG] instrunction resolved to: LD H,%u\n", imm_8);
                    #endif
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
                    #ifdef debug
                        printf("[DEBUG] instrunction resolved to: LD L,%u\n", imm_8);
                    #endif
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
                    #ifdef debug
                        printf("[DEBUG] instrunction resolved to: LD A,%u\n", imm_8);
                    #endif
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
                    #ifdef debug
                        printf("[DEBUG] instrunction resolved to: LD B,(HL)\n");
                    #endif
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
                    #ifdef debug
                        printf("[DEBUG] instrunction resolved to: LD C,(HL)\n");
                    #endif
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
                    #ifdef debug
                        printf("[DEBUG] instrunction resolved to: LD D,(HL)\n");
                    #endif
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
                    #ifdef debug
                        printf("[DEBUG] instrunction resolved to: LD E,(HL)\n");
                    #endif
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
                    #ifdef debug
                        printf("[DEBUG] instrunction resolved to: LD H,(HL)\n");
                    #endif
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
                    #ifdef debug
                        printf("[DEBUG] instrunction resolved to: LD L,(HL)\n");
                    #endif
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
                    #ifdef debug
                        printf("[DEBUG] instrunction resolved to: LD A,(HL)\n");
                    #endif
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
                    #ifdef debug
                        printf("[DEBUG] instrunction resolved to: LD (HL),B\n");
                    #endif
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
                    #ifdef debug
                        printf("[DEBUG] instrunction resolved to: LD (HL),C\n");
                    #endif
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
                    #ifdef debug
                        printf("[DEBUG] instrunction resolved to: LD (HL),D\n");
                    #endif
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
                    #ifdef debug
                        printf("[DEBUG] instrunction resolved to: LD (HL),E\n");
                    #endif
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
                    #ifdef debug
                        printf("[DEBUG] instrunction resolved to: LD (HL),H\n");
                    #endif
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
                    #ifdef debug
                        printf("[DEBUG] instrunction resolved to: LD (HL),L\n");
                    #endif
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
                    #ifdef debug
                        printf("[DEBUG] instrunction resolved to: LD (HL),A\n");
                    #endif
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
                    #ifdef debug
                        printf("[DEBUG] instrunction resolved to: LD (HL),%u\n",imm_8);
                    #endif
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
                    #ifdef debug
                        printf("[DEBUG] instrunction resolved to: LD A,(BC)\n");
                    #endif
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
                    #ifdef debug
                        printf("[DEBUG] instrunction resolved to: LD A,(DE)\n");
                    #endif
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
                    #ifdef debug
                        printf("[DEBUG] instrunction resolved to: LD (BC),A\n");
                    #endif
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
                    #ifdef debug
                        printf("[DEBUG] instrunction resolved to: LD (DE),A\n");
                    #endif
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
                    #ifdef debug
                        printf("[DEBUG] instrunction resolved to: LD A,(0x%04x)\n",imm_16);
                    #endif
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
                    #ifdef debug
                        printf("[DEBUG] instrunction resolved to: LD (0x%04x),A\n", imm_16);
                    #endif
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
                    #ifdef debug
                        printf("[DEBUG] instrunction resolved to: LDH A,(C)\n");
                    #endif
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
                    #ifdef debug
                        printf("[DEBUG] instrunction resolved to: LDH (C),A\n");
                    #endif
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
                    #ifdef debug
                        printf("[DEBUG] instrunction resolved to: LDH A,(0x%02x)\n", imm_8);
                    #endif
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
                    #ifdef debug
                        printf("[DEBUG] instrunction resolved to: LDH (0x%02x),A\n", imm_8);
                    #endif
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
                    #ifdef debug
                        printf("[DEBUG] instrunction resolved to: LD A,(HL-)\n");
                    #endif
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
                    #ifdef debug
                        printf("[DEBUG] instrunction resolved to: LD (HL-),A\n");
                    #endif
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
                    #ifdef debug
                        printf("[DEBUG] instrunction resolved to: LD A,(HL+)\n");
                    #endif
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
                    #ifdef debug
                        printf("[DEBUG] instrunction resolved to: LD (HL+),A\n");
                    #endif
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
                    #ifdef debug
                        printf("[DEBUG] instrunction resolved to: LD BC,0x%04x\n",imm_16);
                    #endif
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
                    #ifdef debug
                        printf("[DEBUG] instrunction resolved to: LD DE,0x%04x\n",imm_16);
                    #endif
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
                    #ifdef debug
                        printf("[DEBUG] instrunction resolved to: LD HL,0x%04x\n",imm_16);
                    #endif
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
                    #ifdef debug
                        printf("[DEBUG] instrunction resolved to: LD SP,0x%04x\n",imm_16);
                    #endif
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
                    #ifdef debug
                        printf("[DEBUG] instrunction resolved to: LD (0x%04x), SP\n",imm_16);
                    #endif
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
                    #ifdef debug
                        printf("[DEBUG] instrunction resolved to: LD SP,HL\n");
                    #endif
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
                    #ifdef debug
                        printf("[DEBUG] instrunction resolved to: PUSH BC\n");
                    #endif
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
                    #ifdef debug
                        printf("[DEBUG] instrunction resolved to: PUSH DE\n");
                    #endif
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
                    #ifdef debug
                        printf("[DEBUG] instrunction resolved to: PUSH HL\n");
                    #endif
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
                    #ifdef debug
                        printf("[DEBUG] instrunction resolved to: PUSH AF\n");
                    #endif
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
                    #ifdef debug
                        printf("[DEBUG] instrunction resolved to: POP BC\n");
                    #endif
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
                    #ifdef debug
                        printf("[DEBUG] instrunction resolved to: POP DE\n");
                    #endif
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
                    #ifdef debug
                        printf("[DEBUG] instrunction resolved to: POP HL\n");
                    #endif
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
                    #ifdef debug
                        printf("[DEBUG] instrunction resolved to: POP AF\n");
                    #endif
                    cpu.pop(A,F); 
                    state = FETCH_OP;
                    cpu.incPC();
                    break;
                default:
                    break;
            }
            break;
        case ADD_IMM:
            switch(state){
                case FETCH_OP:
                    state = EXECUTE_1;
                    cpu.incPC();
                    break;
                case EXECUTE_1:
                    imm_8 = mem[cpu.getPC()];
                    #ifdef debug
                        printf("[DEBUG] instrunction resolved to: ADD %u\n",imm_8);
                    #endif
                    cpu.addImm(imm_8);
                    state = FETCH_OP;
                    cpu.incPC();
                    break;
                default:
                    break;
            }
            break;
        case ADD_A:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: ADD A\n");
            #endif
            cpu.addReg(A);
            cpu.incPC();
            break;
        case ADD_B:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: ADD B\n");
            #endif
            cpu.addReg(B);
            cpu.incPC();
            break;
        case ADD_C:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: ADD C\n");
            #endif
            cpu.addReg(C);
            cpu.incPC();
            break;
        case ADD_D:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: ADD D\n");
            #endif
            cpu.addReg(D);
            cpu.incPC();
            break;
        case ADD_E:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: ADD E\n");
            #endif
            cpu.addReg(E);
            cpu.incPC();
            break;
        case ADD_H:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: ADD H\n");
            #endif
            cpu.addReg(H);
            cpu.incPC();
            break;
        case ADD_L:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: ADD L\n");
            #endif
            cpu.addReg(L);
            cpu.incPC();
            break;
        case ADD_IND:
            switch(state){
                case FETCH_OP:
                    state = EXECUTE_1;
                    break;
                case EXECUTE_1:
                    #ifdef debug
                        printf("[DEBUG] instrunction resolved to: ADD (HL)\n");
                    #endif
                    cpu.addIndirect();
                    state = FETCH_OP;
                    cpu.incPC();
                    break;
                default:
                    break;
            }
            break;
        case ADC_A:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: ADC A\n");
            #endif
            cpu.addRegCarry(A);
            cpu.incPC();
            break;
        case ADC_B:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: ADC B\n");
            #endif
            cpu.addRegCarry(B);
            cpu.incPC();
            break;
        case ADC_C:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: ADC C\n");
            #endif
            cpu.addRegCarry(C);
            cpu.incPC();
            break;
        case ADC_D:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: ADC D\n");
            #endif
            cpu.addRegCarry(D);
            cpu.incPC();
            break;
        case ADC_E:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: ADC E\n");
            #endif
            cpu.addRegCarry(E);
            cpu.incPC();
            break;
        case ADC_H:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: ADC H\n");
            #endif
            cpu.addRegCarry(H);
            cpu.incPC();
            break;
        case ADC_L:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: ADC L\n");
            #endif
            cpu.addRegCarry(L);
            cpu.incPC();
            break;
        case ADC_IND:
            switch(state){
                case FETCH_OP:
                    state = EXECUTE_1;
                    break;
                case EXECUTE_1:
                    #ifdef debug
                        printf("[DEBUG] instrunction resolved to: ADC (HL)\n");
                    #endif
                    cpu.addIndirectCarry();
                    state = FETCH_OP;
                    cpu.incPC();
                    break;
                default:
                    break;
            }
            break;
        case ADC_IMM:
            switch(state){
                case FETCH_OP:
                    state = EXECUTE_1;
                    cpu.incPC();
                    break;
                case EXECUTE_1:
                    imm_8 = mem[cpu.getPC()];
                    #ifdef debug
                        printf("[DEBUG] instrunction resolved to: ADC %u\n", imm_8);
                    #endif
                    cpu.addImmCarry(imm_8);
                    state = FETCH_OP;
                    cpu.incPC();
                    break;
                default:
                    break;
            }
            break;
        case SUB_A:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: SUB A\n");
            #endif
            cpu.subReg(A);
            cpu.incPC();
            break;
        case SUB_B:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: SUB B\n");
            #endif
            cpu.subReg(B);
            cpu.incPC();
            break;
        case SUB_C:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: SUB C\n");
            #endif
            cpu.subReg(C);
            cpu.incPC();
            break;
        case SUB_D:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: SUB D\n");
            #endif
            cpu.subReg(D);
            cpu.incPC();
            break;
        case SUB_E:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: SUB E\n");
            #endif
            cpu.subReg(E);
            cpu.incPC();
            break;
        case SUB_H:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: SUB H\n");
            #endif
            cpu.subReg(H);
            cpu.incPC();
            break;
        case SUB_L:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: SUB L\n");
            #endif
            cpu.subReg(L);
            cpu.incPC();
            break;
        case SUB_IND:
            switch(state){
                case FETCH_OP:
                    state = EXECUTE_1;
                    break;
                case EXECUTE_1:
                    #ifdef debug
                        printf("[DEBUG] instrunction resolved to: SUB (HL)\n");
                    #endif
                    cpu.subIndirect();
                    state = FETCH_OP;
                    cpu.incPC();
                    break;
                default:
                    break;
            }
            break;
        case SUB_IMM:
            switch(state){
                case FETCH_OP:
                    state = EXECUTE_1;
                    cpu.incPC();
                    break;
                case EXECUTE_1:
                    imm_8 = mem[cpu.getPC()];
                    #ifdef debug
                        printf("[DEBUG] instrunction resolved to: SUB %u\n",imm_8);
                    #endif
                    cpu.subImm(imm_8);
                    state = FETCH_OP;
                    cpu.incPC();
                    break;
                default:
                    break;
            }
            break;
        case SBC_A:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: SBC A\n");
            #endif
            cpu.subRegCarry(A);
            cpu.incPC();
            break;
        case SBC_B:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: SBC B\n");
            #endif
            cpu.subRegCarry(B);
            cpu.incPC();
            break;
        case SBC_C:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: SBC C\n");
            #endif
            cpu.subRegCarry(C);
            cpu.incPC();
            break;
        case SBC_D:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: SBC D\n");
            #endif
            cpu.subRegCarry(D);
            cpu.incPC();
            break;
        case SBC_E:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: SBC E\n");
            #endif
            cpu.subRegCarry(E);
            cpu.incPC();
            break;
        case SBC_H:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: SBC H\n");
            #endif
            cpu.subRegCarry(H);
            cpu.incPC();
            break;
        case SBC_L:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: SBC L\n");
            #endif
            cpu.subRegCarry(L);
            cpu.incPC();
            break;
        case SBC_IND:
            switch(state){
                case FETCH_OP:
                    state = EXECUTE_1;
                    break;
                case EXECUTE_1:
                    #ifdef debug
                        printf("[DEBUG] instrunction resolved to: SBC (HL)\n");
                    #endif
                    cpu.subIndirectCarry();
                    state = FETCH_OP;
                    cpu.incPC();
                    break;
                default:
                    break;
            }
            break;
        case SBC_IMM:
            switch(state){
                case FETCH_OP:
                    state = EXECUTE_1;
                    cpu.incPC();
                    break;
                case EXECUTE_1:
                    imm_8 = mem[cpu.getPC()];
                    #ifdef debug
                        printf("[DEBUG] instrunction resolved to: SBC %u\n",imm_8);
                    #endif
                    cpu.subImmCarry(imm_8);
                    state = FETCH_OP;
                    cpu.incPC();
                    break;
                default:
                    break;
            }
            break;
        case CP_A:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: CP A\n");
            #endif
            cpu.compareReg(A);
            cpu.incPC();
            break;
        case CP_B:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: CP B\n");
            #endif
            cpu.compareReg(B);
            cpu.incPC();
            break;
        case CP_C:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: CP C\n");
            #endif
            cpu.compareReg(C);
            cpu.incPC();
            break;
        case CP_D:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: CP D\n");
            #endif
            cpu.compareReg(D);
            cpu.incPC();
            break;
        case CP_E:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: CP E\n");
            #endif
            cpu.compareReg(E);
            cpu.incPC();
            break;
        case CP_H:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: CP H\n");
            #endif
            cpu.compareReg(H);
            cpu.incPC();
            break;
        case CP_L:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: CP L\n");
            #endif
            cpu.compareReg(L);
            cpu.incPC();
            break;
        case CP_IND:
            switch(state){
                case FETCH_OP:
                    state = EXECUTE_1;
                    break;
                case EXECUTE_1:
                    #ifdef debug
                        printf("[DEBUG] instrunction resolved to: CP (HL)\n");
                    #endif
                    cpu.compareIndirect();
                    state = FETCH_OP;
                    cpu.incPC();
                    break;
                default:
                    break;
            }
            break;
        case CP_IMM:
            switch(state){
                case FETCH_OP:
                    state = EXECUTE_1;
                    cpu.incPC();
                    break;
                case EXECUTE_1:
                    imm_8 = mem[cpu.getPC()];
                    #ifdef debug
                        printf("[DEBUG] instrunction resolved to: CP %u\n", imm_8);
                    #endif
                    cpu.compareImm(imm_8);
                    state = FETCH_OP;
                    cpu.incPC();
                    break;
                default:
                    break;
            }
            break;
        case INC_A:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: INC A\n");
            #endif
            cpu.incReg(A);
            cpu.incPC();
            break;
        case INC_B:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: INC B\n");
            #endif
            cpu.incReg(B);
            cpu.incPC();
            break;
        case INC_C:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: INC C\n");
            #endif
            cpu.incReg(C);
            cpu.incPC();
            break;
        case INC_D:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: INC D\n");
            #endif
            cpu.incReg(D);
            cpu.incPC();
            break;
        case INC_E:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: INC E\n");
            #endif
            cpu.incReg(E);
            cpu.incPC();
            break;
        case INC_H:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: INC H\n");
            #endif
            cpu.incReg(H);
            cpu.incPC();
            break;
        case INC_L:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: INC L\n");
            #endif
            cpu.incReg(L);
            cpu.incPC();
            break;
        case INC_IND:
            switch(state){
                case FETCH_OP:
                    state = EXECUTE_1;
                    break;
                case EXECUTE_1:
                    #ifdef debug
                        printf("[DEBUG] instrunction resolved to: INC (HL)\n");
                    #endif
                    cpu.incIndirect();
                    state = FETCH_OP;
                    cpu.incPC();
                    break;
                default:
                    break;
            }
            break;
        case DEC_A:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: DEC A\n");
            #endif
            cpu.decReg(A);
            cpu.incPC();
            break;
        case DEC_B:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: DEC B\n");
            #endif
            cpu.decReg(B);
            cpu.incPC();
            break;
        case DEC_C:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: DEC C\n");
            #endif
            cpu.decReg(C);
            cpu.incPC();
            break;
        case DEC_D:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: DEC D\n");
            #endif
            cpu.decReg(D);
            cpu.incPC();
            break;
        case DEC_E:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: DEC E\n");
            #endif
            cpu.decReg(E);
            cpu.incPC();
            break;
        case DEC_H:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: DEC H\n");
            #endif
            cpu.decReg(H);
            cpu.incPC();
            break;
        case DEC_L:
            #ifdef debug
               printf("[DEBUG] instrunction resolved to: DEC L\n");
            #endif
            cpu.decReg(L);
            cpu.incPC();
            break;
        case DEC_IND:
            switch(state){
                case FETCH_OP:
                    state = EXECUTE_1;
                    break;
                case EXECUTE_1:
                    #ifdef debug
                        printf("[DEBUG] instrunction resolved to: DEC (HL)\n");
                    #endif
                    cpu.decIndirect();
                    state = FETCH_OP;
                    cpu.incPC();
                    break;
                default:
                    break;
            }
            break;
        case AND_A:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: AND A\n");
            #endif
            cpu.andReg(A);
            cpu.incPC();
            break;
        case AND_B:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: AND B\n");
            #endif
            cpu.andReg(B);
            cpu.incPC();
            break;
        case AND_C:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: AND C\n");
            #endif
            cpu.andReg(C);
            cpu.incPC();
            break;
        case AND_D:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: AND D\n");
            #endif
            cpu.andReg(D);
            cpu.incPC();
            break;
        case AND_E:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: AND E\n");
            #endif
            cpu.andReg(E);
            cpu.incPC();
            break;
        case AND_H:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: AND H\n");
            #endif
            cpu.andReg(H);
            cpu.incPC();
            break;
        case AND_L:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: AND L\n");
            #endif
            cpu.andReg(L);
            cpu.incPC();
            break;
        case AND_IND:
            switch(state){
                case FETCH_OP:
                    state = EXECUTE_1;
                    break;
                case EXECUTE_1:
                    #ifdef debug
                        printf("[DEBUG] instrunction resolved to: AND (HL)\n");
                    #endif
                    cpu.andIndirect();
                    state = FETCH_OP;
                    cpu.incPC();
                    break;
                default:
                    break;
            }
            break;
        case AND_IMM:
            switch(state){
                case FETCH_OP:
                    state = EXECUTE_1;
                    cpu.incPC();
                    break;
                case EXECUTE_1:
                    imm_8 = mem[cpu.getPC()];
                    #ifdef debug
                        printf("[DEBUG] instrunction resolved to: AND %u\n",imm_8);
                    #endif
                    cpu.andImm(imm_8);
                    state = FETCH_OP;
                    cpu.incPC();
                    break;
                default:
                    break;
            }
            break;
        case OR_A:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: OR A\n");
            #endif
            cpu.orReg(A);
            cpu.incPC();
            break;
        case OR_B:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: OR B\n");
            #endif
            cpu.orReg(B);
            cpu.incPC();
            break;
        case OR_C:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: OR C\n");
            #endif
            cpu.orReg(C);
            cpu.incPC();
            break;
        case OR_D:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: OR D\n");
            #endif
            cpu.orReg(D);
            cpu.incPC();
            break;
        case OR_E:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: OR E\n");
            #endif
            cpu.orReg(E);
            cpu.incPC();
            break;
        case OR_H:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: OR H\n");
            #endif
            cpu.orReg(H);
            cpu.incPC();
            break;
        case OR_L:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: OR L\n");
            #endif
            cpu.orReg(L);
            cpu.incPC();
            break;
        case OR_IND:
            switch(state){
                case FETCH_OP:
                    state = EXECUTE_1;
                    break;
                case EXECUTE_1:
                    #ifdef debug
                        printf("[DEBUG] instrunction resolved to: OR (HL)\n");
                    #endif
                    cpu.orIndirect();
                    state = FETCH_OP;
                    cpu.incPC();
                    break;
                default:
                    break;
            }
            break;
        case OR_IMM:
            switch(state){
                case FETCH_OP:
                    state = EXECUTE_1;
                    cpu.incPC();
                    break;
                case EXECUTE_1:
                    imm_8 = mem[cpu.getPC()];
                    #ifdef debug
                        printf("[DEBUG] instrunction resolved to: OR %u\n",imm_8);
                    #endif
                    cpu.orImm(imm_8);
                    state = FETCH_OP;
                    cpu.incPC();
                    break;
                default:
                    break;
            }
            break;
        case XOR_A:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: XOR A\n");
            #endif
            cpu.xorReg(A);
            cpu.incPC();
            break;
        case XOR_B:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: XOR B\n");
            #endif
            cpu.xorReg(B);
            cpu.incPC();
            break;
        case XOR_C:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: XOR C\n");
            #endif
            cpu.xorReg(C);
            cpu.incPC();
            break;
        case XOR_D:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: XOR D\n");
            #endif
            cpu.xorReg(D);
            cpu.incPC();
            break;
        case XOR_E:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: XOR E\n");
            #endif
            cpu.xorReg(E);
            cpu.incPC();
            break;
        case XOR_H:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: XOR H\n");
            #endif
            cpu.xorReg(H);
            cpu.incPC();
            break;
        case XOR_L:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: XOR L\n");
            #endif
            cpu.xorReg(L);
            cpu.incPC();
            break;
        case XOR_IND:
            switch(state){
                case FETCH_OP:
                    state = EXECUTE_1;
                    break;
                case EXECUTE_1:
                    #ifdef debug
                        printf("[DEBUG] instrunction resolved to: XOR (HL)\n");
                    #endif
                    cpu.xorIndirect();
                    state = FETCH_OP;
                    cpu.incPC();
                    break;
                default:
                    break;
            }
            break;
        case XOR_IMM:
            switch(state){
                case FETCH_OP:
                    state = EXECUTE_1;
                    cpu.incPC();
                    break;
                case EXECUTE_1:
                    imm_8 = mem[cpu.getPC()];
                    #ifdef debug
                        printf("[DEBUG] instrunction resolved to: XOR %u\n",imm_8);
                    #endif
                    cpu.xorImm(imm_8);
                    state = FETCH_OP;
                    cpu.incPC();
                    break;
                default:
                    break;
            }
            break;
        case CCF:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: CCF\n");
            #endif
            cpu.compCarryFlag();
            cpu.incPC();
            break;
        case SCF:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: SCF\n");
            #endif
            cpu.setCarryFlag();
            cpu.incPC();
            break;
        case DAA:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: DAA\n");
            #endif
            cpu.decimalAdjustAcc();
            cpu.incPC();
            break;
        case CPL:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: CPL\n");
            #endif
            cpu.compAcc();
            cpu.incPC();
            break;
        case INC_BC:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: INC BC\n");
            #endif
            cpu.incBC();
            cpu.incPC();
            break;
        case INC_DE:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: INC DE\n");
            #endif
            cpu.incDE();
            cpu.incPC();
            break;
        case INC_HL:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: INC HL\n");
            #endif
            cpu.incHL();
            cpu.incPC();
            break;
        case INC_SP:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: INC SP\n");
            #endif
            cpu.incSP();
            cpu.incPC();
            break;
        case DEC_BC:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: DEC BC\n");
            #endif
            cpu.decBC();
            cpu.incPC();
            break;
        case DEC_DE:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: DEC DE\n");
            #endif
            cpu.decDE();
            cpu.incPC();
            break;
        case DEC_HL:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: DEC HL\n");
            #endif
            cpu.decHL();
            cpu.incPC();
            break;
        case DEC_SP:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: DEC SP\n");
            #endif
            cpu.decSP();
            cpu.incPC();
            break;
        case ADD_HL_BC:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: ADD HL,BC\n");
            #endif
            cpu.addHLRegPair(B,C);
            cpu.incPC();
            break;
        case ADD_HL_DE:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: ADD HL,DE\n");
            #endif
            cpu.addHLRegPair(D,E);
            cpu.incPC();
            break;
        case ADD_HL_HL:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: ADD HL,HL\n");
            #endif
            cpu.addHLRegPair(H,L);
            cpu.incPC();
            break;
        case ADD_HL_SP:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: ADD HL,SP\n");
            #endif
            cpu.addHLSP();
            cpu.incPC();
            break;
        case LD_HL_SP_OFFSET:
            switch(state){
                case FETCH_OP:
                    state = FETCH_1;
                    cpu.incPC();
                    break;
                case FETCH_1:
                    imm_8 = mem[cpu.getPC()];
                    #ifdef debug
                        printf("[DEBUG] instrunction resolved to: ADD HL,SP+%d\n",(int8_t)imm_8);
                    #endif
                    state = EXECUTE_1;
                    break;
                case EXECUTE_1:
                    cpu.loadHLSPOffset((int8_t)imm_8);
                    state = FETCH_OP; 
                    break;
                default:
                    break;
            }
            break;
        case ADD_SP_IMM:
            switch(state){
                case FETCH_OP:
                    state = EXECUTE_1;
                    cpu.incPC();
                    break;
                case EXECUTE_1:
                    imm_8 = mem[cpu.getPC()];
                    #ifdef debug
                        printf("[DEBUG] instrunction resolved to: ADD SP,%d\n",(int8_t)imm_8);
                    #endif
                    cpu.addSPImm((int8_t)imm_8);
                    cpu.incPC();
                    break;
                default:
                    break;
            }
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
                    #ifdef debug
                        printf("[DEBUG] instrunction resolved to: JP 0x%04x\n",imm_16);
                    #endif
                    cpu.jump(imm_16);
                    state = FETCH_OP;
                    break;
                default:
                    break;
            }
            break;
        case JP_HL:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: JP HL\n");
            #endif
            cpu.jumpHL();
            cpu.incPC();
            break;
        case JP_C:
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
                    imm_16 = 0x0000;
                    imm_16 |= msb;
                    imm_16 = imm_16 << 8;
                    imm_16 |= lsb;
                    #ifdef debug
                        printf("[DEBUG] instrunction resolved to: JP c,0x%04x\n",imm_16);
                    #endif
                    if(cpu.jumpCond(imm_16,CARRY)){
                        state = EXECUTE_1;
                    }
                    else{
                        state = FETCH_OP;
                        cpu.incPC();
                    }
                    break;
                case EXECUTE_1:
                    state = FETCH_OP;
                    break;
                default:
                    break;
            }
            break;
        case JP_NC:
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
                    imm_16 = 0x0000;
                    imm_16 |= msb;
                    imm_16 = imm_16 << 8;
                    imm_16 |= lsb;
                    #ifdef debug
                        printf("[DEBUG] instrunction resolved to: JP nc,0x%04x\n",imm_16);
                    #endif
                    if(cpu.jumpCond(imm_16,NO_CARRY)){
                        state = EXECUTE_1;
                    }
                    else{
                        state = FETCH_OP;
                        cpu.incPC();
                    }
                    break;
                case EXECUTE_1:
                    state = FETCH_OP;
                    break;
                default:
                    break;
            }
            break;
        case JP_Z:
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
                    imm_16 = 0x0000;
                    imm_16 |= msb;
                    imm_16 = imm_16 << 8;
                    imm_16 |= lsb;
                    #ifdef debug
                        printf("[DEBUG] instrunction resolved to: JP z,0x%04x\n",imm_16);
                    #endif
                    if(cpu.jumpCond(imm_16,ZERO)){
                        state = EXECUTE_1;
                    }
                    else{
                        state = FETCH_OP;
                        cpu.incPC();
                    }
                    break;
                case EXECUTE_1:
                    state = FETCH_OP;
                    break;
                default:
                    break;
            }
            break;
        case JP_NZ:
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
                    imm_16 = 0x0000;
                    imm_16 |= msb;
                    imm_16 = imm_16 << 8;
                    imm_16 |= lsb;
                    #ifdef debug
                        printf("[DEBUG] instrunction resolved to: JP nz,0x%04x\n",imm_16);
                    #endif
                    if(cpu.jumpCond(imm_16,NOT_ZERO)){
                        state = EXECUTE_1;
                    }
                    else{
                        state = FETCH_OP;
                        cpu.incPC();
                    }
                    break;
                case EXECUTE_1:
                    state = FETCH_OP;
                    break;
                default:
                    break;
            }
            break;
        case JR:
            switch(state){
                case FETCH_OP:
                    state = FETCH_1;
                    cpu.incPC(); 
                    break;
                case FETCH_1:
                    imm_8 = mem[cpu.getPC()];
                    #ifdef debug
                        printf("[DEBUG] instrunction resolved to: JR %d\n",imm_8);
                    #endif
                    state = EXECUTE_1;
                    break;
                case EXECUTE_1:
                    cpu.jumpRel((int8_t)imm_8);
                    state = FETCH_OP;
                    break;
                default:
                    break;
            }
            break;
        case JR_C:
            switch(state){
                case FETCH_OP:
                    state = EXECUTE_1;
                    cpu.incPC(); 
                    break;
                case EXECUTE_1:
                    imm_8 = mem[cpu.getPC()];
                    #ifdef debug
                        printf("[DEBUG] instrunction resolved to: JR c,%d\n",imm_8);
                    #endif
                    if(cpu.jumpRelCond((int8_t)imm_8, CARRY)){
                        state = EXECUTE_2;
                    }
                    else{
                        state = FETCH_OP;
                        cpu.incPC();
                    }
                    break;
                case EXECUTE_2:
                    state = FETCH_OP;
                    break;
                default:
                    break;
            }
            break;
        case JR_NC:
            switch(state){
                case FETCH_OP:
                    state = EXECUTE_1;
                    cpu.incPC(); 
                    break;
                case EXECUTE_1:
                    imm_8 = mem[cpu.getPC()];
                    #ifdef debug
                        printf("[DEBUG] instrunction resolved to: JR nc,%d\n",imm_8);
                    #endif
                    if(cpu.jumpRelCond((int8_t)imm_8, NO_CARRY)){
                        state = EXECUTE_2;
                    }
                    else{
                        state = FETCH_OP;
                        cpu.incPC();
                    }
                    break;
                case EXECUTE_2:
                    state = FETCH_OP;
                    break;
                default:
                    break;
            }
            break;
        case JR_Z:
            switch(state){
                case FETCH_OP:
                    state = EXECUTE_1;
                    cpu.incPC(); 
                    break;
                case EXECUTE_1:
                    imm_8 = mem[cpu.getPC()];
                    #ifdef debug
                        printf("[DEBUG] instrunction resolved to: JR z,%d\n",imm_8);
                    #endif
                    if(cpu.jumpRelCond((int8_t)imm_8, ZERO)){
                        state = EXECUTE_2;
                    }
                    else{
                        state = FETCH_OP;
                        cpu.incPC();
                    }
                    break;
                case EXECUTE_2:
                    state = FETCH_OP;
                    break;
                default:
                    break;
            }
            break;
        case JR_NZ:
            switch(state){
                case FETCH_OP:
                    state = EXECUTE_1;
                    cpu.incPC(); 
                    break;
                case EXECUTE_1:
                    imm_8 = mem[cpu.getPC()];
                    #ifdef debug
                        printf("[DEBUG] instrunction resolved to: JR nz,%d\n",imm_8);
                    #endif
                    if(cpu.jumpRelCond((int8_t)imm_8, NOT_ZERO)){
                        state = EXECUTE_2;
                    }
                    else{
                        state = FETCH_OP;
                        cpu.incPC();
                    }
                    break;
                case EXECUTE_2:
                    state = FETCH_OP;
                    break;
                default:
                    break;
            }
            break;
        case CALL:
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
                    #ifdef debug
                        printf("[DEBUG] instrunction resolved to: CALL 0x%04x\n",imm_16);
                    #endif
                    state = EXECUTE_2;
                    cpu.call(imm_16);
                    break;
                case EXECUTE_2:
                    state = EXECUTE_3;
                    break;
                case EXECUTE_3:
                    state = FETCH_OP;
                    break;
                default:
                    break;
            }
            break;
        case CALL_C:
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
                    imm_16 = 0x0000;
                    imm_16 |= msb;
                    imm_16 = imm_16 << 8;
                    imm_16 |= lsb;
                    #ifdef debug
                        printf("[DEBUG] instrunction resolved to: CALL c,0x%04x\n",imm_16);
                    #endif
                    if(cpu.callCond(imm_16, CARRY)){
                        state = EXECUTE_1;
                    }
                    else{
                        state = FETCH_OP;
                        cpu.incPC();
                    }
                    break;
                case EXECUTE_1:
                    state = EXECUTE_2;
                    break;
                case EXECUTE_2:
                    state = EXECUTE_3;
                    break;
                case EXECUTE_3:
                    state = FETCH_OP;
                    break;
                default:
                    break;
            }
            break;
        case CALL_NC:
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
                    imm_16 = 0x0000;
                    imm_16 |= msb;
                    imm_16 = imm_16 << 8;
                    imm_16 |= lsb;
                    #ifdef debug
                        printf("[DEBUG] instrunction resolved to: CALL nc,0x%04x\n",imm_16);
                    #endif
                    if(cpu.callCond(imm_16, NO_CARRY)){
                        state = EXECUTE_1;
                    }
                    else{
                        state = FETCH_OP;
                        cpu.incPC();
                    }
                    break;
                case EXECUTE_1:
                    state = EXECUTE_2;
                    break;
                case EXECUTE_2:
                    state = EXECUTE_3;
                    break;
                case EXECUTE_3:
                    state = FETCH_OP;
                    break;
                default:
                    break;
            }
            break;
        case CALL_Z:
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
                    imm_16 = 0x0000;
                    imm_16 |= msb;
                    imm_16 = imm_16 << 8;
                    imm_16 |= lsb;
                    #ifdef debug
                        printf("[DEBUG] instrunction resolved to: CALL z,0x%04x\n",imm_16);
                    #endif
                    if(cpu.callCond(imm_16, ZERO)){
                        state = EXECUTE_1;
                    }
                    else{
                        state = FETCH_OP;
                        cpu.incPC();
                    }
                    break;
                case EXECUTE_1:
                    state = EXECUTE_2;
                    break;
                case EXECUTE_2:
                    state = EXECUTE_3;
                    break;
                case EXECUTE_3:
                    state = FETCH_OP;
                    break;
                default:
                    break;
            }
            break;
        case CALL_NZ:
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
                    imm_16 = 0x0000;
                    imm_16 |= msb;
                    imm_16 = imm_16 << 8;
                    imm_16 |= lsb;
                    #ifdef debug
                        printf("[DEBUG] instrunction resolved to: CALL nz,0x%04x\n",imm_16);
                    #endif
                    if(cpu.callCond(imm_16, NOT_ZERO)){
                        state = EXECUTE_1;
                    }
                    else{
                        state = FETCH_OP;
                        cpu.incPC();
                    }
                    break;
                case EXECUTE_1:
                    state = EXECUTE_2;
                    break;
                case EXECUTE_2:
                    state = EXECUTE_3;
                    break;
                case EXECUTE_3:
                    state = FETCH_OP;
                    break;
                default:
                    break;
            }
            break;
        case RET:
            switch(state){
                case FETCH_OP:
                    state = EXECUTE_1;
                    break;
                case EXECUTE_1:
                    state = EXECUTE_2;
                    #ifdef debug
                        printf("[DEBUG] instrunction resolved to: RET\n");
                    #endif
                    cpu.ret();
                    break;
                case EXECUTE_2:
                    state = EXECUTE_3;
                    break;
                case EXECUTE_3:
                    state = FETCH_OP;
                    break;
                default:
                    break;
            }
            break;
        case RET_C:
            switch(state){
                case FETCH_OP:
                    state = EXECUTE_1;
                    break;
                case EXECUTE_1:
                    #ifdef debug
                        printf("[DEBUG] instrunction resolved to: RET c\n");
                    #endif
                    if(cpu.retCond(CARRY)){
                        state = EXECUTE_2;
                    }
                    else{
                        state = FETCH_OP;
                        cpu.incPC();
                    }
                    break;
                case EXECUTE_2:
                    state = EXECUTE_3;
                    break;
                case EXECUTE_3:
                    state = EXECUTE_4;
                    break;
                case EXECUTE_4:
                    state = FETCH_OP;
                    break;
                default:
                    break;
            }
            break;
        case RET_NC:
            switch(state){
                case FETCH_OP:
                    state = EXECUTE_1;
                    break;
                case EXECUTE_1:
                    #ifdef debug
                        printf("[DEBUG] instrunction resolved to: RET nc\n");
                    #endif
                    if(cpu.retCond(NO_CARRY)){
                        state = EXECUTE_2;
                    }
                    else{
                        state = FETCH_OP;
                        cpu.incPC();
                    }
                    break;
                case EXECUTE_2:
                    state = EXECUTE_3;
                    break;
                case EXECUTE_3:
                    state = EXECUTE_4;
                    break;
                case EXECUTE_4:
                    state = FETCH_OP;
                    break;
                default:
                    break;
            }
            break;
        case RET_Z:
            switch(state){
                case FETCH_OP:
                    state = EXECUTE_1;
                    break;
                case EXECUTE_1:
                    #ifdef debug
                        printf("[DEBUG] instrunction resolved to: RET z\n");
                    #endif
                    if(cpu.retCond(ZERO)){
                        state = EXECUTE_2;
                    }
                    else{
                        state = FETCH_OP;
                        cpu.incPC();
                    }
                    break;
                case EXECUTE_2:
                    state = EXECUTE_3;
                    break;
                case EXECUTE_3:
                    state = EXECUTE_4;
                    break;
                case EXECUTE_4:
                    state = FETCH_OP;
                    break;
                default:
                    break;
            }
            break;
        case RET_NZ:
            switch(state){
                case FETCH_OP:
                    state = EXECUTE_1;
                    break;
                case EXECUTE_1:
                    #ifdef debug
                        printf("[DEBUG] instrunction resolved to: RET nz\n");
                    #endif
                    if(cpu.retCond(NOT_ZERO)){
                        state = EXECUTE_2;
                    }
                    else{
                        state = FETCH_OP;
                        cpu.incPC();
                    }
                    break;
                case EXECUTE_2:
                    state = EXECUTE_3;
                    break;
                case EXECUTE_3:
                    state = EXECUTE_4;
                    break;
                case EXECUTE_4:
                    state = FETCH_OP;
                    break;
                default:
                    break;
            }
            break;
        case RETI:
            switch(state){
                case FETCH_OP:
                    state = EXECUTE_1;
                    break;
                case EXECUTE_1:
                    state = EXECUTE_2;
                    #ifdef debug
                        printf("[DEBUG] instrunction resolved to: RETI\n");
                    #endif
                    cpu.reti();
                    break;
                case EXECUTE_2:
                    state = EXECUTE_3;
                    break;
                case EXECUTE_3:
                    state = FETCH_OP;
                    break;
                default:
                    break;
            }
            break;
        case RST_0x00:
            switch(state){
                case FETCH_OP:
                    state = EXECUTE_1;
                    break;
                case EXECUTE_1:
                    state = EXECUTE_2;
                    #ifdef debug
                        printf("[DEBUG] instrunction resolved to: RST 0x00\n");
                    #endif
                    cpu.rst(0x00);
                    break;
                case EXECUTE_2:
                    state = EXECUTE_3;
                    break;
                case EXECUTE_3:
                    state = FETCH_OP;
                    break;
                default:
                    break;
            }
            break;
        case RST_0x08:
            switch(state){
                case FETCH_OP:
                    state = EXECUTE_1;
                    break;
                case EXECUTE_1:
                    state = EXECUTE_2;
                    #ifdef debug
                        printf("[DEBUG] instrunction resolved to: RST 0x08\n");
                    #endif
                    cpu.rst(0x08);
                    break;
                case EXECUTE_2:
                    state = EXECUTE_3;
                    break;
                case EXECUTE_3:
                    state = FETCH_OP;
                    break;
                default:
                    break;
            }
            break;
        case RST_0x10:
            switch(state){
                case FETCH_OP:
                    state = EXECUTE_1;
                    break;
                case EXECUTE_1:
                    state = EXECUTE_2;
                    #ifdef debug
                        printf("[DEBUG] instrunction resolved to: RST 0x10\n");
                    #endif
                    cpu.rst(0x10);
                    break;
                case EXECUTE_2:
                    state = EXECUTE_3;
                    break;
                case EXECUTE_3:
                    state = FETCH_OP;
                    break;
                default:
                    break;
            }
            break;
        case RST_0x18:
            switch(state){
                case FETCH_OP:
                    state = EXECUTE_1;
                    break;
                case EXECUTE_1:
                    state = EXECUTE_2;
                    #ifdef debug
                        printf("[DEBUG] instrunction resolved to: RST 0x18\n");
                    #endif
                    cpu.rst(0x18);
                    break;
                case EXECUTE_2:
                    state = EXECUTE_3;
                    break;
                case EXECUTE_3:
                    state = FETCH_OP;
                    break;
                default:
                    break;
            }
            break;
        case RST_0x20:
            switch(state){
                case FETCH_OP:
                    state = EXECUTE_1;
                    break;
                case EXECUTE_1:
                    state = EXECUTE_2;
                    #ifdef debug
                        printf("[DEBUG] instrunction resolved to: RST 0x20\n");
                    #endif
                    cpu.rst(0x20);
                    break;
                case EXECUTE_2:
                    state = EXECUTE_3;
                    break;
                case EXECUTE_3:
                    state = FETCH_OP;
                    break;
                default:
                    break;
            }
            break;
        case RST_0x28:
            switch(state){
                case FETCH_OP:
                    state = EXECUTE_1;
                    break;
                case EXECUTE_1:
                    state = EXECUTE_2;
                    #ifdef debug
                        printf("[DEBUG] instrunction resolved to: RST 0x28\n");
                    #endif
                    cpu.rst(0x28);
                    break;
                case EXECUTE_2:
                    state = EXECUTE_3;
                    break;
                case EXECUTE_3:
                    state = FETCH_OP;
                    break;
                default:
                    break;
            }
            break;
        case RST_0x30:
            switch(state){
                case FETCH_OP:
                    state = EXECUTE_1;
                    break;
                case EXECUTE_1:
                    state = EXECUTE_2;
                    #ifdef debug
                        printf("[DEBUG] instrunction resolved to: RST 0x30\n");
                    #endif
                    cpu.rst(0x30);
                    break;
                case EXECUTE_2:
                    state = EXECUTE_3;
                    break;
                case EXECUTE_3:
                    state = FETCH_OP;
                    break;
                default:
                    break;
            }
            break;
        case RST_0x38:
            switch(state){
                case FETCH_OP:
                    state = EXECUTE_1;
                    break;
                case EXECUTE_1:
                    state = EXECUTE_2;
                    #ifdef debug
                        printf("[DEBUG] instrunction resolved to: RST 0x38\n");
                    #endif
                    cpu.rst(0x38);
                    break;
                case EXECUTE_2:
                    state = EXECUTE_3;
                    break;
                case EXECUTE_3:
                    state = FETCH_OP;
                    break;
                default:
                    break;
            }
            break;
        case RRC_A:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: RRC A\n");
            #endif
            cpu.rrc(A);
            break;
        case RR_A:
            #ifdef debug
                printf("[DEBUG] instrunction resolved to: RR A\n");
            #endif
            cpu.rr(A);
            break;
        case CB_OP:
            switch(state){
                case FETCH_OP:
                    #ifdef debug
                        printf("[DEBUG] CB Prefix Detected. Fetching Opcode...\n");
                    #endif
                    state = EXECUTE_1; 
                    cpu.incPC();
                    break;
                case EXECUTE_1:
                    cb_op = mem[cpu.getPC()];
                    switch(cb_op){
                        case RRC_A:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: RRC A\n");
                            #endif
                            cpu.rrc(A);
                            break;
                        case RRC_B:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: RRC B\n");
                            #endif
                            cpu.rrc(B);
                            break;
                        case RRC_C:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: RRC C\n");
                            #endif
                            cpu.rrc(C);
                            break;
                        case RRC_D:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: RRC D\n");
                            #endif
                            cpu.rrc(D);
                            break;
                        case RRC_E:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: RRC E\n");
                            #endif
                            cpu.rrc(E);
                            break;
                        case RRC_H:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: RRC H\n");
                            #endif
                            cpu.rrc(H);
                            break;
                        case RRC_L:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: RRC L\n");
                            #endif
                            cpu.rrc(L);
                            break;
                        case RRC_IND:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: RRC (HL)\n");
                            #endif
                            cpu.rrcInd();
                            break;
                        case RR_A:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: RR A\n");
                            #endif
                            cpu.rr(A);
                            break;
                        case RR_B:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: RR B\n");
                            #endif
                            cpu.rr(B);
                            break;
                        case RR_C:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: RR C\n");
                            #endif
                            cpu.rr(C);
                            break;
                        case RR_D:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: RR D\n");
                            #endif
                            cpu.rr(D);
                            break;
                        case RR_E:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: RR E\n");
                            #endif
                            cpu.rr(E);
                            break;
                        case RR_H:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: RR H\n");
                            #endif
                            cpu.rr(H);
                            break;
                        case RR_L:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: RR L\n");
                            #endif
                            cpu.rr(L);
                            break;
                        case RR_IND:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: RR (HL)\n");
                            #endif
                            cpu.rrInd();
                            break;
                        case RLC_A:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: RLC A\n");
                            #endif
                            cpu.rlc(A);
                            break;
                        case RLC_B:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: RLC B\n");
                            #endif
                            cpu.rlc(B);
                            break;
                        case RLC_C:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: RLC C\n");
                            #endif
                            cpu.rlc(C);
                            break;
                        case RLC_D:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: RLC D\n");
                            #endif
                            cpu.rlc(D);
                            break;
                        case RLC_E:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: RLC E\n");
                            #endif
                            cpu.rlc(E);
                            break;
                        case RLC_H:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: RLC H\n");
                            #endif
                            cpu.rlc(H);
                            break;
                        case RLC_L:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: RLC L\n");
                            #endif
                            cpu.rlc(L);
                            break;
                        case RLC_IND:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: RLC (HL)\n");
                            #endif
                            cpu.rlcInd();
                            break;
                        case RL_A:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: RL A\n");
                            #endif
                            cpu.rl(A);
                            break;
                        case RL_B:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: RL B\n");
                            #endif
                            cpu.rl(B);
                            break;
                        case RL_C:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: RL C\n");
                            #endif
                            cpu.rl(C);
                            break;
                        case RL_D:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: RL D\n");
                            #endif
                            cpu.rl(D);
                            break;
                        case RL_E:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: RL E\n");
                            #endif
                            cpu.rl(E);
                            break;
                        case RL_H:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: RL H\n");
                            #endif
                            cpu.rl(H);
                            break;
                        case RL_L:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: RL L\n");
                            #endif
                            cpu.rl(L);
                            break;
                        case RL_IND:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: RL (HL)\n");
                            #endif
                            cpu.rlInd();
                            break;
                        /*
                        case SLA_A:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: SLA A\n");
                            #endif
                            cpu.sla(A);
                            break;
                        case SLA_B:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: SLA B\n");
                            #endif
                            cpu.sla(B);
                            break;
                        case SLA_C:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: SLA C\n");
                            #endif
                            cpu.sla(C);
                            break;
                        case SLA_D:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: SLA D\n");
                            #endif
                            cpu.sla(D);
                            break;
                        case SLA_E:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: SLA E\n");
                            #endif
                            cpu.sla(E);
                            break;
                        case SLA_H:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: SLA H\n");
                            #endif
                            cpu.sla(H);
                            break;
                        case SLA_L:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: SLA L\n");
                            #endif
                            cpu.sla(L);
                            break;
                        case SLA_IND:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: SLA (HL)\n");
                            #endif
                            cpu.slaInd();
                            break;
                        case SRA_A:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: SRA A\n");
                            #endif
                            cpu.sra(A);
                            break;
                        case SRA_B:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: SRA B\n");
                            #endif
                            cpu.sra(B);
                            break;
                        case SRA_C:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: SRA C\n");
                            #endif
                            cpu.sra(C);
                            break;
                        case SRA_D:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: SRA D\n");
                            #endif
                            cpu.sra(D);
                            break;
                        case SRA_E:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: SRA E\n");
                            #endif
                            cpu.sra(E);
                            break;
                        case SRA_H:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: SRA H\n");
                            #endif
                            cpu.sra(H);
                            break;
                        case SRA_L:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: SRA L\n");
                            #endif
                            cpu.sra(L);
                            break;
                        case SRA_IND:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: SRA (HL)\n");
                            #endif
                            cpu.sraInd();
                            break;
                        case SRL_A:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: SRL A\n");
                            #endif
                            cpu.srl(A);
                            break;
                        case SRL_B:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: SRL B\n");
                            #endif
                            cpu.srl(B);
                            break;
                        case SRL_C:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: SRL C\n");
                            #endif
                            cpu.srl(C);
                            break;
                        case SRL_D:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: SRL D\n");
                            #endif
                            cpu.srl(D);
                            break;
                        case SRL_E:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: SRL E\n");
                            #endif
                            cpu.srl(E);
                            break;
                        case SRL_H:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: SRL H\n");
                            #endif
                            cpu.srl(H);
                            break;
                        case SRL_L:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: SRL L\n");
                            #endif
                            cpu.srl(L);
                            break;
                        case SRL_IND:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: SRL (HL)\n");
                            #endif
                            cpu.srlInd();
                            break;
                        case SWAP_A:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: SWAP A\n");
                            #endif
                            cpu.swap(A);
                            break;
                        case SWAP_B:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: SWAP B\n");
                            #endif
                            cpu.swap(B);
                            break;
                        case SWAP_C:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: SWAP C\n");
                            #endif
                            cpu.swap(C);
                            break;
                        case SWAP_D:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: SWAP D\n");
                            #endif
                            cpu.swap(D);
                            break;
                        case SWAP_E:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: SWAP E\n");
                            #endif
                            cpu.swap(E);
                            break;
                        case SWAP_H:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: SWAP H\n");
                            #endif
                            cpu.swap(H);
                            break;
                        case SWAP_L:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: SWAP L\n");
                            #endif
                            cpu.swap(L);
                            break;
                        case SWAP_IND:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: SWAP (HL)\n");
                            #endif
                            cpu.swapInd();
                            break;
                        case BIT_0_A:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: BIT 0,A\n");
                            #endif
                            cpu.bit(A,BIT_ZERO);
                            break;
                        case BIT_1_A:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: BIT 1,A\n");
                            #endif
                            cpu.bit(A,BIT_ONE);
                            break;
                        case BIT_2_A:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: BIT 2,A\n");
                            #endif
                            cpu.bit(A,BIT_TWO);
                            break;
                        case BIT_3_A:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: BIT 3,A\n");
                            #endif
                            cpu.bit(A,BIT_THREE);
                            break;
                        case BIT_4_A:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: BIT 4,A\n");
                            #endif
                            cpu.bit(A,BIT_FOUR);
                            break;
                        case BIT_5_A:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: BIT 5,A\n");
                            #endif
                            cpu.bit(A,BIT_FIVE);
                            break;
                        case BIT_6_A:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: BIT 6,A\n");
                            #endif
                            cpu.bit(A,BIT_SIX);
                            break;
                        case BIT_7_A:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: BIT 7,A\n");
                            #endif
                            cpu.bit(A,BIT_SEVEN);
                            break;
                        case BIT_0_B:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: BIT 0,B\n");
                            #endif
                            cpu.bit(B,BIT_ZERO);
                            break;
                        case BIT_1_B:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: BIT 1,B\n");
                            #endif
                            cpu.bit(B,BIT_ONE);
                            break;
                        case BIT_2_B:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: BIT 2,B\n");
                            #endif
                            cpu.bit(B,BIT_TWO);
                            break;
                        case BIT_3_B:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: BIT 3,B\n");
                            #endif
                            cpu.bit(B,BIT_THREE);
                            break;
                        case BIT_4_B:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: BIT 4,B\n");
                            #endif
                            cpu.bit(B,BIT_FOUR);
                            break;
                        case BIT_5_B:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: BIT 5,B\n");
                            #endif
                            cpu.bit(B,BIT_FIVE);
                            break;
                        case BIT_6_B:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: BIT 6,B\n");
                            #endif
                            cpu.bit(B,BIT_SIX);
                            break;
                        case BIT_7_B:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: BIT 7,B\n");
                            #endif
                            cpu.bit(B,BIT_SEVEN);
                            break;
                        case BIT_0_C:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: BIT 0,C\n");
                            #endif
                            cpu.bit(C,BIT_ZERO);
                            break;
                        case BIT_1_C:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: BIT 1,C\n");
                            #endif
                            cpu.bit(C,BIT_ONE);
                            break;
                        case BIT_2_C:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: BIT 2,C\n");
                            #endif
                            cpu.bit(C,BIT_TWO);
                            break;
                        case BIT_3_C:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: BIT 3,C\n");
                            #endif
                            cpu.bit(C,BIT_THREE);
                            break;
                        case BIT_4_C:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: BIT 4,C\n");
                            #endif
                            cpu.bit(C,BIT_FOUR);
                            break;
                        case BIT_5_C:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: BIT 5,C\n");
                            #endif
                            cpu.bit(C,BIT_FIVE);
                            break;
                        case BIT_6_C:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: BIT 6,C\n");
                            #endif
                            cpu.bit(C,BIT_SIX);
                            break;
                        case BIT_7_C:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: BIT 7,C\n");
                            #endif
                            cpu.bit(C,BIT_SEVEN);
                            break;
                        case BIT_0_D:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: BIT 0,D\n");
                            #endif
                            cpu.bit(D,BIT_ZERO);
                            break;
                        case BIT_1_D:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: BIT 1,D\n");
                            #endif
                            cpu.bit(D,BIT_ONE);
                            break;
                        case BIT_2_D:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: BIT 2,D\n");
                            #endif
                            cpu.bit(D,BIT_TWO);
                            break;
                        case BIT_3_D:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: BIT 3,D\n");
                            #endif
                            cpu.bit(D,BIT_THREE);
                            break;
                        case BIT_4_D:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: BIT 4,D\n");
                            #endif
                            cpu.bit(D,BIT_FOUR);
                            break;
                        case BIT_5_D:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: BIT 5,D\n");
                            #endif
                            cpu.bit(D,BIT_FIVE);
                            break;
                        case BIT_6_D:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: BIT 6,D\n");
                            #endif
                            cpu.bit(D,BIT_SIX);
                            break;
                        case BIT_7_D:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: BIT 7,D\n");
                            #endif
                            cpu.bit(D,BIT_SEVEN);
                            break;
                        case BIT_0_E:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: BIT 0,E\n");
                            #endif
                            cpu.bit(E,BIT_ZERO);
                            break;
                        case BIT_1_E:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: BIT 1,E\n");
                            #endif
                            cpu.bit(E,BIT_ONE);
                            break;
                        case BIT_2_E:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: BIT 2,E\n");
                            #endif
                            cpu.bit(E,BIT_TWO);
                            break;
                        case BIT_3_E:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: BIT 3,E\n");
                            #endif
                            cpu.bit(E,BIT_THREE);
                            break;
                        case BIT_4_E:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: BIT 4,E\n");
                            #endif
                            cpu.bit(E,BIT_FOUR);
                            break;
                        case BIT_5_E:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: BIT 5,E\n");
                            #endif
                            cpu.bit(E,BIT_FIVE);
                            break;
                        case BIT_6_E:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: BIT 6,E\n");
                            #endif
                            cpu.bit(E,BIT_SIX);
                            break;
                        case BIT_7_E:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: BIT 7,E\n");
                            #endif
                            cpu.bit(E,BIT_SEVEN);
                            break;
                        case BIT_0_H:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: BIT 0,H\n");
                            #endif
                            cpu.bit(H,BIT_ZERO);
                            break;
                        case BIT_1_H:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: BIT 1,H\n");
                            #endif
                            cpu.bit(H,BIT_ONE);
                            break;
                        case BIT_2_H:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: BIT 2,H\n");
                            #endif
                            cpu.bit(H,BIT_TWO);
                            break;
                        case BIT_3_H:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: BIT 3,H\n");
                            #endif
                            cpu.bit(H,BIT_THREE);
                            break;
                        case BIT_4_H:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: BIT 4,H\n");
                            #endif
                            cpu.bit(H,BIT_FOUR);
                            break;
                        case BIT_5_H:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: BIT 5,H\n");
                            #endif
                            cpu.bit(H,BIT_FIVE);
                            break;
                        case BIT_6_H:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: BIT 6,H\n");
                            #endif
                            cpu.bit(H,BIT_SIX);
                            break;
                        case BIT_7_H:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: BIT 7,H\n");
                            #endif
                            cpu.bit(H,BIT_SEVEN);
                            break;
                        case BIT_0_L:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: BIT 0,L\n");
                            #endif
                            cpu.bit(L,BIT_ZERO);
                            break;
                        case BIT_1_L:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: BIT 1,L\n");
                            #endif
                            cpu.bit(L,BIT_ONE);
                            break;
                        case BIT_2_L:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: BIT 2,L\n");
                            #endif
                            cpu.bit(L,BIT_TWO);
                            break;
                        case BIT_3_L:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: BIT 3,L\n");
                            #endif
                            cpu.bit(L,BIT_THREE);
                            break;
                        case BIT_4_L:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: BIT 4,L\n");
                            #endif
                            cpu.bit(L,BIT_FOUR);
                            break;
                        case BIT_5_L:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: BIT 5,L\n");
                            #endif
                            cpu.bit(L,BIT_FIVE);
                            break;
                        case BIT_6_L:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: BIT 6,L\n");
                            #endif
                            cpu.bit(L,BIT_SIX);
                            break;
                        case BIT_7_L:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: BIT 7,L\n");
                            #endif
                            cpu.bit(L,BIT_SEVEN);
                            break;
                        case BIT_0_IND:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: BIT 0,IND\n");
                            #endif
                            cpu.bitInd(BIT_ZERO);
                            break;
                        case BIT_1_IND:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: BIT 1,IND\n");
                            #endif
                            cpu.bitInd(BIT_ONE);
                            break;
                        case BIT_2_IND:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: BIT 2,IND\n");
                            #endif
                            cpu.bitInd(BIT_TWO);
                            break;
                        case BIT_3_IND:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: BIT 3,IND\n");
                            #endif
                            cpu.bitInd(BIT_THREE);
                            break;
                        case BIT_4_IND:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: BIT 4,IND\n");
                            #endif
                            cpu.bitInd(BIT_FOUR);
                            break;
                        case BIT_5_IND:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: BIT 5,IND\n");
                            #endif
                            cpu.bitInd(BIT_FIVE);
                            break;
                        case BIT_6_IND:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: BIT 6,IND\n");
                            #endif
                            cpu.bitInd(BIT_SIX);
                            break;
                        case BIT_7_IND:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: BIT 7,IND\n");
                            #endif
                            cpu.bitInd(BIT_SEVEN);
                            break;
                        case RES_0_A:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: RES 0,A\n");
                            #endif
                            cpu.res(A,BIT_ZERO);
                            break;
                        case RES_1_A:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: RES 1,A\n");
                            #endif
                            cpu.res(A,BIT_ONE);
                            break;
                        case RES_2_A:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: RES 2,A\n");
                            #endif
                            cpu.res(A,BIT_TWO);
                            break;
                        case RES_3_A:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: RES 3,A\n");
                            #endif
                            cpu.res(A,BIT_THREE);
                            break;
                        case RES_4_A:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: RES 4,A\n");
                            #endif
                            cpu.res(A,BIT_FOUR);
                            break;
                        case RES_5_A:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: RES 5,A\n");
                            #endif
                            cpu.res(A,BIT_FIVE);
                            break;
                        case RES_6_A:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: RES 6,A\n");
                            #endif
                            cpu.res(A,BIT_SIX);
                            break;
                        case RES_7_A:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: RES 7,A\n");
                            #endif
                            cpu.res(A,BIT_SEVEN);
                            break;
                        case RES_0_B:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: RES 0,B\n");
                            #endif
                            cpu.res(B,BIT_ZERO);
                            break;
                        case RES_1_B:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: RES 1,B\n");
                            #endif
                            cpu.res(B,BIT_ONE);
                            break;
                        case RES_2_B:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: RES 2,B\n");
                            #endif
                            cpu.res(B,BIT_TWO);
                            break;
                        case RES_3_B:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: RES 3,B\n");
                            #endif
                            cpu.res(B,BIT_THREE);
                            break;
                        case RES_4_B:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: RES 4,B\n");
                            #endif
                            cpu.res(B,BIT_FOUR);
                            break;
                        case RES_5_B:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: RES 5,B\n");
                            #endif
                            cpu.res(B,BIT_FIVE);
                            break;
                        case RES_6_B:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: RES 6,B\n");
                            #endif
                            cpu.res(B,BIT_SIX);
                            break;
                        case RES_7_B:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: RES 7,B\n");
                            #endif
                            cpu.res(B,BIT_SEVEN);
                            break;
                        case RES_0_C:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: RES 0,C\n");
                            #endif
                            cpu.res(C,BIT_ZERO);
                            break;
                        case RES_1_C:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: RES 1,C\n");
                            #endif
                            cpu.res(C,BIT_ONE);
                            break;
                        case RES_2_C:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: RES 2,C\n");
                            #endif
                            cpu.res(C,BIT_TWO);
                            break;
                        case RES_3_C:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: RES 3,C\n");
                            #endif
                            cpu.res(C,BIT_THREE);
                            break;
                        case RES_4_C:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: RES 4,C\n");
                            #endif
                            cpu.res(C,BIT_FOUR);
                            break;
                        case RES_5_C:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: RES 5,C\n");
                            #endif
                            cpu.res(C,BIT_FIVE);
                            break;
                        case RES_6_C:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: RES 6,C\n");
                            #endif
                            cpu.res(C,BIT_SIX);
                            break;
                        case RES_7_C:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: RES 7,C\n");
                            #endif
                            cpu.res(C,BIT_SEVEN);
                            break;
                        case RES_0_D:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: RES 0,D\n");
                            #endif
                            cpu.res(D,BIT_ZERO);
                            break;
                        case RES_1_D:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: RES 1,D\n");
                            #endif
                            cpu.res(D,BIT_ONE);
                            break;
                        case RES_2_D:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: RES 2,D\n");
                            #endif
                            cpu.res(D,BIT_TWO);
                            break;
                        case RES_3_D:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: RES 3,D\n");
                            #endif
                            cpu.res(D,BIT_THREE);
                            break;
                        case RES_4_D:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: RES 4,D\n");
                            #endif
                            cpu.res(D,BIT_FOUR);
                            break;
                        case RES_5_D:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: RES 5,D\n");
                            #endif
                            cpu.res(D,BIT_FIVE);
                            break;
                        case RES_6_D:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: RES 6,D\n");
                            #endif
                            cpu.res(D,BIT_SIX);
                            break;
                        case RES_7_D:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: RES 7,D\n");
                            #endif
                            cpu.res(D,BIT_SEVEN);
                            break;
                        case RES_0_E:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: RES 0,E\n");
                            #endif
                            cpu.res(E,BIT_ZERO);
                            break;
                        case RES_1_E:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: RES 1,E\n");
                            #endif
                            cpu.res(E,BIT_ONE);
                            break;
                        case RES_2_E:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: RES 2,E\n");
                            #endif
                            cpu.res(E,BIT_TWO);
                            break;
                        case RES_3_E:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: RES 3,E\n");
                            #endif
                            cpu.res(E,BIT_THREE);
                            break;
                        case RES_4_E:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: RES 4,E\n");
                            #endif
                            cpu.res(E,BIT_FOUR);
                            break;
                        case RES_5_E:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: RES 5,E\n");
                            #endif
                            cpu.res(E,BIT_FIVE);
                            break;
                        case RES_6_E:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: RES 6,E\n");
                            #endif
                            cpu.res(E,BIT_SIX);
                            break;
                        case RES_7_E:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: RES 7,E\n");
                            #endif
                            cpu.res(E,BIT_SEVEN);
                            break;
                        case RES_0_H:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: RES 0,H\n");
                            #endif
                            cpu.res(H,BIT_ZERO);
                            break;
                        case RES_1_H:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: RES 1,H\n");
                            #endif
                            cpu.res(H,BIT_ONE);
                            break;
                        case RES_2_H:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: RES 2,H\n");
                            #endif
                            cpu.res(H,BIT_TWO);
                            break;
                        case RES_3_H:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: RES 3,H\n");
                            #endif
                            cpu.res(H,BIT_THREE);
                            break;
                        case RES_4_H:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: RES 4,H\n");
                            #endif
                            cpu.res(H,BIT_FOUR);
                            break;
                        case RES_5_H:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: RES 5,H\n");
                            #endif
                            cpu.res(H,BIT_FIVE);
                            break;
                        case RES_6_H:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: RES 6,H\n");
                            #endif
                            cpu.res(H,BIT_SIX);
                            break;
                        case RES_7_H:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: RES 7,H\n");
                            #endif
                            cpu.res(H,BIT_SEVEN);
                            break;
                        case RES_0_L:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: RES 0,L\n");
                            #endif
                            cpu.res(L,BIT_ZERO);
                            break;
                        case RES_1_L:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: RES 1,L\n");
                            #endif
                            cpu.res(L,BIT_ONE);
                            break;
                        case RES_2_L:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: RES 2,L\n");
                            #endif
                            cpu.res(L,BIT_TWO);
                            break;
                        case RES_3_L:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: RES 3,L\n");
                            #endif
                            cpu.res(L,BIT_THREE);
                            break;
                        case RES_4_L:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: RES 4,L\n");
                            #endif
                            cpu.res(L,BIT_FOUR);
                            break;
                        case RES_5_L:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: RES 5,L\n");
                            #endif
                            cpu.res(L,BIT_FIVE);
                            break;
                        case RES_6_L:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: RES 6,L\n");
                            #endif
                            cpu.res(L,BIT_SIX);
                            break;
                        case RES_7_L:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: RES 7,L\n");
                            #endif
                            cpu.res(L,BIT_SEVEN);
                            break;
                        case RES_0_IND:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: RES 0,IND\n");
                            #endif
                            cpu.resInd(BIT_ZERO);
                            break;
                        case RES_1_IND:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: RES 1,IND\n");
                            #endif
                            cpu.resInd(BIT_ONE);
                            break;
                        case RES_2_IND:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: RES 2,IND\n");
                            #endif
                            cpu.resInd(BIT_TWO);
                            break;
                        case RES_3_IND:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: RES 3,IND\n");
                            #endif
                            cpu.resInd(BIT_THREE);
                            break;
                        case RES_4_IND:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: RES 4,IND\n");
                            #endif
                            cpu.resInd(BIT_FOUR);
                            break;
                        case RES_5_IND:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: RES 5,IND\n");
                            #endif
                            cpu.resInd(BIT_FIVE);
                            break;
                        case RES_6_IND:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: RES 6,IND\n");
                            #endif
                            cpu.resInd(BIT_SIX);
                            break;
                        case RES_7_IND:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: RES 7,IND\n");
                            #endif
                            cpu.resInd(BIT_SEVEN);
                            break;
                        case SET_0_A:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: SET 0,A\n");
                            #endif
                            cpu.set(A,BIT_ZERO);
                            break;
                        case SET_1_A:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: SET 1,A\n");
                            #endif
                            cpu.set(A,BIT_ONE);
                            break;
                        case SET_2_A:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: SET 2,A\n");
                            #endif
                            cpu.set(A,BIT_TWO);
                            break;
                        case SET_3_A:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: SET 3,A\n");
                            #endif
                            cpu.set(A,BIT_THREE);
                            break;
                        case SET_4_A:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: SET 4,A\n");
                            #endif
                            cpu.set(A,BIT_FOUR);
                            break;
                        case SET_5_A:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: SET 5,A\n");
                            #endif
                            cpu.set(A,BIT_FIVE);
                            break;
                        case SET_6_A:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: SET 6,A\n");
                            #endif
                            cpu.set(A,BIT_SIX);
                            break;
                        case SET_7_A:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: SET 7,A\n");
                            #endif
                            cpu.set(A,BIT_SEVEN);
                            break;
                        case SET_0_B:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: SET 0,B\n");
                            #endif
                            cpu.set(B,BIT_ZERO);
                            break;
                        case SET_1_B:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: SET 1,B\n");
                            #endif
                            cpu.set(B,BIT_ONE);
                            break;
                        case SET_2_B:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: SET 2,B\n");
                            #endif
                            cpu.set(B,BIT_TWO);
                            break;
                        case SET_3_B:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: SET 3,B\n");
                            #endif
                            cpu.set(B,BIT_THREE);
                            break;
                        case SET_4_B:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: SET 4,B\n");
                            #endif
                            cpu.set(B,BIT_FOUR);
                            break;
                        case SET_5_B:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: SET 5,B\n");
                            #endif
                            cpu.set(B,BIT_FIVE);
                            break;
                        case SET_6_B:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: SET 6,B\n");
                            #endif
                            cpu.set(B,BIT_SIX);
                            break;
                        case SET_7_B:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: SET 7,B\n");
                            #endif
                            cpu.set(B,BIT_SEVEN);
                            break;
                        case SET_0_C:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: SET 0,C\n");
                            #endif
                            cpu.set(C,BIT_ZERO);
                            break;
                        case SET_1_C:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: SET 1,C\n");
                            #endif
                            cpu.set(C,BIT_ONE);
                            break;
                        case SET_2_C:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: SET 2,C\n");
                            #endif
                            cpu.set(C,BIT_TWO);
                            break;
                        case SET_3_C:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: SET 3,C\n");
                            #endif
                            cpu.set(C,BIT_THREE);
                            break;
                        case SET_4_C:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: SET 4,C\n");
                            #endif
                            cpu.set(C,BIT_FOUR);
                            break;
                        case SET_5_C:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: SET 5,C\n");
                            #endif
                            cpu.set(C,BIT_FIVE);
                            break;
                        case SET_6_C:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: SET 6,C\n");
                            #endif
                            cpu.set(C,BIT_SIX);
                            break;
                        case SET_7_C:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: SET 7,C\n");
                            #endif
                            cpu.set(C,BIT_SEVEN);
                            break;
                        case SET_0_D:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: SET 0,D\n");
                            #endif
                            cpu.set(D,BIT_ZERO);
                            break;
                        case SET_1_D:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: SET 1,D\n");
                            #endif
                            cpu.set(D,BIT_ONE);
                            break;
                        case SET_2_D:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: SET 2,D\n");
                            #endif
                            cpu.set(D,BIT_TWO);
                            break;
                        case SET_3_D:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: SET 3,D\n");
                            #endif
                            cpu.set(D,BIT_THREE);
                            break;
                        case SET_4_D:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: SET 4,D\n");
                            #endif
                            cpu.set(D,BIT_FOUR);
                            break;
                        case SET_5_D:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: SET 5,D\n");
                            #endif
                            cpu.set(D,BIT_FIVE);
                            break;
                        case SET_6_D:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: SET 6,D\n");
                            #endif
                            cpu.set(D,BIT_SIX);
                            break;
                        case SET_7_D:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: SET 7,D\n");
                            #endif
                            cpu.set(D,BIT_SEVEN);
                            break;
                        case SET_0_E:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: SET 0,E\n");
                            #endif
                            cpu.set(E,BIT_ZERO);
                            break;
                        case SET_1_E:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: SET 1,E\n");
                            #endif
                            cpu.set(E,BIT_ONE);
                            break;
                        case SET_2_E:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: SET 2,E\n");
                            #endif
                            cpu.set(E,BIT_TWO);
                            break;
                        case SET_3_E:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: SET 3,E\n");
                            #endif
                            cpu.set(E,BIT_THREE);
                            break;
                        case SET_4_E:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: SET 4,E\n");
                            #endif
                            cpu.set(E,BIT_FOUR);
                            break;
                        case SET_5_E:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: SET 5,E\n");
                            #endif
                            cpu.set(E,BIT_FIVE);
                            break;
                        case SET_6_E:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: SET 6,E\n");
                            #endif
                            cpu.set(E,BIT_SIX);
                            break;
                        case SET_7_E:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: SET 7,E\n");
                            #endif
                            cpu.set(E,BIT_SEVEN);
                            break;
                        case SET_0_H:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: SET 0,H\n");
                            #endif
                            cpu.set(H,BIT_ZERO);
                            break;
                        case SET_1_H:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: SET 1,H\n");
                            #endif
                            cpu.set(H,BIT_ONE);
                            break;
                        case SET_2_H:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: SET 2,H\n");
                            #endif
                            cpu.set(H,BIT_TWO);
                            break;
                        case SET_3_H:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: SET 3,H\n");
                            #endif
                            cpu.set(H,BIT_THREE);
                            break;
                        case SET_4_H:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: SET 4,H\n");
                            #endif
                            cpu.set(H,BIT_FOUR);
                            break;
                        case SET_5_H:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: SET 5,H\n");
                            #endif
                            cpu.set(H,BIT_FIVE);
                            break;
                        case SET_6_H:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: SET 6,H\n");
                            #endif
                            cpu.set(H,BIT_SIX);
                            break;
                        case SET_7_H:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: SET 7,H\n");
                            #endif
                            cpu.set(H,BIT_SEVEN);
                            break;
                        case SET_0_L:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: SET 0,L\n");
                            #endif
                            cpu.set(L,BIT_ZERO);
                            break;
                        case SET_1_L:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: SET 1,L\n");
                            #endif
                            cpu.set(L,BIT_ONE);
                            break;
                        case SET_2_L:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: SET 2,L\n");
                            #endif
                            cpu.set(L,BIT_TWO);
                            break;
                        case SET_3_L:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: SET 3,L\n");
                            #endif
                            cpu.set(L,BIT_THREE);
                            break;
                        case SET_4_L:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: SET 4,L\n");
                            #endif
                            cpu.set(L,BIT_FOUR);
                            break;
                        case SET_5_L:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: SET 5,L\n");
                            #endif
                            cpu.set(L,BIT_FIVE);
                            break;
                        case SET_6_L:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: SET 6,L\n");
                            #endif
                            cpu.set(L,BIT_SIX);
                            break;
                        case SET_7_L:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: SET 7,L\n");
                            #endif
                            cpu.set(L,BIT_SEVEN);
                            break;
                        case SET_0_IND:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: SET 0,IND\n");
                            #endif
                            cpu.setInd(BIT_ZERO);
                            break;
                        case SET_1_IND:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: SET 1,IND\n");
                            #endif
                            cpu.setInd(BIT_ONE);
                            break;
                        case SET_2_IND:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: SET 2,IND\n");
                            #endif
                            cpu.setInd(BIT_TWO);
                            break;
                        case SET_3_IND:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: SET 3,IND\n");
                            #endif
                            cpu.setInd(BIT_THREE);
                            break;
                        case SET_4_IND:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: SET 4,IND\n");
                            #endif
                            cpu.setInd(BIT_FOUR);
                            break;
                        case SET_5_IND:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: SET 5,IND\n");
                            #endif
                            cpu.setInd(BIT_FIVE);
                            break;
                        case SET_6_IND:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: SET 6,IND\n");
                            #endif
                            cpu.setInd(BIT_SIX);
                            break;
                        case SET_7_IND:
                            #ifdef debug
                                printf("[DEBUG] instrunction resolved to: SET 7,IND\n");
                            #endif
                            cpu.setInd(BIT_SEVEN);
                            break;
                        default:
                            break;
                    */
                    }
                    state = FETCH_OP;
                    cpu.incPC();
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