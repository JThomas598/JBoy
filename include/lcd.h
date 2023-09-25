#ifndef LCD_H
#define LCD_H
#include <cstdint>
#include <memory.h>

constexpr int SCREEN_WIDTH = 160, SCREEN_HEIGHT = 144;
constexpr int SCAN_WIDTH = 210, SCAN_HEIGHT = 154;


//LCDC MASKS
typedef enum ControlBitIndex{
    LCDC_BG_WIN_EN,
    LCDC_OBJ_EN,
    LCDC_OBJ_SIZE,
    LCDC_BG_MAP_SEL,
    LCDC_BG_WIN_DATA_SEL,
    LCDC_WIN_EN,
    LCDC_WIN_MAP_SEL,
    LCDC_LCD_EN
}ControlBitIndex;

#endif