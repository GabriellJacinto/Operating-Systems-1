#include "Concurrency/traits.h"
#include "Game/Logic/Info.h"

__BEGIN_API

Semaphore* Info::scoreSemaphore = new Semaphore();
Semaphore* Info::livesSemaphore = new Semaphore();
Semaphore* Info::levelSemaphore = new Semaphore();


Info::Info()
{
    score = 0;
    lives = 3;
    level = 1;
}

Info::~Info()
{
    delete livesSemaphore;
    delete scoreSemaphore;
    delete levelSemaphore;
}

void Info::incraseScore()
{
    scoreSemaphore->p();
    this->score += 100;
    scoreSemaphore->v();
}

void Info::decreaseLives()
{
    livesSemaphore->p();
    this->lives--;
    livesSemaphore->v();
}

void Info::increaseLevel()
{
    levelSemaphore->p();
    this->level++;
    levelSemaphore->v();
}

__END_API
