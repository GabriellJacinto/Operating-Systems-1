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
}

float Clock::getElapsedTime()
{
    float currentTime = this->clock.getElapsedTime().asSeconds();
    float elapsedTime = currentTime - this->previousTime;
    this->previousTime = currentTime;
    return elapsedTime;
}

__END_API