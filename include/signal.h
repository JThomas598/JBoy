#ifndef SIGNAL_H
#define SIGNAL_H
#include <memory.h>

constexpr Regval16 VBLANK_SIGNAL = 0x01;

constexpr Regval16 HBLANK_SIGNAL = 0x02;

constexpr Regval16 LINE_SIGNAL = 0x04;

constexpr Regval16 FRAME_SIGNAL = 0x08;

constexpr Regval16 ALL_SIGNALS = 0x0F;

class Signal{
    private:
        static Regval16 flags;
        static Regval16 flagEnable;

        void clearSignal(Regval16 mask);
        bool checkFlag(Regval16 mask);
    
    public:
        Signal();

        void raiseSignal(Regval16 mask);

        void enableSignal(Regval16 mask);

        bool signalRaised(Regval16 mask);
};
#endif