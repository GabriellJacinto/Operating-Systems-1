#include "Concurrency/traits.h"
#include "Game/Logic/Info.h"

__BEGIN_API

void Info::incraseScore(Info& info)
{
    info.score += 100;
}

void Info::decreaseLives(Info& info)
{
    info.lives--;
}

void Info::increaseLevel(Info &info)
{
    info.level++;
}

__END_API