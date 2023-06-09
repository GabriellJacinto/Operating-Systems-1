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
#include "Game/Logic/Shot.h"

__BEGIN_API

class CollisionHandler;

class Player : public Collidable
{
public:
    Player(KeyboardHandler* keyboardHandler);
    ~Player() override = default;

    void run();
    void draw(sf::RenderWindow, double diffTime) override;
    void collide(int damage) override;
    bool isDead() override;
    void update(double diffTime) override;
    bool isOutOfPlay();
    int getLife() const;
    int getSize() override;
    Point getPosition() override;

private:
    static int HALF_PLAYER_SIZE;
    static int PLAYER_SIZE;
    static int PLAYER_SPEED;
    static float SHOT_COOLDOWN;
    static Vector SHOT_SPEED;

    KeyboardHandler* keyboardHandler;
    std::shared_ptr<Clock> shotClock;

    int life = 3;

    bool invulnerable = false;
    float invulnerabilityTime; // in seconds
    static float INVULNERABILITY_TIME;

    void move(double diffTime);
    void handleOutOfBounds();
    void removeFromGame();
    void updateSprite();
    void processKeyboardInput();
    void shoot(Shot::Direction direction);

    sf::Sprite sprite;
    sf::Texture texture;
    Vector speed;
    void loadAndBindTexture();
    Point position;
    Shot::Direction direction;
};

__END_API

#endif //PLAYER_H
