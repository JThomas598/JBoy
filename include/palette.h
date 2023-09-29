#include "memory.h"
#include <array>

typedef enum PaletteIndex{
    COLOR_0,
    COLOR_1,
    COLOR_2,
    COLOR_3,
}PaletteIndex;

typedef enum PaletteSelect{
    BGP,
    OBP0,
    OBP1
}PaletteSelect;


//GB Colors
constexpr uint32_t ARGB_BLACK = 0xFF000000;
constexpr uint32_t ARGB_DARK_GRAY = 0xFF545454;
constexpr uint32_t ARGB_LIGHT_GRAY = 0xFFA9A9A9;
constexpr uint32_t ARGB_WHITE = 0xFFFFFFFF;

class Palette{
    private:
        static constexpr Regval8 COLOR_0_MASK = 0x03;
        static constexpr Regval8 COLOR_1_MASK = 0x0C;
        static constexpr Regval8 COLOR_2_MASK = 0x30;
        static constexpr Regval8 COLOR_3_MASK = 0xC0;

        std::array<uint32_t, 4>bgp;
        std::array<uint32_t, 4>obp0;
        std::array<uint32_t, 4>obp1;

        Memory mem;
        Register bgpReg;
        Register obp0Reg;
        Register obp1Reg;

        uint32_t convertColor(Regval8 color);
    public:
        Palette();
        uint32_t getColor(PaletteSelect select, PaletteIndex index);
};