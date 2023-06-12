#include "Game/Control/keyboardController.h"
#include <SFML/Graphics.hpp>
#include "Concurrency/traits.h"
#include "Game/Control/Play.h"
#include "Game/Control/Config.h"
#include "Game/Control/BrickShooter.h"

using namespace std;

__BEGIN_API

Semaphore* KeyboardHandler::eventQueueSemaphore = new Semaphore();
Semaphore* KeyboardHandler::saveEventsSemaphore = new Semaphore();

KeyboardHandler::KeyboardHandler(Window *window)
{
    this->window = window;
}

KeyboardHandler::~KeyboardHandler()
{
    delete KeyboardHandler::eventQueueSemaphore;
    delete KeyboardHandler::saveEventsSemaphore;
}

void KeyboardHandler::run()
{
    while (!Config::finished)
    {
        Event event = getNextEvent();
        Play::KeyPress key = getPressedKey(event);

        if (key == Play::KeyPress::QUIT)
        {
            Config::finishedSemaphore->p();
            Config::finished = true;
            Config::finishedSemaphore->v();

            window->quit();
        }
        else if (key == Play::KeyPress::PAUSE)
        {
            Config::pausedSemaphore->p();
            Config::paused = !Config::paused;
            Config::pausedSemaphore->v();
            BrickShooter::pause();
            window->pause();
        }
        else if (key == Play::KeyPress::RESTART)
        {
            Config::gameOverSemaphore->p();
            Config::gameOver = false;
            Config::gameOverSemaphore->v();
            BrickShooter::restart();
        }
        else
        {
            eventQueueSemaphore->p();
            saveEventsSemaphore->p();

            if (saveEvents)
            {
                eventQueue.push(key);
            }

            saveEventsSemaphore->v();
            eventQueueSemaphore->v();
        }

        Thread::yield();
    }
}

Play::KeyPress KeyboardHandler::getNextKey()
{
    KeyboardHandler::eventQueueSemaphore->p();
    Play::KeyPress key = eventQueue.front();
    eventQueue.pop();
    KeyboardHandler::eventQueueSemaphore->v();

    return key;
}

sf::Event KeyboardHandler::getNextEvent()
{
    Event event;
    window->getWindow()->pollEvent(event);
    return event;
}

Play::KeyPress KeyboardHandler::getPressedKey(Event event)
{
    switch (event.type)
    {
        case sf::Event::KeyPressed:
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                return Play::KeyPress::LEFT;
            } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                return Play::KeyPress::RIGHT;
            } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                return Play::KeyPress::DOWN;
            } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
                return Play::KeyPress::UP;
            } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
                return Play::KeyPress::SHOOT;
            } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
                return Play::KeyPress::QUIT;
            } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
                return Play::KeyPress::RESTART;
            } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::P)) {
                return Play::KeyPress::PAUSE;
            } else {
                return Play::KeyPress::NONE;
            }
    }
}

__END_API
