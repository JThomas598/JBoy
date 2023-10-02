#include "memory.h"
#include <array>
#include <vector>

typedef struct Object{
    Regval8 y_pos;
    Regval8 x_pos;
    Regval8 entryNum;
    Regval8 tileIndex;
    Regval8 flags;
}Object;


typedef enum AttributeBitIndex{
    PALLETE_NUMBER = 4,
    X_FLIP,
    Y_FLIP,
    MAP_OVER_OBJ
}AttributeBitIndex;

constexpr Regval8 OBJECT_MAX = 40;
constexpr Regval8 OBJECT_SIZE = 4;

class OAM{
    private:
        Memory mem; 
        std::vector<Object> visibleObjs;
        Register lcdcReg;

        void printVisibleObjs();
        void checkBit(Regval8 val, Regval8 index);
        
    public:
        OAM();
        Object popObj();
        Regval8 getMinX();
        Regval8 searchLine(const Regval8 lineNum);
        void clearQueue();
};