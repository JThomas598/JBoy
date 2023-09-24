#ifndef LCD_H
#define LCD_H
#include <cstdint>
#include <memory.h>

constexpr int SCREEN_WIDTH = 160, SCREEN_HEIGHT = 144;
constexpr int SCAN_WIDTH = 210, SCAN_HEIGHT = 154;

//GB Colors
constexpr uint32_t ARGB_BLACK = 0xFF000000;
constexpr uint32_t ARGB_DARK_GRAY = 0xFF545454;
constexpr uint32_t ARGB_LIGHT_GRAY = 0xFFA9A9A9;
constexpr uint32_t ARGB_WHITE = 0xFFFFFFFF;

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

typedef enum GBColor{
    WHITE,
    LIGHT_GRAY,
    DARK_GRAY,
    BLACK,
}GBColor;
#endif