#include "Game/Logic/Shot.h"
#include "Concurrency/traits.h"
#include "Game/Logic/CollisionHandler.h"
#include "Game/Logic/Player.h"
#include "Game/Interface/Sounds.h"

__BEGIN_API

Shot::Shot(const Point &position, Shot::Direction direction,bool isPlayerShot)
{
    this->position = position;
    this->speed = getSpeed(direction);
    this->isPlayerShot = isPlayerShot;
    this->direction = direction;
    //Window::toBeDrawnSemaphore->p();
    Window::addElementToDraw(this);
    //Window::toBeDrawnSemaphore->v();
    //CollisionHandler::shotsSemaphore->p();
    CollisionHandler::addShot(this);
    //CollisionHandler::shotsSemaphore->v();
    loadAndBindTexture();
    Sounds::playShotSound();
}

Vector Shot::getSpeed(Shot::Direction directionOfShot)
{
    Vector shotSpeed;
    switch (directionOfShot)
    {
        case Shot::Direction::LEFT:
            shotSpeed = Vector(-Player::PLAYER_SPEED, 0);
            break;
        case Shot::Direction::RIGHT:
            shotSpeed = Vector(Player::PLAYER_SPEED, 0);
            break;
        case Shot::Direction::UP:
            shotSpeed = Vector(0, -Player::PLAYER_SPEED);
            break;
        case Shot::Direction::DOWN:
            shotSpeed = Vector(0, Player::PLAYER_SPEED);
            break;
    }
    return shotSpeed * this->speedFactor;
}

void Shot::loadAndBindTexture()
{
    texture.loadFromFile("assets/sprites/space_ships/shot.png");
    sprite.setTexture(texture);
    sprite.scale(-0.5, -0.5);
    sf::Vector2u textureSize = texture.getSize();
    sprite.setOrigin(textureSize.x / 2, textureSize.y / 2);
}

void Shot::draw(sf::RenderWindow &window, double diffTime)
{
    window.draw(this->sprite);
    this->update(diffTime);
    this->updateSprite();
}

inline void Shot::update(double diffTime)
{
    this->move(diffTime);
}

void Shot::updateSprite()
{
    this->sprite.setPosition(this->position.x, this->position.y);
}

inline void Shot::move(double diffTime)
{
    this->position = this->position + this->speed * diffTime;
    this->handleOutOfBounds();
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
    //Window::toBeDrawnSemaphore->p();
    Window::removeElementToDraw(this);
    //Window::toBeDrawnSemaphore->v();
    //CollisionHandler::shotsSemaphore->p();
    CollisionHandler::removeShot(this);
    //CollisionHandler::shotsSemaphore->v();
}

Point Shot::getPosition()
{
    return this->position;
}

void Shot::collide()
{
    this->removeFromGame();
}

sf::FloatRect Shot::getGlobalBounds()
{
    return this->sprite.getGlobalBounds();
}

__END_API
