#ifndef controller_h
#define controller_h

#include "Concurrency/traits.h"
#include <SFML/Graphics.hpp>

__BEGIN_API


class BrickShooter
{
public:
    typedef sf::Event Event;

    BrickShooter();
    ~BrickShooter();

    void processUserInput(Event event);

private:
    // fpsController
    // SpaceShipController

    void update();
};

__END_API

#endif //controller_h
