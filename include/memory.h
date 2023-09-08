#ifndef MEMORY_H
#define MEMORY_H

#include <cstdint>
#include <cstddef>
#include <array>

typedef uint8_t RegVal_8;
typedef uint16_t RegVal_16;

//Write Permissions
typedef enum Perm{
    CPU_PERM,
    PPU_PERM,
    DMA_PERM,
    SYS_PERM
}Permission;

//Access Types
typedef enum Access{
    READ,
    WRITE
}Access;

//Key Memory Map Boundaries
constexpr RegVal_16 ROM_BANK_0_START = 0x0000;
constexpr RegVal_16 ROM_BANK_0_END = 0x3FFF;
constexpr RegVal_16 ROM_BANK_1_START = 0x4000;
constexpr RegVal_16 ROM_BANK_1_END = 0x7FFF;
constexpr RegVal_16 ECHO_START = 0xE000;
constexpr RegVal_16 ECHO_END = 0xFDFF;
constexpr RegVal_16 BAD_ZONE_START = 0xFEA0;
constexpr RegVal_16 BAD_ZONE_END = 0xFEFF;
constexpr RegVal_16 OAM_START = 0xFE00;
constexpr RegVal_16 OAM_END = 0xFE9F;
constexpr RegVal_16 HRAM_START = 0xFF80;
constexpr RegVal_16 HRAM_END = 0xFFFE;
constexpr RegVal_16 VRAM_START = 0x8000;
constexpr RegVal_16 VRAM_END = 0x9FFF;

//I/O register addresses
constexpr RegVal_16 SB = 0xFF01;
constexpr RegVal_16 SC = 0xFF02;
constexpr RegVal_16 LCDC = 0xFF40;
constexpr RegVal_16 STAT = 0xFF41;
constexpr RegVal_16 SCY = 0xFF42;
constexpr RegVal_16 SCX = 0xFF43;
constexpr RegVal_16 LY = 0xFF44;
constexpr RegVal_16 IE = 0xFFFF;
constexpr RegVal_16 IF = 0xFF0F;

//Interrupt Service Routine Addresses
constexpr RegVal_16 VBLANK_ISR = 0x40; 
constexpr RegVal_16 LCD_STAT_ISR = 0x48; 
constexpr RegVal_16 TIMER_ISR = 0x50;
constexpr RegVal_16 SERIAL_ISR = 0x58;
constexpr RegVal_16 JOYPAD_ISR = 0x60;

//Interrupt Masks
constexpr RegVal_8 VBLANK_INT = 0x01;
constexpr RegVal_8 LCD_STAT_INT = 0x02;
constexpr RegVal_8 TIMER_INT = 0x04;
constexpr RegVal_8 SERIAL_INT = 0x08;
constexpr RegVal_8 JOYPAD_INT = 0x10;


class Memory{
    private:
        static std::array<RegVal_8,UINT16_MAX+1> mem;
        static bool ppuLock;
        static bool dmaLock;
        const Permission perm;

        bool inRange(const RegVal_16 addr, const RegVal_16 low, const RegVal_16 hi) const;
        bool checkPerm(const RegVal_16 addr, Access acc) const;
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
        bool write(const RegVal_16 addr, const RegVal_8 byte) const;
    /**
     * @brief returns a reference to a specified address in memory.
     * 
     * @param addr address of interest
     * 
     * @return byte at address
     */
        RegVal_8 read(const RegVal_16 addr) const;
    /**
     * @brief dumps n bytes from buf, starting from addr in memory
     * 
     * @param addr starting point of dump
     * 
     * @param n number of bytes to dump
     * 
     * @return number of bytes written 
     */
        RegVal_16 dump(const RegVal_16 addr, const RegVal_8* buf, const size_t n) const;
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
        bool unlockVram();
};
#endif