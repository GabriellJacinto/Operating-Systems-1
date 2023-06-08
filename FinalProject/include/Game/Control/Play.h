#ifndef PLAY_H
#define PLAY_H

#include "traits.h"

__BEGIN_API

namespace Play
{
    enum KeyPress
    {
        UP,
        DOWN,
        RIGHT,
        LEFT,
        SHOOT,
        QUIT,
        RESTART,
        PAUSE,
        NONE
    };
}

__END_API

#endif //PLAY_H
