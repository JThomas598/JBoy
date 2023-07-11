#include <stdint.h>

//MISCELLANEOUS===========================================================
constexpr uint8_t NOP = 0x00;
constexpr uint8_t STOP = 0x10;
constexpr uint8_t HALT = 0x76;
constexpr uint8_t DI = 0xF3;
constexpr uint8_t EI = 0xFB;
//8-BIT LOADS=============================================================
//Load General Purpose Register to B
constexpr uint8_t LD_B_B = 0x40;
constexpr uint8_t LD_B_C = 0x41;
constexpr uint8_t LD_B_D = 0x42;
constexpr uint8_t LD_B_E = 0x43;
constexpr uint8_t LD_B_H = 0x44;
constexpr uint8_t LD_B_L = 0x45;
constexpr uint8_t LD_B_A = 0x47;
//Load General Purpose Register to C
constexpr uint8_t LD_C_B = 0x48;
constexpr uint8_t LD_C_C = 0x49;
constexpr uint8_t LD_C_D = 0x4A;
constexpr uint8_t LD_C_E = 0x4B;
constexpr uint8_t LD_C_H = 0x4C;
constexpr uint8_t LD_C_L = 0x4D;
constexpr uint8_t LD_C_A = 0x4F;
//Load General Purpose Register to D
constexpr uint8_t LD_D_B = 0x50;
constexpr uint8_t LD_D_C = 0x51;
constexpr uint8_t LD_D_D = 0x52;
constexpr uint8_t LD_D_E = 0x53;
constexpr uint8_t LD_D_H = 0x54;
constexpr uint8_t LD_D_L = 0x55;
constexpr uint8_t LD_D_A = 0x57;
//Load General Purpose Register to E
constexpr uint8_t LD_E_B = 0x58;
constexpr uint8_t LD_E_C = 0x59;
constexpr uint8_t LD_E_D = 0x5A;
constexpr uint8_t LD_E_E = 0x5B;
constexpr uint8_t LD_E_H = 0x5C;
constexpr uint8_t LD_E_L = 0x5D;
constexpr uint8_t LD_E_A = 0x5F;
//Load General Purpose Register to H
constexpr uint8_t LD_H_B = 0x60;
constexpr uint8_t LD_H_C = 0x61;
constexpr uint8_t LD_H_D = 0x62;
constexpr uint8_t LD_H_E = 0x63;
constexpr uint8_t LD_H_H = 0x64;
constexpr uint8_t LD_H_L = 0x65;
constexpr uint8_t LD_H_A = 0x67;
//Load General Purpose Register to L
constexpr uint8_t LD_L_B = 0x68;
constexpr uint8_t LD_L_C = 0x69;
constexpr uint8_t LD_L_D = 0x6A;
constexpr uint8_t LD_L_E = 0x6B;
constexpr uint8_t LD_L_H = 0x6C;
constexpr uint8_t LD_L_L = 0x6D;
constexpr uint8_t LD_L_A = 0x6F;
//Load General Purpose Register to A
constexpr uint8_t LD_A_B = 0x78;
constexpr uint8_t LD_A_C = 0x79;
constexpr uint8_t LD_A_D = 0x7A;
constexpr uint8_t LD_A_E = 0x7B;
constexpr uint8_t LD_A_H = 0x7C;
constexpr uint8_t LD_A_L = 0x7D;
constexpr uint8_t LD_A_A = 0x7F;
//Load Immediate to General Purpose Register
constexpr uint8_t LD_B_IMM = 0x06;
constexpr uint8_t LD_D_IMM = 0x16;
constexpr uint8_t LD_H_IMM = 0x26;
constexpr uint8_t LD_C_IMM = 0x0E;
constexpr uint8_t LD_E_IMM = 0x1E;
constexpr uint8_t LD_L_IMM = 0x2E;
constexpr uint8_t LD_A_IMM = 0x3E;
//Load A to Address in 16-bit Register Pair
constexpr uint8_t LD_BC_A = 0x02;
constexpr uint8_t LD_DE_A = 0x12;
//Load Contents of Address in 16-bit Register Pair to A
constexpr uint8_t LD_A_BC = 0x0A;
constexpr uint8_t LD_A_DE = 0x1A;
//Load General Purpose Register to Address in HL
constexpr uint8_t LD_HL_B = 0x70;
constexpr uint8_t LD_HL_C = 0x71;
constexpr uint8_t LD_HL_D = 0x72;
constexpr uint8_t LD_HL_E = 0x73;
constexpr uint8_t LD_HL_H = 0x74;
constexpr uint8_t LD_HL_L = 0x75;
constexpr uint8_t LD_HL_A = 0x77;
//Load Contents of Address in HL to General Purpose Register
constexpr uint8_t LD_B_HL = 0x46;
constexpr uint8_t LD_D_HL = 0x56;
constexpr uint8_t LD_H_HL = 0x66;
constexpr uint8_t LD_C_HL = 0x4E;
constexpr uint8_t LD_E_HL = 0x5E;
constexpr uint8_t LD_L_HL = 0x6E;
constexpr uint8_t LD_A_HL = 0x7E;
//Load Immediate to Address in HL
constexpr uint8_t LD__HL_IMM_IND = 0x36;
//Load A to Address in HL and Increment HL
constexpr uint8_t LD_HL_A_INC = 0x22;
//Load A to Address in HL and Decrement HL
constexpr uint8_t LD_HL_A_DEC = 0x32;
//Load Contents of Address in HL to A and Increment HL
constexpr uint8_t LD_A_HL_INC = 0x2A;
//Load Contents of Address in HL to A and Decrement HL
constexpr uint8_t LD_A_HL_DEC = 0x3A;
//Load A to = 0xFF00 + an immediate offset 
constexpr uint8_t LDH_OFFSET_A = 0xE0;
//Load Contents of = 0xFF00 + an immediate offset to A
constexpr uint8_t LDH_A_OFFSET = 0xF0;
//Load A to = 0xFF00 + an offset stored in C
constexpr uint8_t LDH_C_A = 0xE2;
//Load Contents of = 0xFF00 + an offset stored in C to A
constexpr uint8_t LDH_A_C = 0xF2;
//Load A to Immediate Address
constexpr uint8_t LD_ADDR_A = 0xEA;
//Load Contents of Immediate Address to A
constexpr uint8_t LD_A_ADDR = 0xFA;
//16-BIT LOADS=================================================================
//Load Immediate Value to 16-bit Register Pair
constexpr uint8_t LD_BC_IMM = 0x01;
constexpr uint8_t LD_DE_IMM = 0x11;
constexpr uint8_t LD_HL_IMM = 0x21;
//Load Immediate Value to Stack Pointer
constexpr uint8_t LD_SP_IMM = 0x31;
//Pop Stack Value to 16-bit Register Pair
constexpr uint8_t POP_BC = 0xC1;
constexpr uint8_t POP_DE = 0xD1;
constexpr uint8_t POP_HL = 0xE1;
constexpr uint8_t POP_AF = 0xF1;
//Push Value from 16-bit Register Pair to Stack
constexpr uint8_t PUSH_BC = 0xC5;
constexpr uint8_t PUSH_DE = 0xD5;
constexpr uint8_t PUSH_HL = 0xE5;
constexpr uint8_t PUSH_AF = 0xF5;
//Load Stack Pointer Value to Immediate Address
constexpr uint8_t LD_ADDR_SP = 0x08;
//Load HL to Stack Pointer
constexpr uint8_t LD_SP_HL = 0xF9;
//Load Stack Pointer Value Plus Immediate to HL
constexpr uint8_t LD_HL_SP_OFFSET = 0xF8;
//8-BIT ARITHMETIC/LOGIC=======================================================
//Increment 8-Bit Register
constexpr uint8_t INC_B = 0x04;
constexpr uint8_t INC_D = 0x14;
constexpr uint8_t INC_H = 0x24;
constexpr uint8_t INC_C = 0x0C;
constexpr uint8_t INC_E = 0x1C;
constexpr uint8_t INC_L = 0x2C;
constexpr uint8_t INC_A = 0x3C;
//Decrement 8-Bit Register
constexpr uint8_t DEC_B = 0x05;
constexpr uint8_t DEC_D = 0x15;
constexpr uint8_t DEC_H = 0x25;
constexpr uint8_t DEC_C = 0x0D;
constexpr uint8_t DEC_E = 0x1D;
constexpr uint8_t DEC_L = 0x2D;
constexpr uint8_t DEC_A = 0x3D;
//Increment Contents of Address in HL
constexpr uint8_t INC_HL_IND = 0x34;
//Decrement Contents of Address in HL
constexpr uint8_t DEC_HL_IND = 0x35;
//Decimal Adjust Accummulator
constexpr uint8_t DAA = 0x27;
//Set Carry Flag, and Clear N/H Flags
constexpr uint8_t SCF = 0x37;
//Complement Accumulator, and Set N/H Flags
constexpr uint8_t CPL = 0x2F;
//Complement Carry Flag, and Clear N/H Flags
constexpr uint8_t CCF = 0x2F;
//Add General Purpose Register to A
constexpr uint8_t ADD_B = 0x80;
constexpr uint8_t ADD_C = 0x81;
constexpr uint8_t ADD_D = 0x82;
constexpr uint8_t ADD_E = 0x83;
constexpr uint8_t ADD_H = 0x84;
constexpr uint8_t ADD_L = 0x85;
constexpr uint8_t ADD_A = 0x87;
//Add Immediate to A
constexpr uint8_t ADD_IMM = 0xC6;
//Add Contents of Address in HL to A
constexpr uint8_t ADD_HL = 0x86;
//Add General Purpose Register and Carry Flag to A
constexpr uint8_t ADC_B = 0x88;
constexpr uint8_t ADC_C = 0x89;
constexpr uint8_t ADC_D = 0x8A;
constexpr uint8_t ADC_E = 0x8B;
constexpr uint8_t ADC_H = 0x8C;
constexpr uint8_t ADC_L = 0x8D;
constexpr uint8_t ADC_A = 0x8F;
//Add Immediate and Carry Flag to A
constexpr uint8_t ADC_IMM = 0xCE;
//Add Contents of Address in HL and Carry Flag to A
constexpr uint8_t ADC_HL = 0x8E;
//Subtract General Purpose Register from A
constexpr uint8_t SUB_B = 0x90;
constexpr uint8_t SUB_C = 0x91;
constexpr uint8_t SUB_D = 0x92;
constexpr uint8_t SUB_E = 0x93;
constexpr uint8_t SUB_H = 0x94;
constexpr uint8_t SUB_L = 0x95;
constexpr uint8_t SUB_A = 0x97;
//Subtract Immediate from A
constexpr uint8_t SUB_IMM = 0xD6;
//Subtract Contents of Address in HL from A
constexpr uint8_t SUB_HL = 0x96;
//Subract General Purpose Register and Carry Flag From A
constexpr uint8_t SBC_B = 0x98;
constexpr uint8_t SBC_C = 0x99;
constexpr uint8_t SBC_D = 0x9A;
constexpr uint8_t SBC_E = 0x9B;
constexpr uint8_t SBC_H = 0x9C;
constexpr uint8_t SBC_L = 0x9D;
constexpr uint8_t SBC_A = 0x9F;
//Subtract Immediate and Carry Flag from A
constexpr uint8_t SBC_IMM = 0xDE;
//Subtract Contents of Address in HL and Carry Flag from A
constexpr uint8_t SBC_HL = 0x9E;
//Perform Bitwise AND on A with General Purpose Register
constexpr uint8_t AND_B = 0xA0;
constexpr uint8_t AND_C = 0xA1;
constexpr uint8_t AND_D = 0xA2;
constexpr uint8_t AND_E = 0xA3;
constexpr uint8_t AND_H = 0xA4;
constexpr uint8_t AND_L = 0xA5;
constexpr uint8_t AND_A = 0xA7;
//Perform Bitwise AND on A with Immediate
constexpr uint8_t AND_IMM = 0xE6;
//Perform Bitwise AND on A with Contents of Address in HL 
constexpr uint8_t AND_HL = 0xA6;
//Perform Bitwise XOR on A with General Purpose Register
constexpr uint8_t XOR_B = 0xA8;
constexpr uint8_t XOR_C = 0xA9;
constexpr uint8_t XOR_D = 0xAA;
constexpr uint8_t XOR_E = 0xAB;
constexpr uint8_t XOR_H = 0xAC;
constexpr uint8_t XOR_L = 0xAD;
constexpr uint8_t XOR_A = 0xAF;
//Perform Bitwise XOR on A with Immediate
constexpr uint8_t XOR_IMM = 0xEE;
//Perform Bitwise XOR on A with Contents of Address in HL 
constexpr uint8_t XOR_HL = 0xAE;
//Perform Bitwise OR on A with General Purpose Register
constexpr uint8_t OR_B = 0xB0;
constexpr uint8_t OR_C = 0xB1;
constexpr uint8_t OR_D = 0xB2;
constexpr uint8_t OR_E = 0xB3;
constexpr uint8_t OR_H = 0xB4;
constexpr uint8_t OR_L = 0xB5;
constexpr uint8_t OR_A = 0xB7;
//Perform Bitwise OR on A with Immediate
constexpr uint8_t OR_IMM = 0xF6;
//Perform Bitwise AND on A with Contents of Address in HL 
constexpr uint8_t OR_HL = 0xB6;
//Compare General Purpose Register and A (Same as subtract but doesn't modify A). Use flags to check comparison.
constexpr uint8_t CP_B = 0xB8;
constexpr uint8_t CP_C = 0xB9;
constexpr uint8_t CP_D = 0xBA;
constexpr uint8_t CP_E = 0xBB;
constexpr uint8_t CP_H = 0xBC;
constexpr uint8_t CP_L = 0xBD;
constexpr uint8_t CP_A = 0xBF;
//Compare Immediate and A (Same as subtract but doesn't modify A). Use flags to check comparison.
constexpr uint8_t CP_IMM = 0xFE;
//Compare Contents of Address in HL and A (Same as subtract but doesn't modify A). Use flags to check comparison.
constexpr uint8_t CP_HL = 0xBE;
//16-BIT ARITHMETIC================================================================
//Increment 16-bit Register Pair
constexpr uint8_t INC_BC = 0x03;
constexpr uint8_t INC_DE = 0x13;
constexpr uint8_t INC_HL = 0x23;
//Increment Stack Pointer
constexpr uint8_t INC_SP = 0x33;
//Decrement 16-bit Register Pair
constexpr uint8_t DEC_BC = 0x08;
constexpr uint8_t DEC_DE = 0x18;
constexpr uint8_t DEC_HL = 0x28;
//Decrement Stack Pointer
constexpr uint8_t DEC_SP = 0x38;
//Add 16-bit Register Pair to HL
constexpr uint8_t ADD_HL_BC = 0x09;
constexpr uint8_t ADD_HL_DE = 0x19;
constexpr uint8_t ADD_HL_HL = 0x29;
//Add Stack Pointer to HL
constexpr uint8_t ADD_HL_SP = 0x39;
//Add Immediate to Stack Pointer
constexpr uint8_t ADD_SP_IMM = 0xE8;
//Rotate A Right Through Carry
constexpr uint8_t RRCA = 0x0F;
//Rotate A Right 
constexpr uint8_t RRA = 0x1F;
//Rotate A Left Through Carry
constexpr uint8_t RLCA = 0x07;
//Rotate A Left
constexpr uint8_t RLA = 0x17;
//CB (Extended Operation Prefix)
constexpr uint8_t CB_OP = 0xCB;
//CONTROL FLOW==================================================================
//Relative Jump by Offset
constexpr uint8_t JR = 0x18;
//Relative Jump if Zero Flag High
constexpr uint8_t JR_Z = 0x28;
//Relative Jump if Zero Flag Low
constexpr uint8_t JR_NZ = 0x20;
//Relative Jump if Carry Flag High
constexpr uint8_t JR_C = 0x38;
//Relative Jump if Carry Flag Low
constexpr uint8_t JR_NC = 0x30;
//Return if Zero Flag Low
constexpr uint8_t RET_NZ = 0xC0;
//Return if Carry Flag Low
constexpr uint8_t RET_NC = 0xD0;
//Jump to Immediate Address if Zero Flag Low
constexpr uint8_t JP_NZ = 0xC2;
//Jump to Immediate Address if Carry Flag Low
constexpr uint8_t JP_NC = 0xD2;
//Unconditional Jump to Immediate Address
constexpr uint8_t JP = 0xC3;    
//Call Function at Immediate Address if Zero Flag Low
constexpr uint8_t CALL_NZ = 0xC4;
//Call function if Carry Flag Low
constexpr uint8_t CALL_NC = 0xD4;
//Unconditional Function Call to = 0x00
constexpr uint8_t RST_0x00 = 0xC7;
//Unconditional Function Call to = 0x10
constexpr uint8_t RST_0x10 = 0xD7;
//Unconditional Function Call to = 0x20
constexpr uint8_t RST_0x20 = 0xE7;
//Unconditional Function Call to = 0x30
constexpr uint8_t RST_0x30 = 0xF7;
//Return if Zero Flag High
constexpr uint8_t RET_Z = 0xC8;
//Return if Carry Flag High
constexpr uint8_t RET_C = 0xD8;
//Unconditional Return 
constexpr uint8_t RET = 0xC9;
//Return from interrupt handler
constexpr uint8_t RETI = 0xD9;
//Unconditional Jump to Address in HL
constexpr uint8_t JP_HL = 0xE9;
//Jump to Immediate Address if Zero Flag High
constexpr uint8_t JP_Z = 0xCA;
//Jump to Immediate Address if Carry Flag High
constexpr uint8_t JP_C = 0xDA;
//Call Function at Immediate Address if Zero Flag High
constexpr uint8_t CALL_Z = 0xCC;
//Call Function at Immediate Address if Carry Flag High
constexpr uint8_t CALL_C = 0xDC;
//Unconditional Function Call to Immediate Address
constexpr uint8_t CALL = 0xCD;
//Unconditional Function Call to = 0x08
constexpr uint8_t RST_0x08 = 0xCF;
//Unconditional Function Call to 0x18
constexpr uint8_t RST_0x18 = 0xDF;
//Unconditional Function Call to 0x28
constexpr uint8_t RST_0x28 = 0xEF;
//Unconditional Function Call to 0x38
constexpr uint8_t RST_0x38 = 0xFF;