#include <stdint.h>

#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H
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
constexpr uint8_t LD_HL_IMM_IND = 0x36;
//Load A to Address in HL and Increment HL
constexpr uint8_t LD_HL_A_INC = 0x22;
//Load A to Address in HL and Decrement HL
constexpr uint8_t LD_HL_A_DEC = 0x32;
//Load Contents of Address in HL to A and Increment HL
constexpr uint8_t LD_A_HL_INC = 0x2A;
//Load Contents of Address in HL to A and Decrement HL
constexpr uint8_t LD_A_HL_DEC = 0x3A;
//Load A to = 0xFF00 + an immediate offset 
constexpr uint8_t LDH_IMM_A = 0xE0;
//Load Contents of = 0xFF00 + an immediate offset to A
constexpr uint8_t LDH_A_IMM = 0xF0;
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
constexpr uint8_t LD_DIR_SP = 0x08;
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
constexpr uint8_t INC_IND = 0x34;
//Decrement Contents of Address in HL
constexpr uint8_t DEC_IND = 0x35;
//Decimal Adjust Accummulator
constexpr uint8_t DAA = 0x27;
//Set Carry Flag, and Clear N/H Flags
constexpr uint8_t SCF = 0x37;
//Complement Accumulator, and Set N/H Flags
constexpr uint8_t CPL = 0x2F;
//Complement Carry Flag, and Clear N/H Flags
constexpr uint8_t CCF = 0x3F;
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
constexpr uint8_t ADD_IND = 0x86;
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
constexpr uint8_t ADC_IND = 0x8E;
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
constexpr uint8_t SUB_IND = 0x96;
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
constexpr uint8_t SBC_IND = 0x9E;
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
constexpr uint8_t AND_IND = 0xA6;
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
constexpr uint8_t XOR_IND = 0xAE;
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
constexpr uint8_t OR_IND = 0xB6;
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
constexpr uint8_t CP_IND = 0xBE;
//16-BIT ARITHMETIC================================================================
//Increment 16-bit Register Pair
constexpr uint8_t INC_BC = 0x03;
constexpr uint8_t INC_DE = 0x13;
constexpr uint8_t INC_HL = 0x23;
//Increment Stack Pointer
constexpr uint8_t INC_SP = 0x33;
//Decrement 16-bit Register Pair
constexpr uint8_t DEC_BC = 0x0B;
constexpr uint8_t DEC_DE = 0x1B;
constexpr uint8_t DEC_HL = 0x2B;
//Decrement Stack Pointer
constexpr uint8_t DEC_SP = 0x3B;
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
//CB OPS=======================================================
constexpr uint8_t CB_OP = 0xCB;
constexpr uint8_t RLC_B = 0x00;
constexpr uint8_t RL_B = 0x10;
constexpr uint8_t SLA_B = 0x20;
constexpr uint8_t SWAP_B = 0x30;
constexpr uint8_t BIT_0_B = 0x40;
constexpr uint8_t BIT_2_B = 0x50;
constexpr uint8_t BIT_4_B = 0x60;
constexpr uint8_t BIT_6_B = 0x70;
constexpr uint8_t RES_0_B = 0x80;
constexpr uint8_t RES_2_B = 0x90;
constexpr uint8_t RES_4_B = 0xA0;
constexpr uint8_t RES_6_B = 0xB0;
constexpr uint8_t SET_0_B = 0xC0;
constexpr uint8_t SET_2_B = 0xD0;
constexpr uint8_t SET_4_B = 0xE0;
constexpr uint8_t SET_6_B = 0xF0;
constexpr uint8_t RLC_C = 0x01;
constexpr uint8_t RL_C = 0x11;
constexpr uint8_t SLA_C = 0x21;
constexpr uint8_t SWAP_C = 0x31;
constexpr uint8_t BIT_0_C = 0x41;
constexpr uint8_t BIT_2_C = 0x51;
constexpr uint8_t BIT_4_C = 0x61;
constexpr uint8_t BIT_6_C = 0x71;
constexpr uint8_t RES_0_C = 0x81;
constexpr uint8_t RES_2_C = 0x91;
constexpr uint8_t RES_4_C = 0xA1;
constexpr uint8_t RES_6_C = 0xB1;
constexpr uint8_t SET_0_C = 0xC1;
constexpr uint8_t SET_2_C = 0xD1;
constexpr uint8_t SET_4_C = 0xE1;
constexpr uint8_t SET_6_C = 0xF1;
constexpr uint8_t RLC_D = 0x02;
constexpr uint8_t RL_D = 0x12;
constexpr uint8_t SLA_D = 0x22;
constexpr uint8_t SWAP_D = 0x32;
constexpr uint8_t BIT_0_D = 0x42;
constexpr uint8_t BIT_2_D = 0x52;
constexpr uint8_t BIT_4_D = 0x62;
constexpr uint8_t BIT_6_D = 0x72;
constexpr uint8_t RES_0_D = 0x82;
constexpr uint8_t RES_2_D = 0x92;
constexpr uint8_t RES_4_D = 0xA2;
constexpr uint8_t RES_6_D = 0xB2;
constexpr uint8_t SET_0_D = 0xC2;
constexpr uint8_t SET_2_D = 0xD2;
constexpr uint8_t SET_4_D = 0xE2;
constexpr uint8_t SET_6_D = 0xF2;
constexpr uint8_t RLC_E = 0x03;
constexpr uint8_t RL_E = 0x13;
constexpr uint8_t SLA_E = 0x23;
constexpr uint8_t SWAP_E = 0x33;
constexpr uint8_t BIT_0_E = 0x43;
constexpr uint8_t BIT_2_E = 0x53;
constexpr uint8_t BIT_4_E = 0x63;
constexpr uint8_t BIT_6_E = 0x73;
constexpr uint8_t RES_0_E = 0x83;
constexpr uint8_t RES_2_E = 0x93;
constexpr uint8_t RES_4_E = 0xA3;
constexpr uint8_t RES_6_E = 0xB3;
constexpr uint8_t SET_0_E = 0xC3;
constexpr uint8_t SET_2_E = 0xD3;
constexpr uint8_t SET_4_E = 0xE3;
constexpr uint8_t SET_6_E = 0xF3;
constexpr uint8_t RLC_H = 0x04;
constexpr uint8_t RL_H = 0x14;
constexpr uint8_t SLA_H = 0x24;
constexpr uint8_t SWAP_H = 0x34;
constexpr uint8_t BIT_0_H = 0x44;
constexpr uint8_t BIT_2_H = 0x54;
constexpr uint8_t BIT_4_H = 0x64;
constexpr uint8_t BIT_6_H = 0x74;
constexpr uint8_t RES_0_H = 0x84;
constexpr uint8_t RES_2_H = 0x94;
constexpr uint8_t RES_4_H = 0xA4;
constexpr uint8_t RES_6_H = 0xB4;
constexpr uint8_t SET_0_H = 0xC4;
constexpr uint8_t SET_2_H = 0xD4;
constexpr uint8_t SET_4_H = 0xE4;
constexpr uint8_t SET_6_H = 0xF4;
constexpr uint8_t RLC_L = 0x05;
constexpr uint8_t RL_L = 0x15;
constexpr uint8_t SLA_L = 0x25;
constexpr uint8_t SWAP_L = 0x35;
constexpr uint8_t BIT_0_L = 0x45;
constexpr uint8_t BIT_2_L = 0x55;
constexpr uint8_t BIT_4_L = 0x65;
constexpr uint8_t BIT_6_L = 0x75;
constexpr uint8_t RES_0_L = 0x85;
constexpr uint8_t RES_2_L = 0x95;
constexpr uint8_t RES_4_L = 0xA5;
constexpr uint8_t RES_6_L = 0xB5;
constexpr uint8_t SET_0_L = 0xC5;
constexpr uint8_t SET_2_L = 0xD5;
constexpr uint8_t SET_4_L = 0xE5;
constexpr uint8_t SET_6_L = 0xF5;
constexpr uint8_t RLC_IND = 0x06;
constexpr uint8_t RL_IND = 0x16;
constexpr uint8_t SLA_IND = 0x26;
constexpr uint8_t SWAP_IND = 0x36;
constexpr uint8_t BIT_0_IND = 0x46;
constexpr uint8_t BIT_2_IND = 0x56;
constexpr uint8_t BIT_4_IND = 0x66;
constexpr uint8_t BIT_6_IND = 0x76;
constexpr uint8_t RES_0_IND = 0x86;
constexpr uint8_t RES_2_IND = 0x96;
constexpr uint8_t RES_4_IND = 0xA6;
constexpr uint8_t RES_6_IND = 0xB6;
constexpr uint8_t SET_0_IND = 0xC6;
constexpr uint8_t SET_2_IND = 0xD6;
constexpr uint8_t SET_4_IND = 0xE6;
constexpr uint8_t SET_6_IND = 0xF6;
constexpr uint8_t RLC_A = 0x07;
constexpr uint8_t RL_A = 0x17;
constexpr uint8_t SLA_A = 0x27;
constexpr uint8_t SWAP_A = 0x37;
constexpr uint8_t BIT_0_A = 0x47;
constexpr uint8_t BIT_2_A = 0x57;
constexpr uint8_t BIT_4_A = 0x67;
constexpr uint8_t BIT_6_A = 0x77;
constexpr uint8_t RES_0_A = 0x87;
constexpr uint8_t RES_2_A = 0x97;
constexpr uint8_t RES_4_A = 0xA7;
constexpr uint8_t RES_6_A = 0xB7;
constexpr uint8_t SET_0_A = 0xC7;
constexpr uint8_t SET_2_A = 0xD7;
constexpr uint8_t SET_4_A = 0xE7;
constexpr uint8_t SET_6_A = 0xF7;
constexpr uint8_t RRC_B = 0x08;
constexpr uint8_t RR_B = 0x18;
constexpr uint8_t SRA_B = 0x28;
constexpr uint8_t SRL_B = 0x38;
constexpr uint8_t BIT_1_B = 0x48;
constexpr uint8_t BIT_3_B = 0x58;
constexpr uint8_t BIT_5_B = 0x68;
constexpr uint8_t BIT_7_B = 0x78;
constexpr uint8_t RES_1_B = 0x88;
constexpr uint8_t RES_3_B = 0x98;
constexpr uint8_t RES_5_B = 0xA8;
constexpr uint8_t RES_7_B = 0xB8;
constexpr uint8_t SET_1_B = 0xC8;
constexpr uint8_t SET_3_B = 0xD8;
constexpr uint8_t SET_5_B = 0xE8;
constexpr uint8_t SET_7_B = 0xF8;
constexpr uint8_t RRC_C = 0x09;
constexpr uint8_t RR_C = 0x19;
constexpr uint8_t SRA_C = 0x29;
constexpr uint8_t SRL_C = 0x39;
constexpr uint8_t BIT_1_C = 0x49;
constexpr uint8_t BIT_3_C = 0x59;
constexpr uint8_t BIT_5_C = 0x69;
constexpr uint8_t BIT_7_C = 0x79;
constexpr uint8_t RES_1_C = 0x89;
constexpr uint8_t RES_3_C = 0x99;
constexpr uint8_t RES_5_C = 0xA9;
constexpr uint8_t RES_7_C = 0xB9;
constexpr uint8_t SET_1_C = 0xC9;
constexpr uint8_t SET_3_C = 0xD9;
constexpr uint8_t SET_5_C = 0xE9;
constexpr uint8_t SET_7_C = 0xF9;
constexpr uint8_t RRC_D = 0x0A;
constexpr uint8_t RR_D = 0x1A;
constexpr uint8_t SRA_D = 0x2A;
constexpr uint8_t SRL_D = 0x3A;
constexpr uint8_t BIT_1_D = 0x4A;
constexpr uint8_t BIT_3_D = 0x5A;
constexpr uint8_t BIT_5_D = 0x6A;
constexpr uint8_t BIT_7_D = 0x7A;
constexpr uint8_t RES_1_D = 0x8A;
constexpr uint8_t RES_3_D = 0x9A;
constexpr uint8_t RES_5_D = 0xAA;
constexpr uint8_t RES_7_D = 0xBA;
constexpr uint8_t SET_1_D = 0xCA;
constexpr uint8_t SET_3_D = 0xDA;
constexpr uint8_t SET_5_D = 0xEA;
constexpr uint8_t SET_7_D = 0xFA;
constexpr uint8_t RRC_E = 0x0B;
constexpr uint8_t RR_E = 0x1B;
constexpr uint8_t SRA_E = 0x2B;
constexpr uint8_t SRL_E = 0x3B;
constexpr uint8_t BIT_1_E = 0x4B;
constexpr uint8_t BIT_3_E = 0x5B;
constexpr uint8_t BIT_5_E = 0x6B;
constexpr uint8_t BIT_7_E = 0x7B;
constexpr uint8_t RES_1_E = 0x8B;
constexpr uint8_t RES_3_E = 0x9B;
constexpr uint8_t RES_5_E = 0xAB;
constexpr uint8_t RES_7_E = 0xBB;
constexpr uint8_t SET_1_E = 0xCB;
constexpr uint8_t SET_3_E = 0xDB;
constexpr uint8_t SET_5_E = 0xEB;
constexpr uint8_t SET_7_E = 0xFB;
constexpr uint8_t RRC_H = 0x0C;
constexpr uint8_t RR_H = 0x1C;
constexpr uint8_t SRA_H = 0x2C;
constexpr uint8_t SRL_H = 0x3C;
constexpr uint8_t BIT_1_H = 0x4C;
constexpr uint8_t BIT_3_H = 0x5C;
constexpr uint8_t BIT_5_H = 0x6C;
constexpr uint8_t BIT_7_H = 0x7C;
constexpr uint8_t RES_1_H = 0x8C;
constexpr uint8_t RES_3_H = 0x9C;
constexpr uint8_t RES_5_H = 0xAC;
constexpr uint8_t RES_7_H = 0xBC;
constexpr uint8_t SET_1_H = 0xCC;
constexpr uint8_t SET_3_H = 0xDC;
constexpr uint8_t SET_5_H = 0xEC;
constexpr uint8_t SET_7_H = 0xFC;
constexpr uint8_t RRC_L = 0x0D;
constexpr uint8_t RR_L = 0x1D;
constexpr uint8_t SRA_L = 0x2D;
constexpr uint8_t SRL_L = 0x3D;
constexpr uint8_t BIT_1_L = 0x4D;
constexpr uint8_t BIT_3_L = 0x5D;
constexpr uint8_t BIT_5_L = 0x6D;
constexpr uint8_t BIT_7_L = 0x7D;
constexpr uint8_t RES_1_L = 0x8D;
constexpr uint8_t RES_3_L = 0x9D;
constexpr uint8_t RES_5_L = 0xAD;
constexpr uint8_t RES_7_L = 0xBD;
constexpr uint8_t SET_1_L = 0xCD;
constexpr uint8_t SET_3_L = 0xDD;
constexpr uint8_t SET_5_L = 0xED;
constexpr uint8_t SET_7_L = 0xFD;
constexpr uint8_t RRC_IND = 0x0E;
constexpr uint8_t RR_IND = 0x1E;
constexpr uint8_t SRA_IND = 0x2E;
constexpr uint8_t SRL_IND = 0x3E;
constexpr uint8_t BIT_1_IND = 0x4E;
constexpr uint8_t BIT_3_IND = 0x5E;
constexpr uint8_t BIT_5_IND = 0x6E;
constexpr uint8_t BIT_7_IND = 0x7E;
constexpr uint8_t RES_1_IND = 0x8E;
constexpr uint8_t RES_3_IND = 0x9E;
constexpr uint8_t RES_5_IND = 0xAE;
constexpr uint8_t RES_7_IND = 0xBE;
constexpr uint8_t SET_1_IND = 0xCE;
constexpr uint8_t SET_3_IND = 0xDE;
constexpr uint8_t SET_5_IND = 0xEE;
constexpr uint8_t SET_7_IND = 0xFE;
constexpr uint8_t RRC_A = 0x0F;
constexpr uint8_t RR_A = 0x1F;
constexpr uint8_t SRA_A = 0x2F;
constexpr uint8_t SRL_A = 0x3F;
constexpr uint8_t BIT_1_A = 0x4F;
constexpr uint8_t BIT_3_A = 0x5F;
constexpr uint8_t BIT_5_A = 0x6F;
constexpr uint8_t BIT_7_A = 0x7F;
constexpr uint8_t RES_1_A = 0x8F;
constexpr uint8_t RES_3_A = 0x9F;
constexpr uint8_t RES_5_A = 0xAF;
constexpr uint8_t RES_7_A = 0xBF;
constexpr uint8_t SET_1_A = 0xCF;
constexpr uint8_t SET_3_A = 0xDF;
constexpr uint8_t SET_5_A = 0xEF;
constexpr uint8_t SET_7_A = 0xFF;

static const std::array<std::string, 256> opcode_names = {
    "NOP",
    "LD BC,nn",
    "LD (BC),A",
    "INC BC",
    "INC B",
    "DEC B",
    "LD B,n",
    "RLCA",
    "LD (nn),SP",
    "ADD HL,BC",
    "LD A,(BC)",
    "DEC BC",
    "INC C",
    "DEC C",
    "LD C,n",
    "RRCA",

    "STOP",
    "LD DE,nn",
    "LD (DE),A",
    "INC DE",
    "INC D",
    "DEC D",
    "LD D,n",
    "RLA",
    "JR n",
    "ADD HL,DE",
    "LD A,(DE)",
    "DEC DE",
    "INC E",
    "DEC E",
    "LD E,n",
    "RRA",

    "JR NZ,n",
    "LD HL,nn",
    "LD (HL+),A",
    "INC HL",
    "INC H",
    "DEC H",
    "LD H,n",
    "DAA",
    "JR Z,n",
    "ADD HL,HL",
    "LD A,(HLI)",
    "DEC HL",
    "INC L",
    "DEC L",
    "LD L,n",
    "CPL",

    "JR NC,n",
    "LD SP,nn",
    "LD (HL-),A",
    "INC SP",
    "INC (HL)",
    "DEC (HL)",
    "LD (HL),n",
    "SCF",
    "JR C,n",
    "ADD HL,SP",
    "LD A,(HLD)",
    "DEC SP",
    "INC A",
    "DEC A",
    "LDA,n",
    "CCF",

    "LD B,B",
    "LD B,C",
    "LD B,D",
    "LD B,E",
    "LD B,H",
    "LD B,L",
    "LD B,(HL)",
    "LD B,A",
    "LD C,B",
    "LD C,C",
    "LD C,D",
    "LD C,E",
    "LD C,H",
    "LD C,L",
    "LD C,(HL)",
    "LD C,A",

    "LD D,B",
    "LD D,C",
    "LD D,D",
    "LD D,E",
    "LD D,H",
    "LD D,L",
    "LD D,(HL)",
    "LD D,A",
    "LD E,B",
    "LD E,C",
    "LD E,D",
    "LD E,E",
    "LD E,H",
    "LD E,L",
    "LD E,(HL)",
    "LD E,A",

    "LD H,B",
    "LD H,C",
    "LD H,D",
    "LD H,E",
    "LD H,H",
    "LD H,L",
    "LD H,(HL)",
    "LD H,A",
    "LD L,B",
    "LD L,C",
    "LD L,D",
    "LD L,E",
    "LD L,H",
    "LD L,L",
    "LD L,(HL)",
    "LD L,A",

    "LD (HL),B",
    "LD (HL),C",
    "LD (HL),D",
    "LD (HL),E",
    "LD (HL),H",
    "LD (HL),L",
    "HALT",
    "LD (HL),A",
    "LD A,B",
    "LD A,C",
    "LD A,D",
    "LD A,E",
    "LD A,H",
    "LD A,L",
    "LD A,(HL)",
    "LD A,A",

    "ADD A,B",
    "ADD A,C",
    "ADD A,D",
    "ADD A,E",
    "ADD A,H",
    "ADD A,L",
    "ADD A,(HL)",
    "ADD A,A",
    "ADC A,B",
    "ADC A,C",
    "ADC A,D",
    "ADC A,E",
    "ADC A,H",
    "ADC A,L",
    "ADC A,(HL)",
    "ADC A,A",

    "SUB B",
    "SUB C",
    "SUB D",
    "SUB E",
    "SUB H",
    "SUB L",
    "SUB (HL)",
    "SUB A",
    "SBC A,B",
    "SBC A,C",
    "SBC A,D",
    "SBC A,E",
    "SBC A,H",
    "SBC A,L",
    "SBC A,(HL)",
    "SBC A,A",

    "AND B",
    "AND C",
    "AND D",
    "AND E",
    "AND H",
    "AND L",
    "AND (HL)",
    "AND A",
    "XOR B",
    "XOR C",
    "XOR D",
    "XOR E",
    "XOR H",
    "XOR L",
    "XOR (HL)",
    "XOR A",

    "OR B",
    "OR C",
    "OR D",
    "OR E",
    "OR H",
    "OR L",
    "OR (HL)",
    "OR A",
    "CP B",
    "CP C",
    "CP D",
    "CP E",
    "CP H",
    "CP L",
    "CP (HL)",
    "CP A",

    "RET NZ",
    "POP BC",
    "JP NZ,nn",
    "JP nn",
    "CALL NZ,nn",
    "PUSH BC",
    "ADD A,n",
    "RST ",
    "RET Z",
    "RET",
    "JP Z,nn",
    "cb opcode",
    "CALL Z,nn",
    "CALL nn",
    "ADC A,n",
    "RST 0x08",

    "RET NC",
    "POP DE",
    "JP NC,nn",
    "unused opcode",
    "CALL NC,nn",
    "PUSH DE",
    "SUB n",
    "RST 0x10",
    "RET C",
    "RETI",
    "JP C,nn",
    "unused opcode",
    "CALL C,nn",
    "unused opcode",
    "SBC A,n",
    "RST 0x18",

    "LD (0xFF00+n),A",
    "POP HL",
    "LD (0xFF00+C),A",
    "unused opcode",
    "unused opcode",
    "PUSH HL",
    "AND n",
    "RST 0x20",
    "ADD SP,n",
    "JP (HL)",
    "LD (nn),A",
    "unused opcode",
    "unused opcode",
    "unused opcode",
    "XOR n",
    "RST 0x28",

    "LD A,(0xFF00+n)",
    "POP AF",
    "LD A,(0xFF00+C)",
    "DI",
    "unused opcode",
    "PUSH AF",
    "OR n",
    "RST 0x30",
    "LD HL,SP",
    "LD SP,HL",
    "LD A,(nn)",
    "EI",
    "unused opcode",
    "unused opcode",
    "CP n",
    "RST 0x38"
};

static const std::array<std::string, 256> opcode_cb_names = {
    "RLC B",
    "RLC C",
    "RLC D",
    "RLC E",
    "RLC H",
    "RLC L",
    "RLC (HL)",
    "RLC A",
    "RRC B",
    "RRC C",
    "RRC D",
    "RRC E",
    "RRC H",
    "RRC L",
    "RRC (HL)",
    "RRC A",

    "RL B",
    "RL C",
    "RL D",
    "RL E",
    "RL H",
    "RL L ",
    "RL (HL)",
    "RL A",
    "RR B",
    "RR C",
    "RR D",
    "RR E",
    "RR H",
    "RR L",
    "RR (HL)",
    "RR A",

    "SLA B",
    "SLA C",
    "SLA D",
    "SLA E",
    "SLA H",
    "SLA L",
    "SLA (HL)",
    "SLA A",
    "SRA B",
    "SRA C",
    "SRA D",
    "SRA E",
    "SRA H",
    "SRA L",
    "SRA (HL)",
    "SRA A",

    "SWAP B",
    "SWAP C",
    "SWAP D",
    "SWAP E",
    "SWAP H",
    "SWAP L",
    "SWAP (HL)",
    "SWAP A",
    "SRL B",
    "SRL C",
    "SRL D",
    "SRL E",
    "SRL H",
    "SRL L",
    "SRL (HL)",
    "SRL A",

    "BIT 0 B",
    "BIT 0 C",
    "BIT 0 D",
    "BIT 0 E",
    "BIT 0 H",
    "BIT 0 L",
    "BIT 0 (HL)",
    "BIT 0 A",
    "BIT 1 B",
    "BIT 1 C",
    "BIT 1 D",
    "BIT 1 E",
    "BIT 1 H",
    "BIT 1 L",
    "BIT 1 (HL)",
    "BIT 1 A",

    "BIT 2 B",
    "BIT 2 C",
    "BIT 2 D",
    "BIT 2 E",
    "BIT 2 H",
    "BIT 2 L",
    "BIT 2 (HL)",
    "BIT 2 A",
    "BIT 3 B",
    "BIT 3 C",
    "BIT 3 D",
    "BIT 3 E",
    "BIT 3 H",
    "BIT 3 L",
    "BIT 3 (HL)",
    "BIT 3 A",

    "BIT 4 B",
    "BIT 4 C",
    "BIT 4 D",
    "BIT 4 E",
    "BIT 4 H",
    "BIT 4 L",
    "BIT 4 (HL)",
    "BIT 4 A",
    "BIT 5 B",
    "BIT 5 C",
    "BIT 5 D",
    "BIT 5 E",
    "BIT 5 H",
    "BIT 5 L",
    "BIT 5 (HL)",
    "BIT 5 A",

    "BIT 6 B",
    "BIT 6 C",
    "BIT 6 D",
    "BIT 6 E",
    "BIT 6 H",
    "BIT 6 L",
    "BIT 6 (HL)",
    "BIT 6 A",
    "BIT 7 B",
    "BIT 7 C",
    "BIT 7 D",
    "BIT 7 E",
    "BIT 7 H",
    "BIT 7 L",
    "BIT 7 (HL)",
    "BIT 7 A",

    "RES 0 B",
    "RES 0 C",
    "RES 0 D",
    "RES 0 E",
    "RES 0 H",
    "RES 0 L",
    "RES 0 (HL)",
    "RES 0 A",
    "RES 1 B",
    "RES 1 C",
    "RES 1 D",
    "RES 1 E",
    "RES 1 H",
    "RES 1 L",
    "RES 1 (HL)",
    "RES 1 A",

    "RES 2 B",
    "RES 2 C",
    "RES 2 D",
    "RES 2 E",
    "RES 2 H",
    "RES 2 L",
    "RES 2 (HL)",
    "RES 2 A",
    "RES 3 B",
    "RES 3 C",
    "RES 3 D",
    "RES 3 E",
    "RES 3 H",
    "RES 3 L",
    "RES 3 (HL)",
    "RES 3 A",

    "RES 4 B",
    "RES 4 C",
    "RES 4 D",
    "RES 4 E",
    "RES 4 H",
    "RES 4 L",
    "RES 4 (HL)",
    "RES 4 A",
    "RES 5 B",
    "RES 5 C",
    "RES 5 D",
    "RES 5 E",
    "RES 5 H",
    "RES 5 L",
    "RES 5 (HL)",
    "RES 5 A",

    "RES 6 B",
    "RES 6 C",
    "RES 6 D",
    "RES 6 E",
    "RES 6 H",
    "RES 6 L",
    "RES 6 (HL)",
    "RES 6 A",
    "RES 7 B",
    "RES 7 C",
    "RES 7 D",
    "RES 7 E",
    "RES 7 H",
    "RES 7 L",
    "RES 7 (HL)",
    "RES 7 A",

    "SET 0 B",
    "SET 0 C",
    "SET 0 D",
    "SET 0 E",
    "SET 0 H",
    "SET 0 L",
    "SET 0 (HL)",
    "SET 0 A",
    "SET 1 B",
    "SET 1 C",
    "SET 1 D",
    "SET 1 E",
    "SET 1 H",
    "SET 1 L",
    "SET 1 (HL)",
    "SET 1 A",

    "SET 2 B",
    "SET 2 C",
    "SET 2 D",
    "SET 2 E",
    "SET 2 H",
    "SET 2 L",
    "SET 2 (HL)",
    "SET 2 A",
    "SET 3 B",
    "SET 3 C",
    "SET 3 D",
    "SET 3 E",
    "SET 3 H",
    "SET 3 L",
    "SET 3 (HL)",
    "SET 3 A",

    "SET 4 B",
    "SET 4 C",
    "SET 4 D",
    "SET 4 E",
    "SET 4 H",
    "SET 4 L",
    "SET 4 (HL)",
    "SET 4 A",
    "SET 5 B",
    "SET 5 C",
    "SET 5 D",
    "SET 5 E",
    "SET 5 H",
    "SET 5 L",
    "SET 5 (HL)",
    "SET 5 A",

    "SET 6 B",
    "SET 6 C",
    "SET 6 D",
    "SET 6 E",
    "SET 6 H",
    "SET 6 L",
    "SET 6 (HL)",
    "SET 6 A",
    "SET 7 B",
    "SET 7 C",
    "SET 7 D",
    "SET 7 E",
    "SET 7 H",
    "SET 7 L",
    "SET 7 (HL)",
    "SET 7 A"
};
#endif