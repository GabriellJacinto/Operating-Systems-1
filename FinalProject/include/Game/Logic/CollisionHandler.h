#ifndef COLLISIONHANDLER_H
#define COLLISIONHANDLER_H
#include <list>
#include "Game/Interface/Collidable.h"
#include "Game/Logic/Shot.h"
#include "Concurrency/semaphore.h"
#include "Concurrency/thread.h"
#include "Concurrency/traits.h"
#include "Game/Logic/Player.h"
#include "Game/Logic/Enemy.h"

__BEGIN_API

class CollisionHandler
{
public:
    CollisionHandler() = default;
    ~CollisionHandler() = default;
    static Semaphore* shipsSemaphore;
    static Semaphore* shotsSemaphore;

private:
    list<Collidable*> ships;
    list<Shot*> shots;
};

__END_API

#endif //COLLISIONHANDLER_H
