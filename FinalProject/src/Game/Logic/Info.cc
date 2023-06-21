#include "Concurrency/traits.h"
#include "Game/Logic/Info.h"

__BEGIN_API

Info::Info()
{
    score = 0;
    lives = 3;
    level = 1;
}

void Info::incraseScore()
{
    this->score += 100;
}

void Info::decreaseLives()
{
    this->lives--;
}

void Info::increaseLevel()
{
    this->level++;
}

__END_API