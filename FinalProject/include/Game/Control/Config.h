#ifndef CONFIGS_H
#define CONFIGS_H

#include "Concurrency/traits.h"
#include <iostream>
#include "Concurrency/semaphore.h.h"
#include <ucontext.h>

__BEGIN_API

class Config
{
public:
    static Semaphore* finishedSemaphore;
    static Semaphore* pausedSemaphore;
    static int windowWidth;
    static int windowHeight;
    static int playableAreaWidth;
    static int playableAreaHeight;
    static int infoAreaWidth;
    static int infoAreaHeight;
    static int fps;
    static bool finished;
    static bool paused;
    static bool bossExists;
};

__END_API

#endif //CONFIGS_H
