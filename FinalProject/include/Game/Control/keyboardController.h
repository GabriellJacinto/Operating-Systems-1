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
#include "Game/Control/Clock.h"

__BEGIN_API

using namespace std;

class BrickShooter;

class KeyboardHandler
{
public:
    struct keys
    {
        Play::KeyPress moveKey;
        Play::KeyPress actionKey;
    };

    KeyboardHandler(Window* window);
    ~KeyboardHandler();

    typedef sf::Event Event;
    void run();
    static Semaphore* eventQueueSemaphore;
    static Semaphore* saveEventsSemaphore;

    keys getNextKey();

    bool saveEvents = true;

private:
    static std::map<sf::Keyboard::Key, bool> keyStates;
    keys getPressedKey();
    Window* window;
    std::queue<keys> eventQueue;
    Clock actionClock;
};

__END_API

#endif
