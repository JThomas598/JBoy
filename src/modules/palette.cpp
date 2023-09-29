#include "palette.h"
#include <stdexcept>



Palette::Palette() :
    mem(PPU_PERM),
    bgpReg(mem.getRegister(BGP_REG_ADDR)),
    obp0Reg(mem.getRegister(OBP0_REG_ADDR)),
    obp1Reg(mem.getRegister(OBP1_REG_ADDR))
{
    bgpReg = 0xFC;
    obp0Reg = 0x00;
    obp1Reg = 0x00;
}

uint32_t Palette::convertColor(Regval8 color){
    uint32_t retval;
    switch(color){
        case 0x00:
            retval = ARGB_WHITE;
            break;
        case 0x01:
            retval = ARGB_LIGHT_GRAY;
            break;
        case 0x02:
            retval = ARGB_DARK_GRAY;
            break;
        case 0x03:
            retval = ARGB_BLACK;
            break;
        default:
            std::logic_error("Palette::convertColor(): Could not resolve pixel color for palette update.");
    }
    return retval;
}

uint32_t Palette::getColor(PaletteSelect select, PaletteIndex index){
    Regval8 regVal;
    Regval8 mask;
    switch(select){
        case BGP:
            regVal = bgpReg; break;
        case OBP0:
            regVal = obp0Reg;break;
        case OBP1:
            regVal = obp1Reg;break;
    }
    switch(index){
        case COLOR_0:
            mask = COLOR_0_MASK;break;
        case COLOR_1:
            mask = COLOR_1_MASK;break;
        case COLOR_2:
            mask = COLOR_2_MASK;break;
        case COLOR_3:
            mask = COLOR_3_MASK;break;
    }
    return convertColor((regVal & mask) >> (2 * index));
}