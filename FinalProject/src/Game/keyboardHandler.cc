#include "Game/keyboardHandler.h"
#include <SFML/Graphics.hpp>
#include "Concurrency/traits.h"
#include <string>

using namespace std;

__BEGIN_API

string KeyboardHandler::getPressedKey(Event event)
{
    switch (event.type)
    {
        case sf::Event::Closed:
            return "Closed";

        case sf::Event::KeyPressed:
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
                return "Left";
            } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
                return "Right";
            } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
                return "Down";
            } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
                return "Up";
            } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
                return "Shoot";
            } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                return "PauseOrResume";
            } else {
                return "Ignore";
            }
    }
}

__END_API
