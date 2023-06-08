#ifndef keyboardHandler_h
#define keyboardHandler_h

#include <SFML/Graphics.hpp>
#include <iostream>
#include "Concurrency/traits.h"
#include <queue>
#include "Play.h"
#include <string>
#include "Concurrency/semaphore.h"
#include "Game/Interface/window.h"
#include "Concurrency/thread.h"

__BEGIN_API

using namespace std;

class KeyboardHandler
{
public:
    KeyboardHandler(Window* window);
    ~KeyboardHandler() = default;

    typedef sf::Event Event;
    void run();
    Play::KeyPress getPressedKey(Event event);
    static Semaphore* eventQueueSemaphore;
    Event getNextEvent();

private:
    Window* window;
    queue<Play::KeyPress> eventQueue;
};

__END_API

#endif
