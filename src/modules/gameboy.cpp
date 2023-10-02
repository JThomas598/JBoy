#include "gameboy.h"
#include "instructions.h"
#include <time.h>
#include <iostream>
#include <fstream>
#include <stdexcept>

using namespace std;

Gameboy::Gameboy() : mem(SYS_PERM){
    mem.write(IE_REG_ADDR, 0x00);
    mem.write(IF_REG_ADDR, 0x00);
    state = FETCH_OP;
    IME = false;
}

void Gameboy::printStatus(){
    cpu.printStatus();
    cout << "---OPCODE---" << endl << opcode_names[opcode] << endl;
    cout << "-----PPU----" << endl;
    ppu.printStatus();
    cout << "---MEMORY---" << endl;
    mem.printStatus();
}

size_t Gameboy::loadGame(std::string filename){
    char buf[ROM_BANK_SIZE];
    ifstream game(filename, ios_base::in | ios_base::binary);
    if(game.fail()){
        throw invalid_argument("[ERROR] Gameboy::loadGame(): Failed to open requested gb file\n");
    }
    int totalRead = 0;
    int currRead = 0;
    int currBank = 0;
    //Read in bank 0
    while(totalRead != ROM_BANK_SIZE){
        game.read(buf, ROM_BANK_SIZE - totalRead);
        currRead = game.gcount();
        mem.dump(totalRead, (Regval8*)buf, currRead);
        totalRead += currRead;
    }
    mem.resolveCartridgeType(); 
    totalRead = 0;
    currBank++;
    //Read in switchable banks
    while(!game.eof()){
        while(totalRead != ROM_BANK_SIZE && !game.eof()){
            game.read(buf + totalRead, ROM_BANK_SIZE - totalRead);
            currRead = game.gcount();
            totalRead += currRead;
        }
        mem.copyRomBank((Regval8*)buf, currBank);
        currBank++;
        totalRead = 0;
    }
    printf("[INFO] Rom banks loaded successfully.\n");
    game.close();
    return totalRead;
}

bool Gameboy::loadSram(std::string filename){
    char buf[RAM_BANK_BANK_SIZE];
    ifstream storage(filename, ios_base::in | ios_base::binary);
    if(storage.fail()){
        return false;
    }
    int totalRead = 0;
    int currRead = 0;
    int currBank = 0;
    while(!storage.eof()){
        while(totalRead != RAM_BANK_BANK_SIZE && !storage.eof()){
            storage.read(buf, RAM_BANK_BANK_SIZE - totalRead);
            currRead = storage.gcount();
            totalRead += currRead;
            mem.copyRamBank((Regval8*)buf, currBank);
        }
        currBank++;
        totalRead = 0;
    }
    printf("[INFO] SRAM loaded successfully.\n");
    return true;
}

void Gameboy::saveSram(std::string filename){
    ofstream storage(filename, ios_base::out | ios_base::binary);
    if(storage.fail()){
        std::cout << "failed to save game..." << std::endl;
        return;
    }
    mem.saveRamState(filename);
}

void Gameboy::clearInterrupt(Regval8 mask){
    Regval8 iFlag = mem.read(IF_REG_ADDR);
    iFlag &= ~mask;
    mem.write(IF_REG_ADDR, iFlag);
    IME = false;
}

Regval8 Gameboy::readMem(Regval16 addr){
    return mem.read(addr);
}

void Gameboy::handleInterrupt(){
    Regval8 iEn = mem.read(IE_REG_ADDR);
    Regval8 iFlag = mem.read(IF_REG_ADDR);
    if(VBLANK_INT & iEn & iFlag){
        clearInterrupt(VBLANK_INT);
        cpu.callInt(VBLANK_ISR_ADDR);
    }
    else if(LCD_STAT_INT & iEn & iFlag){
        clearInterrupt(LCD_STAT_INT);
        cpu.callInt(LCD_STAT_ISR_ADDR);
    }
    else if(TIMER_INT & iEn & iFlag){
        clearInterrupt(TIMER_INT);
        cpu.callInt(TIMER_ISR_ADDR);
    }
    else if(SERIAL_INT & iEn & iFlag){
        clearInterrupt(SERIAL_INT);
        cpu.callInt(SERIAL_ISR_ADDR);
    }
    else if(JOYPAD_INT & iEn & iFlag){
        clearInterrupt(JOYPAD_INT);
        cpu.callInt(JOYPAD_ISR_ADDR);
    }
}

Regval16 Gameboy::emulateCycle(){
    static int numCycles = 1;
    if(numCycles % 4 == 0){
        runFSM();
        numCycles = 1;
    }
    else{
        numCycles++;
    }
    dma.emulateCycle();
    ppu.emulateCycle();
    counters.emulateCycle();
    //printSerial();
    return cpu.getPC();
}

GbState Gameboy::getState(){
    GbState ret;
    ret.opcode = opcode;
    for(int i = 0; i < NUM_REGS_8; i++)
        ret.regs_8[i] = cpu.getReg((RegIndex_8)i);
    for(int i = 0; i < NUM_REGS_16; i++)
        ret.regs_16[i] = cpu.getReg((RegIndex_16)i);
    return ret;
}

void Gameboy::limitCycleRate(){
    static std::chrono::high_resolution_clock::time_point lastCycleTime = std::chrono::high_resolution_clock::now();
    const int targetCycleRate = CYCLE_RATE;  // Target frame rate in frames per second
    const std::chrono::duration<double> targetCycleDuration(1.0 / targetCycleRate);
    // Calculate time since the last frame
    std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
    lastCycleTime = currentTime;
    // Wait to draw next frame
    while (true){
        currentTime = std::chrono::high_resolution_clock::now();
        if((currentTime - lastCycleTime) > targetCycleDuration){
            lastCycleTime = currentTime;
            break;
        }
    }
}

void Gameboy::setJoypad(Regval8 byte){
    mem.setJoypadBuff(byte);
}

Regval8 Gameboy::getJoypad(){
    return mem.getJoypadBuff();
}

//TODO: Add serial interrupts if needed
void Gameboy::printSerial(){
    if(mem.read(SC) == 0x81){
        mem.write(SC, 0x00); //clear request
        char c = mem.read(SB);
        if(c == '\n'){
            cout << std::endl;
        }
        else{
            cout << c;
        }
    }
}

void Gameboy::executeCBOP(){
    switch(cb_op){
        case RRC_A:
            cpu.rrc(A, true);
            break;
        case RRC_B:
            cpu.rrc(B, true);
            break;
        case RRC_C:
            cpu.rrc(C, true);
            break;
        case RRC_D:
            cpu.rrc(D, true);
            break;
        case RRC_E:
            cpu.rrc(E, true);
            break;
        case RRC_H:
            cpu.rrc(H, true);
            break;
        case RRC_L:
            cpu.rrc(L, true);
            break;
        case RRC_IND:
            cpu.rrcInd();
            break;
        case RR_A:
            cpu.rr(A, true);
            break;
        case RR_B:
            cpu.rr(B, true);
            break;
        case RR_C:
            cpu.rr(C, true);
            break;
        case RR_D:
            cpu.rr(D, true);
            break;
        case RR_E:
            cpu.rr(E, true);
            break;
        case RR_H:
            cpu.rr(H, true);
            break;
        case RR_L:
            cpu.rr(L, true);
            break;
        case RR_IND:
            cpu.rrInd();
            break;
        case RLC_A:
            cpu.rlc(A, true);
            break;
        case RLC_B:
            cpu.rlc(B, true);
            break;
        case RLC_C:
            cpu.rlc(C, true);
            break;
        case RLC_D:
            cpu.rlc(D, true);
            break;
        case RLC_E:
            cpu.rlc(E, true);
            break;
        case RLC_H:
            cpu.rlc(H, true);
            break;
        case RLC_L:
            cpu.rlc(L, true);
            break;
        case RLC_IND:
            cpu.rlcInd();
            break;
        case RL_A:
            cpu.rl(A, true);
            break;
        case RL_B:
            cpu.rl(B, true);
            break;
        case RL_C:
            cpu.rl(C, true);
            break;
        case RL_D:
            cpu.rl(D, true);
            break;
        case RL_E:
            cpu.rl(E, true);
            break;
        case RL_H:
            cpu.rl(H, true);
            break;
        case RL_L:
            cpu.rl(L, true);
            break;
        case RL_IND:
            cpu.rlInd();
            break;
        case SLA_A:
            cpu.sla(A);
            break;
        case SLA_B:
            cpu.sla(B);
            break;
        case SLA_C:
            cpu.sla(C);
            break;
        case SLA_D:
            cpu.sla(D);
            break;
        case SLA_E:
            cpu.sla(E);
            break;
        case SLA_H:
            cpu.sla(H);
            break;
        case SLA_L:
            cpu.sla(L);
            break;
        case SLA_IND:
            cpu.slaInd();
            break;
        case SRA_A:
            cpu.sra(A);
            break;
        case SRA_B:
            cpu.sra(B);
            break;
        case SRA_C:
            cpu.sra(C);
            break;
        case SRA_D:
            cpu.sra(D);
            break;
        case SRA_E:
            cpu.sra(E);
            break;
        case SRA_H:
            cpu.sra(H);
            break;
        case SRA_L:
            cpu.sra(L);
            break;
        case SRA_IND:
            cpu.sraInd();
            break;
        case SRL_A:
            cpu.srl(A);
            break;
        case SRL_B:
            cpu.srl(B);
            break;
        case SRL_C:
            cpu.srl(C);
            break;
        case SRL_D:
            cpu.srl(D);
            break;
        case SRL_E:
            cpu.srl(E);
            break;
        case SRL_H:
            cpu.srl(H);
            break;
        case SRL_L:
            cpu.srl(L);
            break;
        case SRL_IND:
            cpu.srlInd();
            break;
        case SWAP_A:
            cpu.swap(A);
            break;
        case SWAP_B:
            cpu.swap(B);
            break;
        case SWAP_C:
            cpu.swap(C);
            break;
        case SWAP_D:
            cpu.swap(D);
            break;
        case SWAP_E:
            cpu.swap(E);
            break;
        case SWAP_H:
            cpu.swap(H);
            break;
        case SWAP_L:
            cpu.swap(L);
            break;
        case SWAP_IND:
            cpu.swapInd();
            break;
        case BIT_0_A:
            cpu.bit(A,BIT_ZERO);
            break;
        case BIT_1_A:
            cpu.bit(A,BIT_ONE);
            break;
        case BIT_2_A:
            cpu.bit(A,BIT_TWO);
            break;
        case BIT_3_A:
            cpu.bit(A,BIT_THREE);
            break;
        case BIT_4_A:
            cpu.bit(A,BIT_FOUR);
            break;
        case BIT_5_A:
            cpu.bit(A,BIT_FIVE);
            break;
        case BIT_6_A:
            cpu.bit(A,BIT_SIX);
            break;
        case BIT_7_A:
            cpu.bit(A,BIT_SEVEN);
            break;
        case BIT_0_B:
            cpu.bit(B,BIT_ZERO);
            break;
        case BIT_1_B:
            cpu.bit(B,BIT_ONE);
            break;
        case BIT_2_B:
            cpu.bit(B,BIT_TWO);
            break;
        case BIT_3_B:
            cpu.bit(B,BIT_THREE);
            break;
        case BIT_4_B:
            cpu.bit(B,BIT_FOUR);
            break;
        case BIT_5_B:
            cpu.bit(B,BIT_FIVE);
            break;
        case BIT_6_B:
            cpu.bit(B,BIT_SIX);
            break;
        case BIT_7_B:
            cpu.bit(B,BIT_SEVEN);
            break;
        case BIT_0_C:
            cpu.bit(C,BIT_ZERO);
            break;
        case BIT_1_C:
            cpu.bit(C,BIT_ONE);
            break;
        case BIT_2_C:
            cpu.bit(C,BIT_TWO);
            break;
        case BIT_3_C:
            cpu.bit(C,BIT_THREE);
            break;
        case BIT_4_C:
            cpu.bit(C,BIT_FOUR);
            break;
        case BIT_5_C:
            cpu.bit(C,BIT_FIVE);
            break;
        case BIT_6_C:
            cpu.bit(C,BIT_SIX);
            break;
        case BIT_7_C:
            cpu.bit(C,BIT_SEVEN);
            break;
        case BIT_0_D:
            cpu.bit(D,BIT_ZERO);
            break;
        case BIT_1_D:
            cpu.bit(D,BIT_ONE);
            break;
        case BIT_2_D:
            cpu.bit(D,BIT_TWO);
            break;
        case BIT_3_D:
            cpu.bit(D,BIT_THREE);
            break;
        case BIT_4_D:
            cpu.bit(D,BIT_FOUR);
            break;
        case BIT_5_D:
            cpu.bit(D,BIT_FIVE);
            break;
        case BIT_6_D:
            cpu.bit(D,BIT_SIX);
            break;
        case BIT_7_D:
            cpu.bit(D,BIT_SEVEN);
            break;
        case BIT_0_E:
            cpu.bit(E,BIT_ZERO);
            break;
        case BIT_1_E:
            cpu.bit(E,BIT_ONE);
            break;
        case BIT_2_E:
            cpu.bit(E,BIT_TWO);
            break;
        case BIT_3_E:
            cpu.bit(E,BIT_THREE);
            break;
        case BIT_4_E:
            cpu.bit(E,BIT_FOUR);
            break;
        case BIT_5_E:
            cpu.bit(E,BIT_FIVE);
            break;
        case BIT_6_E:
            cpu.bit(E,BIT_SIX);
            break;
        case BIT_7_E:
            cpu.bit(E,BIT_SEVEN);
            break;
        case BIT_0_H:
            cpu.bit(H,BIT_ZERO);
            break;
        case BIT_1_H:
            cpu.bit(H,BIT_ONE);
            break;
        case BIT_2_H:
            cpu.bit(H,BIT_TWO);
            break;
        case BIT_3_H:
            cpu.bit(H,BIT_THREE);
            break;
        case BIT_4_H:
            cpu.bit(H,BIT_FOUR);
            break;
        case BIT_5_H:
            cpu.bit(H,BIT_FIVE);
            break;
        case BIT_6_H:
            cpu.bit(H,BIT_SIX);
            break;
        case BIT_7_H:
            cpu.bit(H,BIT_SEVEN);
            break;
        case BIT_0_L:
            cpu.bit(L,BIT_ZERO);
            break;
        case BIT_1_L:
            cpu.bit(L,BIT_ONE);
            break;
        case BIT_2_L:
            cpu.bit(L,BIT_TWO);
            break;
        case BIT_3_L:
            cpu.bit(L,BIT_THREE);
            break;
        case BIT_4_L:
            cpu.bit(L,BIT_FOUR);
            break;
        case BIT_5_L:
            cpu.bit(L,BIT_FIVE);
            break;
        case BIT_6_L:
            cpu.bit(L,BIT_SIX);
            break;
        case BIT_7_L:
            cpu.bit(L,BIT_SEVEN);
            break;
        case BIT_0_IND:
            cpu.bitInd(BIT_ZERO);
            break;
        case BIT_1_IND:
            cpu.bitInd(BIT_ONE);
            break;
        case BIT_2_IND:
            cpu.bitInd(BIT_TWO);
            break;
        case BIT_3_IND:
            cpu.bitInd(BIT_THREE);
            break;
        case BIT_4_IND:
            cpu.bitInd(BIT_FOUR);
            break;
        case BIT_5_IND:
            cpu.bitInd(BIT_FIVE);
            break;
        case BIT_6_IND:
            cpu.bitInd(BIT_SIX);
            break;
        case BIT_7_IND:
            cpu.bitInd(BIT_SEVEN);
            break;
        case RES_0_A:
            cpu.res(A,BIT_ZERO);
            break;
        case RES_1_A:
            cpu.res(A,BIT_ONE);
            break;
        case RES_2_A:
            cpu.res(A,BIT_TWO);
            break;
        case RES_3_A:
            cpu.res(A,BIT_THREE);
            break;
        case RES_4_A:
            cpu.res(A,BIT_FOUR);
            break;
        case RES_5_A:
            cpu.res(A,BIT_FIVE);
            break;
        case RES_6_A:
            cpu.res(A,BIT_SIX);
            break;
        case RES_7_A:
            cpu.res(A,BIT_SEVEN);
            break;
        case RES_0_B:
            cpu.res(B,BIT_ZERO);
            break;
        case RES_1_B:
            cpu.res(B,BIT_ONE);
            break;
        case RES_2_B:
            cpu.res(B,BIT_TWO);
            break;
        case RES_3_B:
            cpu.res(B,BIT_THREE);
            break;
        case RES_4_B:
            cpu.res(B,BIT_FOUR);
            break;
        case RES_5_B:
            cpu.res(B,BIT_FIVE);
            break;
        case RES_6_B:
            cpu.res(B,BIT_SIX);
            break;
        case RES_7_B:
            cpu.res(B,BIT_SEVEN);
            break;
        case RES_0_C:
            cpu.res(C,BIT_ZERO);
            break;
        case RES_1_C:
            cpu.res(C,BIT_ONE);
            break;
        case RES_2_C:
            cpu.res(C,BIT_TWO);
            break;
        case RES_3_C:
            cpu.res(C,BIT_THREE);
            break;
        case RES_4_C:
            cpu.res(C,BIT_FOUR);
            break;
        case RES_5_C:
            cpu.res(C,BIT_FIVE);
            break;
        case RES_6_C:
            cpu.res(C,BIT_SIX);
            break;
        case RES_7_C:
            cpu.res(C,BIT_SEVEN);
            break;
        case RES_0_D:
            cpu.res(D,BIT_ZERO);
            break;
        case RES_1_D:
            cpu.res(D,BIT_ONE);
            break;
        case RES_2_D:
            cpu.res(D,BIT_TWO);
            break;
        case RES_3_D:
            cpu.res(D,BIT_THREE);
            break;
        case RES_4_D:
            cpu.res(D,BIT_FOUR);
            break;
        case RES_5_D:
            cpu.res(D,BIT_FIVE);
            break;
        case RES_6_D:
            cpu.res(D,BIT_SIX);
            break;
        case RES_7_D:
            cpu.res(D,BIT_SEVEN);
            break;
        case RES_0_E:
            cpu.res(E,BIT_ZERO);
            break;
        case RES_1_E:
            cpu.res(E,BIT_ONE);
            break;
        case RES_2_E:
            cpu.res(E,BIT_TWO);
            break;
        case RES_3_E:
            cpu.res(E,BIT_THREE);
            break;
        case RES_4_E:
            cpu.res(E,BIT_FOUR);
            break;
        case RES_5_E:
            cpu.res(E,BIT_FIVE);
            break;
        case RES_6_E:
            cpu.res(E,BIT_SIX);
            break;
        case RES_7_E:
            cpu.res(E,BIT_SEVEN);
            break;
        case RES_0_H:
            cpu.res(H,BIT_ZERO);
            break;
        case RES_1_H:
            cpu.res(H,BIT_ONE);
            break;
        case RES_2_H:
            cpu.res(H,BIT_TWO);
            break;
        case RES_3_H:
            cpu.res(H,BIT_THREE);
            break;
        case RES_4_H:
            cpu.res(H,BIT_FOUR);
            break;
        case RES_5_H:
            cpu.res(H,BIT_FIVE);
            break;
        case RES_6_H:
            cpu.res(H,BIT_SIX);
            break;
        case RES_7_H:
            cpu.res(H,BIT_SEVEN);
            break;
        case RES_0_L:
            cpu.res(L,BIT_ZERO);
            break;
        case RES_1_L:
            cpu.res(L,BIT_ONE);
            break;
        case RES_2_L:
            cpu.res(L,BIT_TWO);
            break;
        case RES_3_L:
            cpu.res(L,BIT_THREE);
            break;
        case RES_4_L:
            cpu.res(L,BIT_FOUR);
            break;
        case RES_5_L:
            cpu.res(L,BIT_FIVE);
            break;
        case RES_6_L:
            cpu.res(L,BIT_SIX);
            break;
        case RES_7_L:
            cpu.res(L,BIT_SEVEN);
            break;
        case RES_0_IND:
            cpu.resInd(BIT_ZERO);
            break;
        case RES_1_IND:
            cpu.resInd(BIT_ONE);
            break;
        case RES_2_IND:
            cpu.resInd(BIT_TWO);
            break;
        case RES_3_IND:
            cpu.resInd(BIT_THREE);
            break;
        case RES_4_IND:
            cpu.resInd(BIT_FOUR);
            break;
        case RES_5_IND:
            cpu.resInd(BIT_FIVE);
            break;
        case RES_6_IND:
            cpu.resInd(BIT_SIX);
            break;
        case RES_7_IND:
            cpu.resInd(BIT_SEVEN);
            break;
        case SET_0_A:
            cpu.set(A,BIT_ZERO);
            break;
        case SET_1_A:
            cpu.set(A,BIT_ONE);
            break;
        case SET_2_A:
            cpu.set(A,BIT_TWO);
            break;
        case SET_3_A:
            cpu.set(A,BIT_THREE);
            break;
        case SET_4_A:
            cpu.set(A,BIT_FOUR);
            break;
        case SET_5_A:
            cpu.set(A,BIT_FIVE);
            break;
        case SET_6_A:
            cpu.set(A,BIT_SIX);
            break;
        case SET_7_A:
            cpu.set(A,BIT_SEVEN);
            break;
        case SET_0_B:
            cpu.set(B,BIT_ZERO);
            break;
        case SET_1_B:
            cpu.set(B,BIT_ONE);
            break;
        case SET_2_B:
            cpu.set(B,BIT_TWO);
            break;
        case SET_3_B:
            cpu.set(B,BIT_THREE);
            break;
        case SET_4_B:
            cpu.set(B,BIT_FOUR);
            break;
        case SET_5_B:
            cpu.set(B,BIT_FIVE);
            break;
        case SET_6_B:
            cpu.set(B,BIT_SIX);
            break;
        case SET_7_B:
            cpu.set(B,BIT_SEVEN);
            break;
        case SET_0_C:
            cpu.set(C,BIT_ZERO);
            break;
        case SET_1_C:
            cpu.set(C,BIT_ONE);
            break;
        case SET_2_C:
            cpu.set(C,BIT_TWO);
            break;
        case SET_3_C:
            cpu.set(C,BIT_THREE);
            break;
        case SET_4_C:
            cpu.set(C,BIT_FOUR);
            break;
        case SET_5_C:
            cpu.set(C,BIT_FIVE);
            break;
        case SET_6_C:
            cpu.set(C,BIT_SIX);
            break;
        case SET_7_C:
            cpu.set(C,BIT_SEVEN);
            break;
        case SET_0_D:
            cpu.set(D,BIT_ZERO);
            break;
        case SET_1_D:
            cpu.set(D,BIT_ONE);
            break;
        case SET_2_D:
            cpu.set(D,BIT_TWO);
            break;
        case SET_3_D:
            cpu.set(D,BIT_THREE);
            break;
        case SET_4_D:
            cpu.set(D,BIT_FOUR);
            break;
        case SET_5_D:
            cpu.set(D,BIT_FIVE);
            break;
        case SET_6_D:
            cpu.set(D,BIT_SIX);
            break;
        case SET_7_D:
            cpu.set(D,BIT_SEVEN);
            break;
        case SET_0_E:
            cpu.set(E,BIT_ZERO);
            break;
        case SET_1_E:
            cpu.set(E,BIT_ONE);
            break;
        case SET_2_E:
            cpu.set(E,BIT_TWO);
            break;
        case SET_3_E:
            cpu.set(E,BIT_THREE);
            break;
        case SET_4_E:
            cpu.set(E,BIT_FOUR);
            break;
        case SET_5_E:
            cpu.set(E,BIT_FIVE);
            break;
        case SET_6_E:
            cpu.set(E,BIT_SIX);
            break;
        case SET_7_E:
            cpu.set(E,BIT_SEVEN);
            break;
        case SET_0_H:
            cpu.set(H,BIT_ZERO);
            break;
        case SET_1_H:
            cpu.set(H,BIT_ONE);
            break;
        case SET_2_H:
            cpu.set(H,BIT_TWO);
            break;
        case SET_3_H:
            cpu.set(H,BIT_THREE);
            break;
        case SET_4_H:
            cpu.set(H,BIT_FOUR);
            break;
        case SET_5_H:
            cpu.set(H,BIT_FIVE);
            break;
        case SET_6_H:
            cpu.set(H,BIT_SIX);
            break;
        case SET_7_H:
            cpu.set(H,BIT_SEVEN);
            break;
        case SET_0_L:
            cpu.set(L,BIT_ZERO);
            break;
        case SET_1_L:
            cpu.set(L,BIT_ONE);
            break;
        case SET_2_L:
            cpu.set(L,BIT_TWO);
            break;
        case SET_3_L:
            cpu.set(L,BIT_THREE);
            break;
        case SET_4_L:
            cpu.set(L,BIT_FOUR);
            break;
        case SET_5_L:
            cpu.set(L,BIT_FIVE);
            break;
        case SET_6_L:
            cpu.set(L,BIT_SIX);
            break;
        case SET_7_L:
            cpu.set(L,BIT_SEVEN);
            break;
        case SET_0_IND:
            cpu.setInd(BIT_ZERO);
            break;
        case SET_1_IND:
            cpu.setInd(BIT_ONE);
            break;
        case SET_2_IND:
            cpu.setInd(BIT_TWO);
            break;
        case SET_3_IND:
            cpu.setInd(BIT_THREE);
            break;
        case SET_4_IND:
            cpu.setInd(BIT_FOUR);
            break;
        case SET_5_IND:
            cpu.setInd(BIT_FIVE);
            break;
        case SET_6_IND:
            cpu.setInd(BIT_SIX);
            break;
        case SET_7_IND:
            cpu.setInd(BIT_SEVEN);
            break;
        default:
            break;
    }
}
void Gameboy::runFSM(){
    if(state == FETCH_OP){
        //weird case where a halt is ceased when ANY interrupt is triggered,
        //regardless of the IME value.
        if(mem.read(IF_REG_ADDR) && opcode == HALT){
            cpu.incPC();
        }
        if(IME){
            handleInterrupt();
        }
        opcode = mem.read(cpu.getPC());
    }
    switch(opcode){
        case NOP:
            cpu.incPC();
            break; 
        case HALT:
            break;
        case STOP:
            break; 
        case DI:
            IME = false;
            cpu.incPC();
            break;
        case EI:
            IME = true;
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
                    imm_8 = mem.read(cpu.getPC());
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
                    imm_8 = mem.read(cpu.getPC());
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
                    imm_8 = mem.read(cpu.getPC());
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
                    imm_8 = mem.read(cpu.getPC());
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
                    imm_8 = mem.read(cpu.getPC());
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
                    imm_8 = mem.read(cpu.getPC());
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
                    imm_8 = mem.read(cpu.getPC());
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
                    imm_8 = mem.read(cpu.getPC());
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
                    lsb = mem.read(cpu.getPC());
                    state = FETCH_2;
                    cpu.incPC(); 
                    break;
                case FETCH_2:
                    msb = mem.read(cpu.getPC());
                    state = EXECUTE_1;
                    break;
                case EXECUTE_1:
                    imm_16 = 0x0000;
                    imm_16 |= msb;
                    imm_16 = imm_16 << 8;
                    imm_16 |= lsb;
                    cpu.loadAImmDirect(imm_16);
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
                    lsb = mem.read(cpu.getPC());
                    state = FETCH_2;
                    cpu.incPC(); 
                    break;
                case FETCH_2:
                    msb = mem.read(cpu.getPC());
                    state = EXECUTE_1;
                    break;
                case EXECUTE_1:
                    imm_16 = 0x0000;
                    imm_16 |= msb;
                    imm_16 = imm_16 << 8;
                    imm_16 |= lsb;
                    cpu.loadImmADirect(imm_16);
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
                    imm_8 = mem.read(cpu.getPC());
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
                    imm_8 = mem.read(cpu.getPC());
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
                    lsb = mem.read(cpu.getPC());
                    cpu.incPC();
                    state = EXECUTE_1;
                    break;
                case EXECUTE_1:
                    msb = mem.read(cpu.getPC());
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
                    lsb = mem.read(cpu.getPC());
                    state = EXECUTE_1;
                    cpu.incPC();
                    break;
                case EXECUTE_1:
                    msb = mem.read(cpu.getPC());
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
                    lsb = mem.read(cpu.getPC());
                    state = EXECUTE_1;
                    cpu.incPC();
                    break;
                case EXECUTE_1:
                    msb = mem.read(cpu.getPC());
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
                    lsb = mem.read(cpu.getPC());
                    state = EXECUTE_1;
                    cpu.incPC();
                    break;
                case EXECUTE_1:
                    msb = mem.read(cpu.getPC());
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
                    lsb = mem.read(cpu.getPC());
                    state = FETCH_2;
                    cpu.incPC();
                    break;
                case FETCH_2:
                    msb = mem.read(cpu.getPC());
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
        case ADD_IMM:
            switch(state){
                case FETCH_OP:
                    state = EXECUTE_1;
                    cpu.incPC();
                    break;
                case EXECUTE_1:
                    imm_8 = mem.read(cpu.getPC());
                    cpu.addImm(imm_8);
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
        case ADD_IND:
            switch(state){
                case FETCH_OP:
                    state = EXECUTE_1;
                    break;
                case EXECUTE_1:
                    cpu.addIndirect();
                    state = FETCH_OP;
                    cpu.incPC();
                    break;
                default:
                    break;
            }
            break;
        case ADC_A:
            cpu.addRegCarry(A);
            cpu.incPC();
            break;
        case ADC_B:
            cpu.addRegCarry(B);
            cpu.incPC();
            break;
        case ADC_C:
            cpu.addRegCarry(C);
            cpu.incPC();
            break;
        case ADC_D:
            cpu.addRegCarry(D);
            cpu.incPC();
            break;
        case ADC_E:
            cpu.addRegCarry(E);
            cpu.incPC();
            break;
        case ADC_H:
            cpu.addRegCarry(H);
            cpu.incPC();
            break;
        case ADC_L:
            cpu.addRegCarry(L);
            cpu.incPC();
            break;
        case ADC_IND:
            switch(state){
                case FETCH_OP:
                    state = EXECUTE_1;
                    break;
                case EXECUTE_1:
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
                    imm_8 = mem.read(cpu.getPC());
                    cpu.addImmCarry(imm_8);
                    state = FETCH_OP;
                    cpu.incPC();
                    break;
                default:
                    break;
            }
            break;
        case SUB_A:
            cpu.subReg(A);
            cpu.incPC();
            break;
        case SUB_B:
            cpu.subReg(B);
            cpu.incPC();
            break;
        case SUB_C:
            cpu.subReg(C);
            cpu.incPC();
            break;
        case SUB_D:
            cpu.subReg(D);
            cpu.incPC();
            break;
        case SUB_E:
            cpu.subReg(E);
            cpu.incPC();
            break;
        case SUB_H:
            cpu.subReg(H);
            cpu.incPC();
            break;
        case SUB_L:
            cpu.subReg(L);
            cpu.incPC();
            break;
        case SUB_IND:
            switch(state){
                case FETCH_OP:
                    state = EXECUTE_1;
                    break;
                case EXECUTE_1:
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
                    imm_8 = mem.read(cpu.getPC());
                    cpu.subImm(imm_8);
                    state = FETCH_OP;
                    cpu.incPC();
                    break;
                default:
                    break;
            }
            break;
        case SBC_A:
            cpu.subRegCarry(A);
            cpu.incPC();
            break;
        case SBC_B:
            cpu.subRegCarry(B);
            cpu.incPC();
            break;
        case SBC_C:
            cpu.subRegCarry(C);
            cpu.incPC();
            break;
        case SBC_D:
            cpu.subRegCarry(D);
            cpu.incPC();
            break;
        case SBC_E:
            cpu.subRegCarry(E);
            cpu.incPC();
            break;
        case SBC_H:
            cpu.subRegCarry(H);
            cpu.incPC();
            break;
        case SBC_L:
            cpu.subRegCarry(L);
            cpu.incPC();
            break;
        case SBC_IND:
            switch(state){
                case FETCH_OP:
                    state = EXECUTE_1;
                    break;
                case EXECUTE_1:
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
                    imm_8 = mem.read(cpu.getPC());
                    cpu.subImmCarry(imm_8);
                    state = FETCH_OP;
                    cpu.incPC();
                    break;
                default:
                    break;
            }
            break;
        case CP_A:
            cpu.compareReg(A);
            cpu.incPC();
            break;
        case CP_B:
            cpu.compareReg(B);
            cpu.incPC();
            break;
        case CP_C:
            cpu.compareReg(C);
            cpu.incPC();
            break;
        case CP_D:
            cpu.compareReg(D);
            cpu.incPC();
            break;
        case CP_E:
            cpu.compareReg(E);
            cpu.incPC();
            break;
        case CP_H:
            cpu.compareReg(H);
            cpu.incPC();
            break;
        case CP_L:
            cpu.compareReg(L);
            cpu.incPC();
            break;
        case CP_IND:
            switch(state){
                case FETCH_OP:
                    state = EXECUTE_1;
                    break;
                case EXECUTE_1:
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
                    imm_8 = mem.read(cpu.getPC());
                    cpu.compareImm(imm_8);
                    state = FETCH_OP;
                    cpu.incPC();
                    break;
                default:
                    break;
            }
            break;
        case INC_A:
            cpu.incReg(A);
            cpu.incPC();
            break;
        case INC_B:
            cpu.incReg(B);
            cpu.incPC();
            break;
        case INC_C:
            cpu.incReg(C);
            cpu.incPC();
            break;
        case INC_D:
            cpu.incReg(D);
            cpu.incPC();
            break;
        case INC_E:
            cpu.incReg(E);
            cpu.incPC();
            break;
        case INC_H:
            cpu.incReg(H);
            cpu.incPC();
            break;
        case INC_L:
            cpu.incReg(L);
            cpu.incPC();
            break;
        case INC_IND:
            switch(state){
                case FETCH_OP:
                    state = EXECUTE_1;
                    break;
                case EXECUTE_1:
                    cpu.incIndirect();
                    state = FETCH_OP;
                    cpu.incPC();
                    break;
                default:
                    break;
            }
            break;
        case DEC_A:
            cpu.decReg(A);
            cpu.incPC();
            break;
        case DEC_B:
            cpu.decReg(B);
            cpu.incPC();
            break;
        case DEC_C:
            cpu.decReg(C);
            cpu.incPC();
            break;
        case DEC_D:
            cpu.decReg(D);
            cpu.incPC();
            break;
        case DEC_E:
            cpu.decReg(E);
            cpu.incPC();
            break;
        case DEC_H:
            cpu.decReg(H);
            cpu.incPC();
            break;
        case DEC_L:
            cpu.decReg(L);
            cpu.incPC();
            break;
        case DEC_IND:
            switch(state){
                case FETCH_OP:
                    state = EXECUTE_1;
                    break;
                case EXECUTE_1:
                    cpu.decIndirect();
                    state = FETCH_OP;
                    cpu.incPC();
                    break;
                default:
                    break;
            }
            break;
        case AND_A:
            cpu.andReg(A);
            cpu.incPC();
            break;
        case AND_B:
            cpu.andReg(B);
            cpu.incPC();
            break;
        case AND_C:
            cpu.andReg(C);
            cpu.incPC();
            break;
        case AND_D:
            cpu.andReg(D);
            cpu.incPC();
            break;
        case AND_E:
            cpu.andReg(E);
            cpu.incPC();
            break;
        case AND_H:
            cpu.andReg(H);
            cpu.incPC();
            break;
        case AND_L:
            cpu.andReg(L);
            cpu.incPC();
            break;
        case AND_IND:
            switch(state){
                case FETCH_OP:
                    state = EXECUTE_1;
                    break;
                case EXECUTE_1:
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
                    imm_8 = mem.read(cpu.getPC());
                    cpu.andImm(imm_8);
                    state = FETCH_OP;
                    cpu.incPC();
                    break;
                default:
                    break;
            }
            break;
        case OR_A:
            cpu.orReg(A);
            cpu.incPC();
            break;
        case OR_B:
            cpu.orReg(B);
            cpu.incPC();
            break;
        case OR_C:
            cpu.orReg(C);
            cpu.incPC();
            break;
        case OR_D:
            cpu.orReg(D);
            cpu.incPC();
            break;
        case OR_E:
            cpu.orReg(E);
            cpu.incPC();
            break;
        case OR_H:
            cpu.orReg(H);
            cpu.incPC();
            break;
        case OR_L:
            cpu.orReg(L);
            cpu.incPC();
            break;
        case OR_IND:
            switch(state){
                case FETCH_OP:
                    state = EXECUTE_1;
                    break;
                case EXECUTE_1:
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
                    imm_8 = mem.read(cpu.getPC());
                    cpu.orImm(imm_8);
                    state = FETCH_OP;
                    cpu.incPC();
                    break;
                default:
                    break;
            }
            break;
        case XOR_A:
            cpu.xorReg(A);
            cpu.incPC();
            break;
        case XOR_B:
            cpu.xorReg(B);
            cpu.incPC();
            break;
        case XOR_C:
            cpu.xorReg(C);
            cpu.incPC();
            break;
        case XOR_D:
            cpu.xorReg(D);
            cpu.incPC();
            break;
        case XOR_E:
            cpu.xorReg(E);
            cpu.incPC();
            break;
        case XOR_H:
            cpu.xorReg(H);
            cpu.incPC();
            break;
        case XOR_L:
            cpu.xorReg(L);
            cpu.incPC();
            break;
        case XOR_IND:
            switch(state){
                case FETCH_OP:
                    state = EXECUTE_1;
                    break;
                case EXECUTE_1:
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
                    imm_8 = mem.read(cpu.getPC());
                    cpu.xorImm(imm_8);
                    state = FETCH_OP;
                    cpu.incPC();
                    break;
                default:
                    break;
            }
            break;
        case CCF:
            cpu.compCarryFlag();
            cpu.incPC();
            break;
        case SCF:
            cpu.setCarryFlag();
            cpu.incPC();
            break;
        case DAA:
            cpu.decimalAdjustAcc();
            cpu.incPC();
            break;
        case CPL:
            cpu.compAcc();
            cpu.incPC();
            break;
        case INC_BC:
            cpu.incBC();
            cpu.incPC();
            break;
        case INC_DE:
            cpu.incDE();
            cpu.incPC();
            break;
        case INC_HL:
            cpu.incHL();
            cpu.incPC();
            break;
        case INC_SP:
            cpu.incSP();
            cpu.incPC();
            break;
        case DEC_BC:
            cpu.decBC();
            cpu.incPC();
            break;
        case DEC_DE:
            cpu.decDE();
            cpu.incPC();
            break;
        case DEC_HL:
            cpu.decHL();
            cpu.incPC();
            break;
        case DEC_SP:
            cpu.decSP();
            cpu.incPC();
            break;
        case ADD_HL_BC:
            cpu.addHLRegPair(B,C);
            cpu.incPC();
            break;
        case ADD_HL_DE:
            cpu.addHLRegPair(D,E);
            cpu.incPC();
            break;
        case ADD_HL_HL:
            cpu.addHLRegPair(H,L);
            cpu.incPC();
            break;
        case ADD_HL_SP:
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
                    imm_8 = mem.read(cpu.getPC());
                    state = EXECUTE_1;
                    break;
                case EXECUTE_1:
                    cpu.loadHLSPOffset((int8_t)imm_8);
                    cpu.incPC();
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
                    imm_8 = mem.read(cpu.getPC());
                    cpu.addSPImm((int8_t)imm_8);
                    state = FETCH_OP;
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
                    lsb = mem.read(cpu.getPC());
                    state = FETCH_2;
                    cpu.incPC(); 
                    break;
                case FETCH_2:
                    msb = mem.read(cpu.getPC());
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
        case JP_HL:
            cpu.jumpHL();
            break;
        case JP_C:
            switch(state){
                case FETCH_OP:
                    state = FETCH_1;
                    cpu.incPC(); 
                    break;
                case FETCH_1:
                    lsb = mem.read(cpu.getPC());
                    state = FETCH_2;
                    cpu.incPC(); 
                    break;
                case FETCH_2:
                    msb = mem.read(cpu.getPC());
                    imm_16 = 0x0000;
                    imm_16 |= msb;
                    imm_16 = imm_16 << 8;
                    imm_16 |= lsb;
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
                    lsb = mem.read(cpu.getPC());
                    state = FETCH_2;
                    cpu.incPC(); 
                    break;
                case FETCH_2:
                    msb = mem.read(cpu.getPC());
                    imm_16 = 0x0000;
                    imm_16 |= msb;
                    imm_16 = imm_16 << 8;
                    imm_16 |= lsb;
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
                    lsb = mem.read(cpu.getPC());
                    state = FETCH_2;
                    cpu.incPC(); 
                    break;
                case FETCH_2:
                    msb = mem.read(cpu.getPC());
                    imm_16 = 0x0000;
                    imm_16 |= msb;
                    imm_16 = imm_16 << 8;
                    imm_16 |= lsb;
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
                    lsb = mem.read(cpu.getPC());
                    state = FETCH_2;
                    cpu.incPC(); 
                    break;
                case FETCH_2:
                    msb = mem.read(cpu.getPC());
                    imm_16 = 0x0000;
                    imm_16 |= msb;
                    imm_16 = imm_16 << 8;
                    imm_16 |= lsb;
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
                    imm_8 = mem.read(cpu.getPC());
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
                    imm_8 = mem.read(cpu.getPC());
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
                    imm_8 = mem.read(cpu.getPC());
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
                    imm_8 = mem.read(cpu.getPC());
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
                    imm_8 = mem.read(cpu.getPC());
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
                    lsb = mem.read(cpu.getPC());
                    state = FETCH_2;
                    cpu.incPC();
                    break;
                case FETCH_2:
                    msb = mem.read(cpu.getPC());
                    state = EXECUTE_1;
                    break;
                case EXECUTE_1:
                    imm_16 = 0x0000;
                    imm_16 |= msb;
                    imm_16 = imm_16 << 8;
                    imm_16 |= lsb;
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
                    lsb = mem.read(cpu.getPC());
                    state = FETCH_2;
                    cpu.incPC();
                    break;
                case FETCH_2:
                    msb = mem.read(cpu.getPC());
                    imm_16 = 0x0000;
                    imm_16 |= msb;
                    imm_16 = imm_16 << 8;
                    imm_16 |= lsb;
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
                    lsb = mem.read(cpu.getPC());
                    state = FETCH_2;
                    cpu.incPC();
                    break;
                case FETCH_2:
                    msb = mem.read(cpu.getPC());
                    imm_16 = 0x0000;
                    imm_16 |= msb;
                    imm_16 = imm_16 << 8;
                    imm_16 |= lsb;
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
                    lsb = mem.read(cpu.getPC());
                    state = FETCH_2;
                    cpu.incPC();
                    break;
                case FETCH_2:
                    msb = mem.read(cpu.getPC());
                    imm_16 = 0x0000;
                    imm_16 |= msb;
                    imm_16 = imm_16 << 8;
                    imm_16 |= lsb;
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
                    lsb = mem.read(cpu.getPC());
                    state = FETCH_2;
                    cpu.incPC();
                    break;
                case FETCH_2:
                    msb = mem.read(cpu.getPC());
                    imm_16 = 0x0000;
                    imm_16 |= msb;
                    imm_16 = imm_16 << 8;
                    imm_16 |= lsb;
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
                    cpu.ret();
                    IME = true;
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
            cpu.rrc(A, false);
            cpu.incPC();
            break;
        case RR_A:
            cpu.rr(A, false);
            cpu.incPC();
            break;
            break;
        case RL_A:
            cpu.rl(A, false);
            cpu.incPC();
            break;
        case RLC_A:
            cpu.rlc(A, false);
            cpu.incPC();
            break;
        case CB_OP:
            switch(state){
                case FETCH_OP:
                    state = EXECUTE_1; 
                    cpu.incPC();
                    break;
                case EXECUTE_1:
                    cb_op = mem.read(cpu.getPC());
                    executeCBOP();
                    state = FETCH_OP;
                    cpu.incPC();
                    break;
                default:
                    break;
            }
            break;
        default:
            std::cout << "last opcode: 0x" << std::hex << (int)opcode << std::endl;
            throw runtime_error("[ERROR] Opcode could not be resolved.\n");
            break;
    }
}