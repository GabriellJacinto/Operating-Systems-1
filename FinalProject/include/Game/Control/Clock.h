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

    double getElapsedTime();

private:
    sf::Clock clock;
    int fps;
    double previousTime;
};

__END_API

#endif //CLOCK_H
