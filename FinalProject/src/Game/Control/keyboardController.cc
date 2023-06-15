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
    this->actionClock = Clock();
}

KeyboardHandler::~KeyboardHandler()
{
    delete KeyboardHandler::eventQueueSemaphore;
    delete KeyboardHandler::saveEventsSemaphore;
}

void KeyboardHandler::run()
{
    this->actionClock.restart();
    while (!Config::finished)
    {
        keys key = getPressedKey();

        if (key.actionKey == Play::KeyPress::QUIT)
        {
            //Config::finishedSemaphore->p();
            Config::finished = true;
            //Config::finishedSemaphore->v();

            window->quit();
        }
        else if (key.actionKey == Play::KeyPress::PAUSE)
        {
            //Config::pausedSemaphore->p();
            //Config::pausedSemaphore->v();
            //saveEventsSemaphore->p();
            saveEvents = !saveEvents;
            //saveEventsSemaphore->v();
            //BrickShooter::pause();
            Config::paused = !Config::paused;
            window->pause();
        }
        else if (key.actionKey == Play::KeyPress::RESTART)
        {
            //Config::gameOverSemaphore->p();
            Config::gameOver = false;
            //Config::gameOverSemaphore->v();
            BrickShooter::restart();
        }
        else if (key.moveKey != Play::KeyPress::NONE || key.actionKey != Play::KeyPress::NONE)
        {
            //eventQueueSemaphore->p();
            //saveEventsSemaphore->p();

            if (saveEvents)
            {
                eventQueue.push(key);
            }

            //saveEventsSemaphore->v();
            //eventQueueSemaphore->v();
        }
        Thread::yield();
    }
}

KeyboardHandler::keys KeyboardHandler::getNextKey()
{
    //KeyboardHandler::eventQueueSemaphore->p();
    if (eventQueue.empty())
    {
        //KeyboardHandler::eventQueueSemaphore->v();
        return {Play::KeyPress::NONE, Play::KeyPress::NONE};
    }
    KeyboardHandler::keys keys = eventQueue.front();
    eventQueue.pop();
    //KeyboardHandler::eventQueueSemaphore->v();

    return keys;
}

inline KeyboardHandler::keys KeyboardHandler::getPressedKey()
{
    sf::Event event;
    window->getWindow()->pollEvent(event);

    static std::map<sf::Keyboard::Key, Play::KeyPress> moveKeys = {
            {sf::Keyboard::Left,  Play::KeyPress::LEFT},
            {sf::Keyboard::Right, Play::KeyPress::RIGHT},
            {sf::Keyboard::Down,  Play::KeyPress::DOWN},
            {sf::Keyboard::Up,    Play::KeyPress::UP}
    };

    static std::map<sf::Keyboard::Key, Play::KeyPress> actionKeys = {
            {sf::Keyboard::Space, Play::KeyPress::SHOOT},
            {sf::Keyboard::Q, Play::KeyPress::QUIT},
            {sf::Keyboard::R, Play::KeyPress::RESTART},
            {sf::Keyboard::P, Play::KeyPress::PAUSE}
    };

    static std::map<sf::Keyboard::Key, bool> keyStates;
    switch (event.type)
    {
        case sf::Event::Closed:
            //Config::finishedSemaphore->p();
            Config::finished = true;
            //Config::finishedSemaphore->v();
            window->close();
            break;
        case sf::Event::KeyPressed:
            keyStates[event.key.code] = true;
            break;
        case sf::Event::KeyReleased:
            keyStates[event.key.code] = false;
            break;
    }

    Play::KeyPress moveKey = Play::KeyPress::NONE;
    for (const auto& pair : moveKeys)
    {
        if (keyStates[pair.first])
        {
            moveKey = pair.second;
        }
    }

    Play::KeyPress actionKey = Play::KeyPress::NONE;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
    {
        actionKey = Play::KeyPress::SHOOT;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::P))
    {
        actionKey = Play::KeyPress::PAUSE;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
    {
        actionKey = Play::KeyPress::QUIT;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
    {
        actionKey = Play::KeyPress::RESTART;
    }

    if (actionKey == Play::KeyPress::PAUSE || actionKey == Play::KeyPress::RESTART)
    {
        if (actionClock.getElapsedTime() > 250)
        {
            actionClock.restart();
        }
        else
        {
            actionKey = Play::KeyPress::NONE;
        }
    }

    return {moveKey, actionKey};
}


__END_API
