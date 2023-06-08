#include "Game/Control/keyboardController.h"
#include <SFML/Graphics.hpp>
#include "Concurrency/traits.h"
#include "Game/Control/Play.h"
#include "Game/Control/Config.h"


using namespace std;

__BEGIN_API

Semaphore* KeyboardHandler::eventQueueSemaphore = new Semaphore();

KeyboardHandler::KeyboardHandler(SOLUTION::Window *window)
{
    this->window = window;
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

            window->pause();
        }

        eventQueueSemaphore->p();
        eventQueue.push(key);
        eventQueueSemaphore->v();
        Thread::yield();
    }
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
