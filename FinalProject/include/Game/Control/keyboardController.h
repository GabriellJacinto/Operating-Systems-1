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

class BrickShooter;

class KeyboardHandler
{
public:
    KeyboardHandler(Window* window);
    ~KeyboardHandler() = default;

    typedef sf::Event Event;
    void run();
    static Semaphore* eventQueueSemaphore;

    Play::KeyPress getNextKey();

private:
    Play::KeyPress getPressedKey(Event event);
    Event getNextEvent();
    Window* window;
    queue<Play::KeyPress> eventQueue; // volatile because it is accessed by different threads (keyboardHandler and player)
};

__END_API

#endif
