#include "Concurrency/traits.h"
#include "Game/Logic/CollisionHandler.h"

__BEGIN_API

Semaphore* CollisionHandler::shipsSemaphore = new Semaphore();
Semaphore* CollisionHandler::shotsSemaphore = new Semaphore();

__END_API
