#ifndef CLOCK_H
#define CLOCK_H

#include "Concurrency/traits.h"
#include "SFML/Graphics.hpp"

__BEGIN_API

class Clock {
public:
    Clock();

    ~Clock() = default;

    void restart();

    float getElapsedTime();

private:
    sf::Clock clock;
    int fps;
    float previousTime;
};

__END_API

#endif //CLOCK_H
