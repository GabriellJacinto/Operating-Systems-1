#ifndef INFO_H
#define INFO_H

#include "Concurrency/traits.h"

__BEGIN_API

namespace Info
{
    struct Info
    {
        int score;
        int lives;
        int level;
    };

    void incraseScore(Info& info);
    void decreaseLives(Info& info);
    void increaseLevel(Info& info);
}
__END_API

#endif //INFO_H
