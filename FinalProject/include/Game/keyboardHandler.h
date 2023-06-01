#ifndef keyboardHandler_h
#define keyboardHandler_h

#include <SFML/Graphics.hpp>
#include <iostream>
#include "Concurrency/traits.h"
#include <queue>
#include <string>

__BEGIN_API

using namespace std;

class KeyboardHandler
{
public:
    typedef sf::Event Event;

    KeyboardHandler() = default;
    ~KeyboardHandler() = default;
    static string getPressedKey(Event event);

private:

};

__END_API

#endif
