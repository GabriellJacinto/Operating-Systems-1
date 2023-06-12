#include "Game/Control/BrickShooter.h"
#include "Concurrency/system.h"
#include "Concurrency/traits.h"

__USING_API

int main(void)
{
    System::init(&BrickShooter::play);
}
