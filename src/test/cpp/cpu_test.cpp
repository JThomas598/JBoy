#include "cpu.h"
#include "memory.h"
#include <stdio.h>

using namespace std;

constexpr char BLUE[] = "\033[34m";
constexpr char GREEN[] = "\033[32m";
constexpr char RED[] = "\033[31m";
constexpr char RESET[] = "\033[0m";

constexpr uint8_t NUM_REGS_8 = 8;
constexpr uint8_t NUM_REGS_16 = 4;

void colorPrint(const char* color, const char* s){
    cout << color << s << RESET << endl;
}

int main(int argc, char** argv){
    Memory mem;
    Memory& rmem = mem;
    CPU cpu(rmem);
    Regval8 val_8;
    Regval16 val_16;
    Regval16 addr;
    //8 bit loads
    cout << "===8-bit Loads===" << endl;
    cout << "LD r,n Test" << endl;
    val_8 = 0x50;
    cpu.loadRegImm(A, val_8);
    if(cpu.getReg(A) != val_8){
        colorPrint(RED, "FAILURE");
    }
    else{
        colorPrint(GREEN, "SUCCESS");
    }
    cout << "LD r,r Test" << endl;
    cpu.loadRegImm(B,0x10);
    cpu.loadRegReg(A,B);
    if(cpu.getReg(A) != cpu.getReg(B)){
        colorPrint(RED, "FAILURE");
    }
    else{
        colorPrint(GREEN, "SUCCESS");
    }
    cout << "LD r, (HL) Test" << endl; 
    val_8 = 0xc0;
    cpu.loadRegImm(H, val_8);
    val_8 = 0x00;
    cpu.loadRegImm(L, val_8);
    cpu.loadIndirectReg(A);
    cpu.loadRegIndirect(D);
    if(cpu.getReg(D) != cpu.getReg(A)){
        colorPrint(RED, "FAILURE");
    }
    else{
        colorPrint(GREEN,"SUCCESS");
    }
    cout << "LD (HL), n Test" << endl; 
    val_8 = 0xF0;
    cpu.loadIndirectImm(val_8);
    if(mem.read(0xc000) != val_8){
        colorPrint(RED, "FAILURE");
    }
    else{
        colorPrint(GREEN, "SUCCESS");
    }
    cout << "LD (HL), r Test" << endl; 
    val_8 = 0xD5;
    cpu.loadRegImm(D, val_8);
    cpu.loadIndirectReg(D);
    if(mem.read(0xc000) != cpu.getReg(D)){
        colorPrint(RED, "FAILURE");
    }
    else{
        colorPrint(GREEN, "SUCCESS");
    }
    cout << "LD A, (BC) Test" << endl; 
    val_8 = 0xc0;
    cpu.loadRegImm(H,val_8);
    cpu.loadRegImm(B,val_8);
    cpu.loadRegImm(D,val_8);
    val_8 = 0x0a;
    cpu.loadRegImm(L,val_8);
    cpu.loadRegImm(C,val_8);
    cpu.loadRegImm(E,val_8);
    val_8 = 0xfc;
    cpu.loadIndirectImm(val_8);
    cpu.loadABC();
    if(cpu.getReg(A) != val_8){
        colorPrint(RED, "FAILURE");
    }
    else{
        colorPrint(GREEN, "SUCCESS");
    }
    cout << "LD A, (DE) Test" << endl; 
    cpu.loadRegImm(A, 0x00);
    cpu.loadADE();
    if(cpu.getReg(A) != val_8){
        colorPrint(RED, "FAILURE");
    }
    else{
        colorPrint(GREEN, "SUCCESS");
    }
    cout << "LD (BC), A Test" << endl; 
    cpu.loadRegImm(A, 0x00);
    cpu.loadBCA();
    if(mem.read(0xc00a) != 0){
        colorPrint(RED, "FAILURE");
    }
    else{
        colorPrint(GREEN, "SUCCESS");
    }
    cout << "LD (DE), A Test" << endl; 
    cpu.loadDEA();
    if(mem.read(0xc00a) != 0){
        colorPrint(RED, "FAILURE");
    }
    else{
        colorPrint(GREEN, "SUCCESS");
    }
    cout << "LD A, (nn) Test" << endl;
    val_8 = 0x32;
    cpu.loadIndirectImm(val_8);
    addr = 0xc00a;
    cpu.loadAImmDirect(addr);
    if(cpu.getReg(A) != val_8){
        colorPrint(RED, "FAILURE");
    }
    else{
        colorPrint(GREEN, "SUCCESS");
    }
    cout << "LD (nn), A Test" << endl;
    val_8 = 0x48;
    cpu.loadRegImm(A, val_8);
    addr = 0xc00a;
    cpu.loadImmADirect(addr);
    if(mem.read(addr) != val_8){
        colorPrint(RED, "FAILURE");
    }
    else{
        colorPrint(GREEN, "SUCCESS");
    }
    cout << "LDH A, (C) Test" << endl;
    val_8 = 0xE8;
    addr = 0xFF00;
    mem.write(addr, val_8);
    val_8 = 0x00;
    cpu.loadRegImm(C, val_8);
    cpu.loadRegImm(A, val_8);
    cpu.loadHighAC();
    if(mem.read(addr) != cpu.getReg(A)){
        colorPrint(RED, "FAILURE");
    }
    else{
        colorPrint(GREEN, "SUCCESS");
    }
    cout << "LDH (C), A Test" << endl;
    addr = 0xFFAB;
    val_8 = 0x00;
    mem.write(addr,val_8);
    val_8 = 0xFF;
    cpu.loadRegImm(A, val_8);
    val_8 = 0xAB;
    cpu.loadRegImm(C,val_8);
    cpu.loadHighCA();
    if(mem.read(addr) != cpu.getReg(A)){
        colorPrint(RED, "FAILURE");
    }
    else{
        colorPrint(GREEN, "SUCCESS");
    }
    cout << "LDH (n), A Test" << endl;
    addr = 0xFF23;
    val_8 = 0xAC;
    cpu.loadRegImm(A, val_8);
    val_8 = 0x00;
    mem.write(addr, val_8);
    val_8 = 0x23;
    cpu.loadHighImmA(val_8);
    if(mem.read(addr) != cpu.getReg(A)){
        colorPrint(RED, "FAILURE");
    }
    else{
        colorPrint(GREEN, "SUCCESS");
    }
    cout << "LDH A, (n) Test" << endl;
    addr = 0xFFD4;
    val_8 = 0x57; 
    mem.write(addr, val_8);
    val_8 = 0x00;
    cpu.loadRegImm(A, val_8);
    val_8 = 0xD4;
    cpu.loadHighImmA(val_8);
    if(mem.read(addr) != cpu.getReg(A)){
        colorPrint(RED, "FAILURE");
    }
    else{
        colorPrint(GREEN, "SUCCESS");
    }
    cout << "LD A, (HL-) Test" << endl;
    addr = 0xc000;
    val_8 = 0x67;
    mem.write(addr, val_8);
    val_8 = 0xc0;
    cpu.loadRegImm(H, val_8);
    val_8 = 0x00;
    cpu.loadRegImm(L, val_8);
    cpu.loadRegImm(A, val_8);
    cpu.loadAIndirectDec();
    if(cpu.getReg(A) != mem.read(addr) ||
        cpu.getReg(H) != (Regval8)0xBF||
        cpu.getReg(L) != (Regval8)0xFF){
        colorPrint(RED, "FAILURE");
    }
    else{
        colorPrint(GREEN, "SUCCESS");
    }
    cout << "LD (HL-), A Test" << endl;
    addr = 0xc000;
    val_8 = 0x00;
    mem.write(addr, val_8);
    val_8 = 0xc0;
    cpu.loadRegImm(H, val_8);
    val_8 = 0x00;
    cpu.loadRegImm(L, val_8);
    val_8 = 0xEE;
    cpu.loadRegImm(A, val_8);
    cpu.loadIndirectADec();
    if(cpu.getReg(A) != mem.read(addr) ||
        cpu.getReg(H) != (Regval8)0xBF||
        cpu.getReg(L) != (Regval8)0xFF){
        colorPrint(RED, "FAILURE");
    }
    else{
        colorPrint(GREEN, "SUCCESS");
    }
    cout << "LD A, (HL+) Test" << endl;
    addr = 0xc000;
    val_8 = 0x67;
    mem.write(addr, val_8);
    val_8 = 0xc0;
    cpu.loadRegImm(H, val_8);
    val_8 = 0x00;
    cpu.loadRegImm(L, val_8);
    cpu.loadRegImm(A, val_8);
    cpu.loadAIndirectInc();
    if(cpu.getReg(A) != mem.read(addr) ||
        cpu.getReg(H) != (Regval8)0xC0||
        cpu.getReg(L) != (Regval8)0x01){
        colorPrint(RED, "FAILURE");
    }
    else{
        colorPrint(GREEN, "SUCCESS");
    }
    cout << "LD (HL+), A Test" << endl;
    addr = 0xc000;
    val_8 = 0x00;
    mem.write(addr, val_8);
    val_8 = 0xc0;
    cpu.loadRegImm(H, val_8);
    val_8 = 0x00;
    cpu.loadRegImm(L, val_8);
    val_8 = 0xEE;
    cpu.loadRegImm(A, val_8);
    cpu.loadIndirectAInc();
    if(cpu.getReg(A) != mem.read(addr) ||
        cpu.getReg(H) != (Regval8)0xC0||
        cpu.getReg(L) != (Regval8)0x01){
        colorPrint(RED, "FAILURE");
    }
    else{
        colorPrint(GREEN, "SUCCESS");
    }
    cout << "===16 Bit Loads===" << endl;
    cout << "LD rr, nn Tests" << endl;
    cout << "Test 1: ";
    val_16 = 0xABCD;
    val_8 = 0x00;
    cpu.loadRegImm(B, val_8);
    cpu.loadRegImm(C, val_8);
    cpu.loadRegPairImm(B,C,val_16);
    if(cpu.getReg(B) != (Regval8)0xAB &&
        cpu.getReg(C) != (Regval8)0xCD){
        colorPrint(RED, "FAILURE");
    }
    else{
        colorPrint(GREEN, "SUCCESS");
    }
    cout << "Test 2: ";
    cpu.loadRegImm(D, val_8);
    cpu.loadRegImm(E, val_8);
    cpu.loadRegPairImm(D,E,val_16);
    if(cpu.getReg(D) != (Regval8)0xAB &&
        cpu.getReg(E) != (Regval8)0xCD){
        colorPrint(RED, "FAILURE");
    }
    else{
        colorPrint(GREEN, "SUCCESS");
    }
    cout << "Test 3: ";
    cpu.loadRegImm(H, val_8);
    cpu.loadRegImm(L, val_8);
    cpu.loadRegPairImm(H,L,val_16);
    if(cpu.getReg(H) != (Regval8)0xAB &&
        cpu.getReg(L) != (Regval8)0xCD){
        colorPrint(RED, "FAILURE");
    }
    else{
        colorPrint(GREEN, "SUCCESS");
    }
    cout << "LD (nn), SP Test" << endl;
    addr = 0xc0AB;
    cpu.loadDirectSP(addr); //SP should be initialized to 0xFFFE
    if(mem.read(addr) != 0xFE ||
        mem.read(addr + 1) != 0xFF){
        colorPrint(RED, "FAILURE");
    }
    else{
        colorPrint(GREEN, "SUCCESS");
    }
    cout << "LD SP, HL Test" << endl;
    val_8 = 0xF0; 
    cpu.loadRegImm(H, val_8);
    cpu.loadRegImm(L, val_8);
    cpu.loadSPHL();
    if(cpu.getReg(SP) != (Regval16)0xF0F0){
        colorPrint(RED, "FAILURE");
    }
    else{
        colorPrint(GREEN, "SUCCESS");
    }
    cout << "PUSH/POP rr Tests" << endl;
    cout << "Test 1: ";
    val_16 = 0xFFFE;
    cpu.loadSPImm(val_16);
    val_8 = 0xAB;
    cpu.loadRegImm(B, val_8);
    val_8 = 0xCD;
    cpu.loadRegImm(C, val_8);
    val_8 = 0xEF;
    cpu.loadRegImm(D, val_8);
    val_8 = 0x12;
    cpu.loadRegImm(E, val_8);
    val_8 = 0x34;
    cpu.loadRegImm(H, val_8);
    val_8 = 0x56;
    cpu.loadRegImm(L, val_8);
    cpu.push(B,C);
    cpu.push(D,E);
    cpu.push(H,L);
    cpu.push(B,C);
    cpu.push(H,L);
    if(cpu.getReg(SP) != (Regval16)0xFFFE - 10){
        colorPrint(RED, "FAILURE");
    }
    else{
        colorPrint(GREEN, "SUCCESS");
    }
    cout << "Test 2: ";
    cpu.pop(B,C);
    cpu.pop(H,L);
    cpu.pop(D,E);
    if(cpu.getReg(SP) != (Regval16)0xFFFE - 4 ||
        cpu.getReg(B) != (Regval8)0x34 ||
        cpu.getReg(C) != (Regval8)0x56 ||
        cpu.getReg(H) != (Regval8)0xAB ||
        cpu.getReg(L) != (Regval8)0xCD){
        colorPrint(RED, "FAILURE");
    }
    else{
        colorPrint(GREEN, "SUCCESS");
    }
    cout << "Test 3: ";
    cpu.pop(B,C);
    cpu.pop(B,C);
    try{
        cpu.pop(B,C);
    }
    catch(exception& e){
        colorPrint(GREEN, "SUCCESS");
    }
    cout << "===8 Bit Arithmetic and Logic===" << endl;
    cout << "ADD r Tests" << endl;
    cout << "Test 1: ";
    val_8 = 0x05;
    cpu.loadRegImm(A, val_8);
    val_8 = 0x06;
    cpu.loadRegImm(B, val_8);
    cpu.addReg(B);
    if(cpu.getReg(A) != (Regval8) 11 ||
        cpu.getReg(F) & CARRY_FLAG ||
        cpu.getReg(F) & HALF_CARRY_FLAG ||
        cpu.getReg(F) & ZERO_FLAG ||
        cpu.getReg(F) & SUBTRACT_FLAG){
        colorPrint(RED, "FAILURE");
    }
    else{
        colorPrint(GREEN, "SUCCESS");
    }
    cout << "Test 2: ";
    val_8 = 0x0F;
    cpu.loadRegImm(A, val_8);
    val_8 = 0x01;
    cpu.loadRegImm(C, val_8);
    cpu.addReg(C);
    if(cpu.getReg(A) != (Regval8) 0x10 ||
        cpu.getReg(F) & CARRY_FLAG ||
        !(cpu.getReg(F) & HALF_CARRY_FLAG) ||
        cpu.getReg(F) & ZERO_FLAG ||
        cpu.getReg(F) & SUBTRACT_FLAG){
        colorPrint(RED, "FAILURE");
    }
    else{
        colorPrint(GREEN, "SUCCESS");
    }
    cout << "Test 3: ";
    val_8 = 0xFF;
    cpu.loadRegImm(A, val_8);
    val_8 = 0x01;
    cpu.loadRegImm(D, val_8);
    cpu.addReg(D);
    if(cpu.getReg(A) != (Regval8) 0x00 ||
        !(cpu.getReg(F) & CARRY_FLAG) ||
        !(cpu.getReg(F) & HALF_CARRY_FLAG) ||
        !(cpu.getReg(F) & ZERO_FLAG) ||
        cpu.getReg(F) & SUBTRACT_FLAG){
        colorPrint(RED, "FAILURE");
    }
    else{
        colorPrint(GREEN, "SUCCESS");
    }
    cout << "Test 4: ";
    val_8 = 0x01;
    cpu.loadRegImm(A, val_8);
    val_8 = 0x01;
    cpu.loadRegImm(E, val_8);
    cpu.addReg(E);
    if(cpu.getReg(A) != (Regval8) 0x02 ||
        cpu.getReg(F) & CARRY_FLAG ||
        cpu.getReg(F) & HALF_CARRY_FLAG ||
        cpu.getReg(F) & ZERO_FLAG ||
        cpu.getReg(F) & SUBTRACT_FLAG){
        colorPrint(RED, "FAILURE");
    }
    else{
        colorPrint(GREEN, "SUCCESS");
    }
    cout << "ADD n Tests" << endl; 
    cout << "Test 1: ";
    val_8 = 0x05;
    cpu.loadRegImm(A, val_8);
    val_8 = 0x06;
    cpu.addImm(val_8);
    if(cpu.getReg(A) != (Regval8) 11 ||
        cpu.getReg(F) & CARRY_FLAG ||
        cpu.getReg(F) & HALF_CARRY_FLAG ||
        cpu.getReg(F) & ZERO_FLAG ||
        cpu.getReg(F) & SUBTRACT_FLAG){
        colorPrint(RED, "FAILURE");
    }
    else{
        colorPrint(GREEN, "SUCCESS");
    }
    cout << "Test 2: ";
    val_8 = 0x0F;
    cpu.loadRegImm(A, val_8);
    val_8 = 0x01;
    cpu.addImm(val_8);
    if(cpu.getReg(A) != (Regval8) 0x10 ||
        cpu.getReg(F) & CARRY_FLAG ||
        !(cpu.getReg(F) & HALF_CARRY_FLAG) ||
        cpu.getReg(F) & ZERO_FLAG ||
        cpu.getReg(F) & SUBTRACT_FLAG){
        colorPrint(RED, "FAILURE");
    }
    else{
        colorPrint(GREEN, "SUCCESS");
    }
    cout << "Test 3: ";
    val_8 = 0xFF;
    cpu.loadRegImm(A, val_8);
    val_8 = 0x01;
    cpu.addImm(val_8);
    if(cpu.getReg(A) != (Regval8) 0x00 ||
        !(cpu.getReg(F) & CARRY_FLAG) ||
        !(cpu.getReg(F) & HALF_CARRY_FLAG) ||
        !(cpu.getReg(F) & ZERO_FLAG) ||
        cpu.getReg(F) & SUBTRACT_FLAG){
        colorPrint(RED, "FAILURE");
    }
    else{
        colorPrint(GREEN, "SUCCESS");
    }
    cout << "Test 4: ";
    val_8 = 0x01;
    cpu.loadRegImm(A, val_8);
    val_8 = 0x01;
    cpu.loadRegImm(E, val_8);
    cpu.addReg(E);
    if(cpu.getReg(A) != (Regval8) 0x02 ||
        cpu.getReg(F) & CARRY_FLAG ||
        cpu.getReg(F) & HALF_CARRY_FLAG ||
        cpu.getReg(F) & ZERO_FLAG ||
        cpu.getReg(F) & SUBTRACT_FLAG){
        colorPrint(RED, "FAILURE");
    }
    else{
        colorPrint(GREEN, "SUCCESS");
    }
    cout << "ADD (HL) Tests" << endl;
    cout << "Test 1: ";
    addr = 0xC01B;
    val_8 = 0x00;
    mem.write(addr, val_8);
    cpu.loadRegImm(A, val_8);
    val_8 = 0x22;
    mem.write(addr, val_8);
    val_8 = 0xC0;
    cpu.loadRegImm(H, val_8);
    val_8 = 0x1B;
    cpu.loadRegImm(L, val_8);
    cpu.addIndirect();
    if(cpu.getReg(A) != (Regval8) 0x22 ||
        cpu.getReg(F) & CARRY_FLAG ||
        !(cpu.getReg(F) & HALF_CARRY_FLAG) ||
        cpu.getReg(F) & ZERO_FLAG ||
        cpu.getReg(F) & SUBTRACT_FLAG){
        colorPrint(RED, "FAILURE");
    }
    else{
        colorPrint(GREEN, "SUCCESS");
    }
    cout << "Test 2: ";
    addr = 0xC023;
    val_8 = 0x00;
    mem.write(addr, val_8);
    val_8 = 0x01;
    cpu.loadRegImm(A, val_8);
    val_8 = 0xFF;
    mem.write(addr, val_8);
    val_8 = 0xC0;
    cpu.loadRegImm(H, val_8);
    val_8 = 0x23;
    cpu.loadRegImm(L, val_8);
    cpu.addIndirect();
    if(cpu.getReg(A) != (Regval8) 0x00 ||
        !(cpu.getReg(F) & CARRY_FLAG) ||
        cpu.getReg(F) & HALF_CARRY_FLAG ||
        !(cpu.getReg(F) & ZERO_FLAG) ||
        cpu.getReg(F) & SUBTRACT_FLAG){
        colorPrint(RED, "FAILURE");
    }
    else{
        colorPrint(GREEN, "SUCCESS");
    }
    cout << "Test 3: ";
    addr = 0xC134;
    val_8 = 0x00;
    mem.write(addr, val_8);
    cpu.loadRegImm(A, val_8);
    val_8 = 0x01;
    mem.write(addr, val_8);
    val_8 = 0xC1;
    cpu.loadRegImm(H, val_8);
    val_8 = 0x34;
    cpu.loadRegImm(L, val_8);
    cpu.addIndirect();
    if(cpu.getReg(A) != (Regval8) 0x1 ||
        cpu.getReg(F) & CARRY_FLAG ||
        cpu.getReg(F) & HALF_CARRY_FLAG ||
        cpu.getReg(F) & ZERO_FLAG ||
        cpu.getReg(F) & SUBTRACT_FLAG){
        colorPrint(RED, "FAILURE");
    }
    else{
        colorPrint(GREEN, "SUCCESS");
    }
    cout << "ADC r Tests" << endl; 
    cout << "Test 1: ";
    val_8 = 0xFF;
    cpu.loadRegImm(A, val_8);
    val_8 = 0x01;
    cpu.addImm(val_8);
    val_8 = 0x01;
    cpu.loadRegImm(B, val_8);
    cpu.addRegCarry(B);
    if(cpu.getReg(A) != (Regval8) 0x02 ||
        cpu.getReg(F) & CARRY_FLAG ||
        cpu.getReg(F) & HALF_CARRY_FLAG ||
        cpu.getReg(F) & ZERO_FLAG ||
        cpu.getReg(F) & SUBTRACT_FLAG){
        colorPrint(RED, "FAILURE");
    }
    else{
        colorPrint(GREEN, "SUCCESS");
    }
    cout << "Test 2: ";
    val_8 = 0x0F;
    cpu.loadRegImm(A, val_8);
    val_8 = 0xFF;
    cpu.addImm(val_8);
    val_8 = 0x01;
    cpu.loadRegImm(B, val_8);
    cpu.addRegCarry(B);
    if(cpu.getReg(A) != (Regval8) 0x10 ||
        cpu.getReg(F) & CARRY_FLAG ||
        !(cpu.getReg(F) & HALF_CARRY_FLAG) ||
        cpu.getReg(F) & ZERO_FLAG ||
        cpu.getReg(F) & SUBTRACT_FLAG){
        colorPrint(RED, "FAILURE");
    }
    else{
        colorPrint(GREEN, "SUCCESS");
    }
    cout << "Test 3: ";
    val_8 = 0xFF;
    cpu.loadRegImm(A, val_8);
    val_8 = 0x01;
    cpu.addImm(val_8);
    val_8 = 0xFF;
    cpu.loadRegImm(B, val_8);
    cpu.addRegCarry(B);
    if(cpu.getReg(A) != (Regval8) 0x00 ||
        !(cpu.getReg(F) & CARRY_FLAG) ||
        cpu.getReg(F) & HALF_CARRY_FLAG ||
        !(cpu.getReg(F) & ZERO_FLAG) ||
        cpu.getReg(F) & SUBTRACT_FLAG){
        colorPrint(RED, "FAILURE");
    }
    else{
        colorPrint(GREEN, "SUCCESS");
    }
    cout << "Test 4: ";
    val_8 = 0xFF;
    cpu.loadRegImm(A, val_8);
    val_8 = 0x01;
    cpu.addImm(val_8);
    val_8 = 0x00;
    cpu.loadRegImm(B, val_8);
    cpu.addRegCarry(B);
    if(cpu.getReg(A) != (Regval8) 0x01 ||
        cpu.getReg(F) & CARRY_FLAG ||
        cpu.getReg(F) & HALF_CARRY_FLAG ||
        cpu.getReg(F) & ZERO_FLAG ||
        cpu.getReg(F) & SUBTRACT_FLAG){
        colorPrint(RED, "FAILURE");
    }
    else{
        colorPrint(GREEN, "SUCCESS");
    }
    return 0;
}