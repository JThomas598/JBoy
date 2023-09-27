#ifndef MEMORY_H
#define MEMORY_H

#include <cstdint>
#include <cstddef>
#include <array>
#include "gb_types.h"

//Write Permissions
typedef enum Perm{
    CPU_PERM,
    PPU_PERM,
    DMA_PERM,
    SYS_PERM,
    COUNTER_PERM,
    OAM_PERM
}Permission;

//Access Types
typedef enum Access{
    READ,
    WRITE
}Access;

//Key Memory Map Boundaries
constexpr Regval16 EXT_RAM_ENABLE_START = 0x0000;
constexpr Regval16 EXT_RAM_ENABLE_END = 0x1FFF;

constexpr Regval16 ROM_BANK_SELECT_START = 0x2000;
constexpr Regval16 ROM_BANK_SELECT_END = 0x3FFF;

constexpr Regval16 RAM_BANK_SELECT_START = 0x4000;
constexpr Regval16 RAM_BANK_SELECT_END = 0x5FFF;

constexpr Regval16 BANKING_MODE_SELECT_START = 0x6000;
constexpr Regval16 BANKING_MODE_SELECT_END = 0x7FFF;

constexpr Regval16 RAM_ENABLE_START = 0x0000;
constexpr Regval16 RAM_ENABLE_END = 0x1FFF;

constexpr Regval16 ROM_BANK_N_START = 0x4000;
constexpr Regval16 ROM_BANK_N_END = 0x7FFF;

constexpr Regval16 EXT_RAM_START = 0xA000;
constexpr Regval16 EXT_RAM_END = 0xBFFF;

constexpr Regval16 ECHO_START = 0xE000;
constexpr Regval16 ECHO_END = 0xFDFF;

constexpr Regval16 BAD_ZONE_START = 0xFEA0;
constexpr Regval16 BAD_ZONE_END = 0xFEFF;

constexpr Regval16 OAM_START = 0xFE00;
constexpr Regval16 OAM_END = 0xFE9F;

constexpr Regval16 HRAM_START = 0xFF80;
constexpr Regval16 HRAM_END = 0xFFFE;

constexpr Regval16 VRAM_START = 0x8000;
constexpr Regval16 VRAM_END = 0x9FFF;

//I/O register addresses
constexpr Regval16 SB = 0xFF01;
constexpr Regval16 SC = 0xFF02;
constexpr Regval16 LCDC_REG_ADDR = 0xFF40;
constexpr Regval16 STAT_REG_ADDR = 0xFF41;
constexpr Regval16 SCY_REG_ADDR = 0xFF42;
constexpr Regval16 SCX_REG_ADDR = 0xFF43;
constexpr Regval16 WINX_REG_ADDR = 0xFF4B;
constexpr Regval16 WINY_REG_ADDR = 0xFF4A;
constexpr Regval16 LY_REG_ADDR = 0xFF44;
constexpr Regval16 LYC_REG_ADDR = 0xFF45;
constexpr Regval16 IE_REG_ADDR = 0xFFFF;
constexpr Regval16 JOYP_REG_ADDR = 0xFF00;
constexpr Regval16 IF_REG_ADDR = 0xFF0F;
constexpr Regval16 DIV_REG_ADDR = 0xFF04;
constexpr Regval16 TIMA_REG_ADDR = 0xFF05;
constexpr Regval16 TMA_REG_ADDR = 0xFF06;
constexpr Regval16 TAC_REG_ADDR = 0xFF07;

//DMA Reg
constexpr Regval16 DMA_REG = 0xFF46;

//Palette Regs
constexpr Regval16 BGP_REG_ADDR = 0xFF47;
constexpr Regval16 OBP0_REG_ADDR = 0xFF48;
constexpr Regval16 OBP1_REG_ADDR = 0xFF49;



//Tilemap/Tiledata Locations
constexpr Regval16 TILE_MAP_ADDR_1 = 0x9800;
constexpr Regval16 TILE_MAP_ADDR_2 = 0x9C00;
constexpr Regval16 TILE_DATA_ADDR_1 = 0x8000;
constexpr Regval16 TILE_DATA_ADDR_2 = 0x9000;


//Interrupt Service Routine Addresses
constexpr Regval16 VBLANK_ISR_ADDR = 0x40; 
constexpr Regval16 LCD_STAT_ISR_ADDR = 0x48; 
constexpr Regval16 TIMER_ISR_ADDR = 0x50;
constexpr Regval16 SERIAL_ISR_ADDR = 0x58;
constexpr Regval16 JOYPAD_ISR_ADDR = 0x60;

//Joypad Masks
constexpr Regval8 A_BUTTON_MASK = 0x01;
constexpr Regval8 B_BUTTON_MASK = 0x02;
constexpr Regval8 SELECT_BUTTON_MASK = 0x04;
constexpr Regval8 START_BUTTON_MASK = 0x08;
constexpr Regval8 ALL_BUTTON_MASK = 0x0F;

constexpr Regval8 RIGHT_PAD_MASK = 0x10;
constexpr Regval8 LEFT_PAD_MASK = 0x20;
constexpr Regval8 UP_PAD_MASK = 0x40;
constexpr Regval8 DOWN_PAD_MASK = 0x80;
constexpr Regval8 ALL_PAD_MASK = 0xF0;

constexpr int EXT_RAM_BANK_SIZE = 8192;
constexpr int ROM_BANK_SIZE = 16384;
constexpr int MBC1_NUM_RAM_BANKS = 4;
constexpr int MBC1_NUM_ROM_BANKS = 128;

class Memory{
    private:
        static std::array<Regval8,UINT16_MAX+1> mem;
        static std::array<std::array<Regval8, EXT_RAM_BANK_SIZE>, MBC1_NUM_RAM_BANKS> ramBanks;
        static std::array<std::array<Regval8, ROM_BANK_SIZE>, MBC1_NUM_ROM_BANKS> romBanks;
        static Regval8 currRamBank;
        static Regval8 currRomBank;
        static bool ramEnabled;
        static bool ppuLock;
        static bool dmaLock;
        static bool vramAltered;
        static Regval8 joypadBuff;
        const Permission perm;

        bool inRange(const Regval16 addr, const Regval16 low, const Regval16 hi) const;
        bool checkPerm(const Regval16 addr, Access acc) const;
        void prepJoypadRead(const Regval8 byte) const;
    public:
    Memory(Permission perm);
    /**
     * @brief writes a byte to specified address in memory.
     * 
     * @param addr address of interest
     * 
     * @param byte data to be written
     * 
     * @return true if the write was successful, false if it wasn't
     */
        bool write(const Regval16 addr, const Regval8 byte) const;
    /**
     * @brief returns a reference to a specified address in memory.
     * 
     * @param addr address of interest
     * 
     * @return byte at address
     */
        Regval8 read(const Regval16 addr) const;
    /**
     * @brief dumps n bytes from buf, starting from addr in memory
     * 
     * @param addr starting point of dump
     * 
     * @param n number of bytes to dump
     * 
     * @return number of bytes written 
     */
        Regval16 dump(const Regval16 addr, const Regval8* buf, const size_t n) const;
    /**
     * @brief dumps n bytes from buf, starting from addr in memory
     * 
     * @param addr starting point of dump
     * 
     * @param n number of bytes to dump
     * 
     * @return number of bytes written 
     */
        Regval16 copyRomBank(const Regval8* buf, const int bankNum);
    /**
     * @brief forbids all but HRAM accesses for the CPU. Only Memory instances with the DMA perm can do this.
     * 
     * @return true if lock is successful, false if it isn't.
     */
        bool lockMemoryDMA();
    /**
     * @brief allows CPU to access memory other than HRAM. Only Memory instances with the DMA perm can do this.
     * 
     * @return true if lock is successful, false if it isn't.
     */
        bool unlockMemoryDMA();
    /**
     * @brief forbids VRAM accesses for the CPU. Only Memory instances with the PPU perm can use this.
     * 
     * @return true if lock is successful, false if it isn't.
     */
        bool lockVram();
    /**
     * @brief allows VRAM accesses for the CPU. Only Memory instances with the PPU perm can use this.
     * 
     * @return true if lock is successful, false if it isn't.
     */
        
        void setJoypadBuff(Regval8 byte);
        Regval8 getJoypadBuff();
    /**
     * @brief Gives caller a reference to a location in memory. Should only be used by modules to gain
     * quick access to their respective I/O registers.
     * 
     * @return reference to requested memory location
     */
        Register getRegister(Regval16 addr);
        bool unlockVram();
        bool vramChange();
        void resetChange();
        void printStatus();
};
#endif