#ifndef keyboardHandler_h
#define keyboardHandler_h

#include <SFML/Graphics.hpp>
#include <iostream>
#include "Concurrency/traits.h"
#include <queue>

__BEGIN_API

using namespace std;

class KeyboardHandler
{
public:
    typedef sf::Keyboard Key;

    KeyboardHandler();
    ~KeyboardHandler();

    void init();
    void start();
    bool isKeyPressed(Key key);

private:
    void update();

    float beginTime;
    float nowTime;

    queue<sf::Event> events;

};

__END_API

#endif
