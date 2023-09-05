#ifndef MEMORY_H
#define MEMORY_H

#include <cstdint>
#include <cstddef>

typedef uint8_t RegVal_8;
typedef uint16_t RegVal_16;

//I/O register addresses
constexpr RegVal_16 SB = 0xFF01;
constexpr RegVal_16 SC = 0xFF02;
constexpr RegVal_16 LCDC = 0xFF40;
constexpr RegVal_16 STAT = 0xFF41;
constexpr RegVal_16 SCY = 0xFF42;
constexpr RegVal_16 SCX = 0xFF43;
constexpr RegVal_16 LY = 0xFF44;


class Memory{
    private:
        RegVal_8 mem[UINT16_MAX + 1];
    public:
    /**
     * @brief writes a byte to specified address in memory.
     * 
     * @param addr address of interest
     * 
     * @param byte data to be written
     * 
     * @return contents of address after write
     */
        RegVal_8 write(RegVal_16 addr, RegVal_8 byte);
    /**
     * @brief reads a byte from specified address in memory.
     * 
     * @param addr address of interest
     * 
     * @return byte at address
     */
        RegVal_8 read(RegVal_16 addr);
    /**
     * @brief reads a byte from specified address in memory.
     * 
     * @param addr starting point of dump
     * 
     * @param n number of bytes to dump
     * 
     * @return number of bytes written 
     */
        RegVal_16 dump(RegVal_16 addr, size_t n, RegVal_8* buf);
};
#endif