#ifndef INFO_H
#define INFO_H

#include "Concurrency/traits.h"
#include "Concurrency/semaphore.h"

__BEGIN_API

class Info
{
public:
    Info();
    ~Info();

    int score;
    int lives;
    int level;

    static Semaphore* scoreSemaphore;
    static Semaphore* livesSemaphore;
    static Semaphore* levelSemaphore;

    void incraseScore();
    void decreaseLives();
    void increaseLevel();

};

__END_API

#endif //INFO_H
