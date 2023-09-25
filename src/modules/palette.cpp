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

void Palette::update(){
    bgp[COLOR_0] = convertColor(bgpReg & COLOR_0_MASK);
    bgp[COLOR_1] = convertColor((bgpReg & COLOR_1_MASK) >> 2);
    bgp[COLOR_2] = convertColor((bgpReg & COLOR_2_MASK) >> 4);
    bgp[COLOR_3] = convertColor((bgpReg & COLOR_3_MASK) >> 6);
    //obp color 0 is ignored since its transparent
    obp0[COLOR_1] = convertColor((obp0Reg & COLOR_1_MASK) >> 2);
    obp0[COLOR_2] = convertColor((obp0Reg & COLOR_2_MASK) >> 4);
    obp0[COLOR_3] = convertColor((obp0Reg & COLOR_3_MASK) >> 6);
    
    obp1[COLOR_1] = convertColor((obp1Reg & COLOR_1_MASK) >> 2);
    obp1[COLOR_2] = convertColor((obp1Reg & COLOR_2_MASK) >> 4);
    obp1[COLOR_3] = convertColor((obp1Reg & COLOR_3_MASK) >> 6);
}

uint32_t Palette::getColor(PaletteSelect select, PaletteIndex index){
    std::array<uint32_t, 4> palette;
    switch(select){
        case BGP:
            palette = bgp;
            break;
        case OBP0:
            palette = obp0;
            break;
        case OBP1:
            palette = obp1;
            break;
    }
    return palette[index];
}