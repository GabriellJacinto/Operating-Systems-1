#ifndef ENEMY_H
#define ENEMY_H

#include "Concurrency/traits.h"
#include "Game/Control/Config.h"
#include "Game/Interface/window.h"
#include "Game/Logic/Vector.h"
#include "Game/Logic/Point.h"
#include <memory>
#include "Game/Control/Clock.h"
#include "Game/Logic/Shot.h"
#include "Game/Interface/Collidable.h"
#include <random>
#include "Game/Logic/Player.h"

__BEGIN_API

class CollisionHandler;

enum class Algorithm
{
    A,
    B
};

class Enemy : public Collidable
{
public:
    Enemy(Algorithm algorithm, Player* player);
    ~Enemy() override;

    void run();
    void draw(sf::RenderWindow, double diffTime) override;
    void collide(int damage) override;
    bool isDead() override {return _isDead;}
    void update(double diffTime) override;
    bool isOutOfPlay();
    int getSize() override;
    Point getPosition() override;

    static Semaphore* isDeadSemaphore;

private:
    static int HALF_ENEMY_SIZE;
    static int ENEMY_SIZE;
    static int ENEMY_SPEED;
    static float SHOT_COOLDOWN;
    static float RELIVE_TIME;
    static Vector SHOT_SPEED;

    std::unique_ptr<Clock> shotClock;
    std::unique_ptr<Clock> reliveClock;

    Player* player;
    bool _isDead = false;
    float reliveTime = 0;
    Algorithm algorithm;

    void processDirectionAlgorithm();
    Shot::Direction directionAlgorithmA();
    Shot::Direction directionAlgorithmB();
    void move(double diffTime);
    void handleOutOfBounds();
    void updateSprite();
    void shoot(Shot::Direction direction);
    void insertInGame();
    void removeFromGame();

    void loadAndBindTexture();

    sf::Texture texture;
    sf::Sprite sprite;
    Vector speed;
    Point position;
    Shot::Direction direction;
};

__END_API

#endif //ENEMY_H
