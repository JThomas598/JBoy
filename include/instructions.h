//MISCELLANEOUS===========================================================
#define NOP 0x00
#define STOP 0x10
#define HALT 0x76
#define DI 0xF3
#define EI 0xFB
//8-BIT LOADS=============================================================
//Load General Purpose Register to B
#define LD_B_B 0x40
#define LD_B_C 0x41
#define LD_B_D 0x42
#define LD_B_E 0x43
#define LD_B_H 0x44
#define LD_B_L 0x45
#define LD_B_A 0x47
//Load General Purpose Register to C
#define LD_C_B 0x48
#define LD_C_C 0x49
#define LD_C_D 0x4A
#define LD_C_E 0x4B
#define LD_C_H 0x4C
#define LD_C_L 0x4D
#define LD_C_A 0x4F
//Load General Purpose Register to D
#define LD_D_B 0x50
#define LD_D_C 0x51
#define LD_D_D 0x52
#define LD_D_E 0x53
#define LD_D_H 0x54
#define LD_D_L 0x55
#define LD_D_A 0x57
//Load General Purpose Register to E
#define LD_E_B 0x58
#define LD_E_C 0x59
#define LD_E_D 0x5A
#define LD_E_E 0x5B
#define LD_E_H 0x5C
#define LD_E_L 0x5D
#define LD_E_A 0x5F
//Load General Purpose Register to H
#define LD_H_B 0x60
#define LD_H_C 0x61
#define LD_H_D 0x62
#define LD_H_E 0x63
#define LD_H_H 0x64
#define LD_H_L 0x65
#define LD_H_A 0x67
//Load General Purpose Register to L
#define LD_L_B 0x68
#define LD_L_C 0x69
#define LD_L_D 0x6A
#define LD_L_E 0x6B
#define LD_L_H 0x6C
#define LD_L_L 0x6D
#define LD_L_A 0x6F
//Load General Purpose Register to A
#define LD_A_B 0x78
#define LD_A_C 0x79
#define LD_A_D 0x7A
#define LD_A_E 0x7B
#define LD_A_H 0x7C
#define LD_A_L 0x7D
#define LD_A_A 0x7F
//Load Immediate to General Purpose Register
#define LD_B_IMM 0x06
#define LD_D_IMM 0x16
#define LD_H_IMM 0x26
#define LD_C_IMM 0x0E
#define LD_E_IMM 0x1E
#define LD_L_IMM 0x2E
#define LD_A_IMM 0x3E
//Load A to Address in 16-bit Register Pair
#define LD_BC_A 0x02
#define LD_DE_A 0x12
//Load Contents of Address in 16-bit Register Pair to A
#define LD_A_BC 0x0A
#define LD_A_DE 0x1A
//Load General Purpose Register to Address in HL
#define LD_HL_B 0x70
#define LD_HL_C 0x71
#define LD_HL_D 0x72
#define LD_HL_E 0x73
#define LD_HL_H 0x74
#define LD_HL_L 0x75
#define LD_HL_A 0x77
//Load Contents of Address in HL to General Purpose Register
#define LD_B_HL 0x46
#define LD_D_HL 0x56
#define LD_H_HL 0x66
#define LD_C_HL 0x4E
#define LD_E_HL 0x5E
#define LD_L_HL 0x6E
#define LD_A_HL 0x7E
//Load Immediate to Address in HL
#define LD__HL_IMM_IND 0x36
//Load A to Address in HL and Increment HL
#define LD_HL_A_INC 0x22
//Load A to Address in HL and Decrement HL
#define LD_HL_A_DEC 0x32
//Load Contents of Address in HL to A and Increment HL
#define LD_A_HL_INC 0x2A
//Load Contents of Address in HL to A and Decrement HL
#define LD_A_HL_DEC 0x3A
//Load A to 0xFF00 + an immediate offset 
#define LDH_OFFSET_A 0xE0
//Load Contents of 0xFF00 + an immediate offset to A
#define LDH_A_OFFSET 0xF0
//Load A to 0xFF00 + an offset stored in C
#define LDH_C_A 0xE2
//Load Contents of 0xFF00 + an offset stored in C to A
#define LDH_A_C 0xF2
//Load A to Immediate Address
#define LD_ADDR_A 0xEA 
//Load Contents of Immediate Address to A
#define LD_A_ADDR 0xFA
//16-BIT LOADS=================================================================
//Load Immediate Value to 16-bit Register Pair
#define LD_BC_IMM 0x01
#define LD_DE_IMM 0x11
#define LD_HL_IMM 0x21
//Load Immediate Value to Stack Pointer
#define LD_SP_IMM 0x31
//Pop Stack Value to 16-bit Register Pair
#define POP_BC 0xC1
#define POP_DE 0xD1
#define POP_HL 0xE1
#define POP_AF 0xF1
//Push Value from 16-bit Register Pair to Stack
#define PUSH_BC 0xC5
#define PUSH_DE 0xD5
#define PUSH_HL 0xE5
#define PUSH_AF 0xF5
//Load Stack Pointer Value to Immediate Address
#define LD_ADDR_SP 0x08
//Load HL to Stack Pointer
#define LD_SP_HL 0xF9
//Load Stack Pointer Value Plus Immediate to HL
#define LD_HL_SP_OFFSET 0xF8
//8-BIT ARITHMETIC/LOGIC=======================================================
//Increment 8-Bit Register
#define INC_B 0x04
#define INC_D 0x14
#define INC_H 0x24
#define INC_C 0x0C
#define INC_E 0x1C
#define INC_L 0x2C
#define INC_A 0x3C
//Decrement 8-Bit Register
#define DEC_B 0x05
#define DEC_D 0x15
#define DEC_H 0x25
#define DEC_C 0x0D
#define DEC_E 0x1D
#define DEC_L 0x2D
#define DEC_A 0x3D
//Increment Contents of Address in HL
#define INC_HL_IND 0x34
//Decrement Contents of Address in HL
#define DEC_HL_IND 0x35
//Decimal Adjust Accummulator
#define DAA 0x27
//Set Carry Flag, and Clear N/H Flags
#define SCF 0x37
//Complement Accumulator, and Set N/H Flags
#define CPL 0x2F
//Complement Carry Flag, and Clear N/H Flags
#define CCF 0x2F
//Add General Purpose Register to A
#define ADD_B 0x80
#define ADD_C 0x81
#define ADD_D 0x82
#define ADD_E 0x83
#define ADD_H 0x84
#define ADD_L 0x85
#define ADD_A 0x87
//Add Immediate to A
#define ADD_IMM 0xC6
//Add Contents of Address in HL to A
#define ADD_HL 0x86
//Add General Purpose Register and Carry Flag to A
#define ADC_B 0x88
#define ADC_C 0x89
#define ADC_D 0x8A
#define ADC_E 0x8B
#define ADC_H 0x8C
#define ADC_L 0x8D
#define ADC_A 0x8F
//Add Immediate and Carry Flag to A
#define ADC_IMM 0xCE
//Add Contents of Address in HL and Carry Flag to A
#define ADC_HL 0x8E
//Subtract General Purpose Register from A
#define SUB_B 0x90
#define SUB_C 0x91
#define SUB_D 0x92
#define SUB_E 0x93
#define SUB_H 0x94
#define SUB_L 0x95
#define SUB_A 0x97
//Subtract Immediate from A
#define SUB_IMM 0xD6
//Subtract Contents of Address in HL from A
#define SUB_HL 0x96
//Subract General Purpose Register and Carry Flag From A
#define SBC_B 0x98
#define SBC_C 0x99
#define SBC_D 0x9A
#define SBC_E 0x9B
#define SBC_H 0x9C
#define SBC_L 0x9D
#define SBC_A 0x9F
//Subtract Immediate and Carry Flag from A
#define SBC_IMM 0xDE
//Subtract Contents of Address in HL and Carry Flag from A
#define SBC_HL 0x9E
//Perform Bitwise AND on A with General Purpose Register
#define AND_B 0xA0
#define AND_C 0xA1
#define AND_D 0xA2
#define AND_E 0xA3
#define AND_H 0xA4
#define AND_L 0xA5
#define AND_A 0xA7
//Perform Bitwise AND on A with Immediate
#define AND_IMM 0xE6
//Perform Bitwise AND on A with Contents of Address in HL 
#define AND_HL 0xA6
//Perform Bitwise XOR on A with General Purpose Register
#define XOR_B 0xA8
#define XOR_C 0xA9
#define XOR_D 0xAA
#define XOR_E 0xAB
#define XOR_H 0xAC
#define XOR_L 0xAD
#define XOR_A 0xAF
//Perform Bitwise XOR on A with Immediate
#define XOR_IMM 0xEE
//Perform Bitwise XOR on A with Contents of Address in HL 
#define XOR_HL 0xAE
//Perform Bitwise OR on A with General Purpose Register
#define OR_B 0xB0
#define OR_C 0xB1
#define OR_D 0xB2
#define OR_E 0xB3
#define OR_H 0xB4
#define OR_L 0xB5
#define OR_A 0xB7
//Perform Bitwise OR on A with Immediate
#define OR_IMM 0xF6
//Perform Bitwise AND on A with Contents of Address in HL 
#define OR_HL 0xB6
//Compare General Purpose Register and A (Same as subtract but doesn't modify A). Use flags to check comparison.
#define CP_B 0xB8
#define CP_C 0xB9
#define CP_D 0xBA
#define CP_E 0xBB
#define CP_H 0xBC
#define CP_L 0xBD
#define CP_A 0xBF
//Compare Immediate and A (Same as subtract but doesn't modify A). Use flags to check comparison.
#define CP_IMM 0xFE
//Compare Contents of Address in HL and A (Same as subtract but doesn't modify A). Use flags to check comparison.
#define CP_HL 0xBE
//16-BIT ARITHMETIC================================================================
//Increment 16-bit Register Pair
#define INC_BC 0x03
#define INC_DE 0x13
#define INC_HL 0x23
//Increment Stack Pointer
#define INC_SP 0x33
//Decrement 16-bit Register Pair
#define DEC_BC 0x08
#define DEC_DE 0x18
#define DEC_HL 0x28
//Decrement Stack Pointer
#define DEC_SP 0x38
//Add 16-bit Register Pair to HL
#define ADD_HL_BC 0x09
#define ADD_HL_DE 0x19
#define ADD_HL_HL 0x29
//Add Stack Pointer to HL
#define ADD_HL_SP 0x39
//Add Immediate to Stack Pointer
#define ADD_SP_IMM 0xE8
//Rotate A Right Through Carry
#define RRCA 0x0F
//Rotate A Right 
#define RRA 0x1F
//Rotate A Left Through Carry
#define RLCA 0x07
//Rotate A Left
#define RLA 0x17
//CB (Extended Operation Prefix)
#define CB_OP 0xCB
//CONTROL FLOW==================================================================
//Relative Jump by Offset
#define JR 0x18
//Relative Jump if Zero Flag High
#define JR_Z 0x28
//Relative Jump if Zero Flag Low
#define JR_NZ 0x20
//Relative Jump if Carry Flag High
#define JR_C 0x38
//Relative Jump if Carry Flag Low
#define JR_NC 0x30
//Return if Zero Flag Low
#define RET_NZ 0xC0
//Return if Carry Flag Low
#define RET_NC 0xD0
//Jump to Immediate Address if Zero Flag Low
#define JP_NZ 0xC2
//Jump to Immediate Address if Carry Flag Low
#define JP_NC 0xD2
//Unconditional Jump to Immediate Address
#define JP 0xC3
//Call Function at Immediate Address if Zero Flag Low
#define CALL_NZ 0xC4
//Call function if Carry Flag Low
#define CALL_NC 0xD4
//Unconditional Function Call to 0x00
#define RST_0x00 0xC7
//Unconditional Function Call to 0x10
#define RST_0x10 0xD7
//Unconditional Function Call to 0x20
#define RST_0x20 0xE7
//Unconditional Function Call to 0x30
#define RST_0x30 0xF7
//Return if Zero Flag High
#define RET_Z 0xC8
//Return if Carry Flag High
#define RET_C 0xD8
//Unconditional Return 
#define RET 0xC9
//Return from interrupt handler
#define RETI 0xD9
//Unconditional Jump to Address in HL
#define JP_HL 0xE9
//Jump to Immediate Address if Zero Flag High
#define JP_Z 0xCA 
//Jump to Immediate Address if Carry Flag High
#define JP_C 0xDA
//Call Function at Immediate Address if Zero Flag High
#define CALL_Z 0xCC
//Call Function at Immediate Address if Carry Flag High
#define CALL_C 0xDC
//Unconditional Function Call to Immediate Address
#define CALL 0xCD
//Unconditional Function Call to 0x08
#define RST_0x08 0xCF
//Unconditional Function Call to 0x18
#define RST_0x18 0xDF
//Unconditional Function Call to 0x28
#define RST_0x28 0xEF
//Unconditional Function Call to 0x38
#define RST_0x38 0xFF