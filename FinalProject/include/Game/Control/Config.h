#ifndef CONFIGS_H
#define CONFIGS_H

#include "Concurrency/traits.h"
#include <iostream>
#include "Concurrency/semaphore.h"
#include <ucontext.h>

__BEGIN_API

class Config
{
public:
    static Semaphore* finishedSemaphore;
    static Semaphore* pausedSemaphore;
    static Semaphore* gameOverSemaphore;
    static int windowWidth;
    static int windowHeight;
    static int playableAreaWidth;
    static int playableAreaHeight;
    static int infoAreaWidth;
    static int infoAreaHeight;
    static int fps;
    static int lives;
    static bool finished;
    static bool paused;
    static bool gameOver;
    static int enemiesPerLevel;
    static bool bossExists;
    static void deleteSemaphores();
};

__END_API

#endif //CONFIGS_H
