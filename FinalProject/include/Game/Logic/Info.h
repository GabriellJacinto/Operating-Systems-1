#ifndef INFO_H
#define INFO_H

#include "Concurrency/traits.h"

__BEGIN_API

class Info
{
public:
    Info();
    ~Info() = default;

    int score;
    int lives;
    int level;

    void incraseScore();
    void decreaseLives();
    void increaseLevel();

};

__END_API

#endif //INFO_H
