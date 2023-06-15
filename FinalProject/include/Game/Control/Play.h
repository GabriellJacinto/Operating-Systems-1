#ifndef PLAY_H
#define PLAY_H

#include "Concurrency/traits.h"

__BEGIN_API

namespace Play
{
    enum KeyPress
    {
        UP = 0,
        DOWN = 1,
        RIGHT = 2,
        LEFT = 3,
        SHOOT = 4,
        QUIT = 5,
        RESTART = 6,
        PAUSE = 7,
        NONE = 8
    };
}

__END_API

#endif //PLAY_H
