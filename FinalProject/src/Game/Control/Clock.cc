#include "Game/Control/Clock.h"
#include "Concurrency/traits.h"
#include "Game/Control/Config.h"

__BEGIN_API

Clock::Clock()
{
    this->fps = Config::fps;
    this->clock = sf::Clock();
}

void Clock::restart()
{
    this->previousTime = 0;
    this->clock.restart();
    double currentTime = this->clock.getElapsedTime().asMilliseconds();
    double previousTime = currentTime;
}

double Clock::getElapsedTime()
{
    double currentTime = this->clock.getElapsedTime().asMilliseconds();
    double elapsedTime = currentTime - this->previousTime;
    return elapsedTime;
}

__END_API