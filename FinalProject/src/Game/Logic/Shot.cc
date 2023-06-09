#include "Game/Logic/Shot.h"
#include "Concurrency/traits.h"

__BEGIN_API

void Shot::loadAndBindTexture()
{
    texture.loadFromFile("assets/Sprites/space_ships/shot.png");
    sprite.setTexture(texture);
    sprite.scale(-0.5, -0.5);
}

void Shot::draw(sf::RenderWindow window, double diffTime)
{
    window.draw(this->sprite);
    this->update(diffTime);
}

inline void Shot::update(double diffTime)
{
    this->move(diffTime);
}

inline void Shot::move(double diffTime)
{
    this->position = this->position + this->speed * diffTime;
}

int Shot::getSize()
{
    return 14;
}

void Shot::handleOutOfBounds()
{
    if (this->position.x > Config::playableAreaWidth - this->getSize())
        this->removeFromGame();
    else if (this->position.x < this->getSize())
        this->removeFromGame();
    if (this->position.y > Config::playableAreaHeight - this->getSize())
        this->removeFromGame();
    else if (this->position.y < this->getSize())
        this->removeFromGame();
}

void Shot::removeFromGame()
{
    Window::toBeDrawnSemaphore->p();
    Window::removeElementToDraw(this);
    Window::toBeDrawnSemaphore->v();
    delete this;
}

Point Shot::getPosition()
{
    return this->position;
}

__END_API
