#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "Concurrency/traits.h"
#include "Game/Logic/Point.h"
#include "Game/Logic/Vector.h"
#include "Game/Interface/Drawable.h"
#include "Game/Control/Config.h"
#include "Game/Interface/window.h"

__BEGIN_API

class CollisionHandler;

class Shot : public Drawable
{
public:
    enum Direction
    {
        LEFT,
        RIGHT,
        UP,
        DOWN
    };

    // TEST: DOES COLLISION HANDLER WORK?
    Shot(const Point& position, const Vector& speed, Direction direction, bool isPlayerShot) : position(position), speed(speed), isPlayerShot(isPlayerShot), direction(direction)
    {
        Window::toBeDrawnSemaphore->p();
        Window::addElementToDraw(this);
        Window::toBeDrawnSemaphore->v();
        CollisionHandler::shotsSemaphore->p();
        CollisionHandler::addShot(this);
        CollisionHandler::shotsSemaphore->v();

        loadAndBindTexture();
    }
    ~Shot() override = default;

    void draw(sf::RenderWindow window, double diffTime) override;
    void update(double diffTime) override;
    int getSize() override;

    int getDamage() const { return this->damage; }

    void move(double diffTime);
    Point getPosition() const { return this->position; }
    bool getIsPlayerShot() const { return this->isPlayerShot; }
    void handleOutOfBounds();
    void removeFromGame();

    void loadAndBindTexture();

private:
    Point position;
    Vector speed;
    bool isPlayerShot;
    int damage = 1;
    sf::Sprite sprite;
    sf::Texture texture;
    Direction direction;
};

__END_API

#endif //PROJECTILE_H
