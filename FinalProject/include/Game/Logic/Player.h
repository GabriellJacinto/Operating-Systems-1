#ifndef PLAYER_H
#define PLAYER_H

#include "Concurrency/traits.h"
#include "Game/Interface/Collidable.h"
#include "Game/Control/Config.h"
#include "Game/Control/BrickShooter.h"
#include "Game/Interface/window.h"
#include "Game/Control/keyboardController.h"
#include "CollisionHandler.h"
#include "Game/Control/Play.h"
#include "Game/Logic/Vector.h"
#include "Game/Logic/Point.h"
#include <memory>
#include "Game/Control/Clock.h"

__BEGIN_API

class CollisionHandler;

class Player : public Collidable
{
public:
    Player();
    Player(KeyboardHandler* keyboardHandler);
    ~Player();

    void run();
    void draw();
    void collide(int damage);
    bool isDead();
    void update(double diffTime);
    int getLife();
    int getSize();
    Point getPosition();

private:
    void init();
    static int HALF_PLAYER_SIZE;
    static int PLAYER_SIZE;
    static int PLAYER_TRAVEL_SPEED;
    static int WEAK_ATTACK_DELAY;
    static int STRONG_ATTACK_DELAY;
    static Vector PLAYER_PROJECTILE_SPEED;

    std::shared_ptr<Clock> projectileClock;

    int life = 3;

    bool wasJustHit = false;
    float invulnerabilityTime = 0.5f; // in seconds
    static int INVULNERABILITY_TIME;

    void isOutOfBounds();
    void removeFromGame();
    void updateSprite();
    void processKeyboardInput();
    void shoot();

    sf::Sprite sprite;
    Vector speed;
    void loadSprite();
    Point position;
};

__END_API

#endif //PLAYER_H
