#include "Game/Logic/Shot.h"
#include "Concurrency/traits.h"
#include "Game/Logic/CollisionHandler.h"

__BEGIN_API

Shot::Shot(const Point &position, const Vector &speed, Shot::Direction direction,bool isPlayerShot)
{
    this->position = position;
    this->speed = speed;
    this->isPlayerShot = isPlayerShot;
    this->direction = direction;

    Window::toBeDrawnSemaphore->p();
    Window::addElementToDraw(this);
    Window::toBeDrawnSemaphore->v();
    CollisionHandler::shotsSemaphore->p();
    CollisionHandler::addShot(this);
    CollisionHandler::shotsSemaphore->v();

    loadAndBindTexture();
}

void Shot::loadAndBindTexture()
{
    texture.loadFromFile("assets/Sprites/space_ships/shot.png");
    sprite.setTexture(texture);
    sprite.scale(-0.5, -0.5);
}

void Shot::draw(sf::RenderWindow &window, double diffTime)
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
    CollisionHandler::shotsSemaphore->p();
    CollisionHandler::removeShot(this);
    CollisionHandler::shotsSemaphore->v();
    delete this;
}

Point Shot::getPosition()
{
    return this->position;
}

void Shot::collide()
{
    this->removeFromGame();
}

__END_API
