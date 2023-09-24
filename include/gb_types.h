#ifndef GB_TYPES_H
#define GB_TYPES_H
#include <cstdint>

typedef uint8_t Regval8;
typedef uint16_t Regval16;

typedef Regval8& Register;

//Interrupt Masks
constexpr Regval8 VBLANK_INT = 0x01;
constexpr Regval8 LCD_STAT_INT = 0x02;
constexpr Regval8 TIMER_INT = 0x04;
constexpr Regval8 SERIAL_INT = 0x08;
constexpr Regval8 JOYPAD_INT = 0x10;
#endif