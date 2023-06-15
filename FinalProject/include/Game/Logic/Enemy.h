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

__BEGIN_API

class Player;

class CollisionHandler;

class Enemy : public Collidable
{
public:
    enum Algorithm
    {
        A,
        B,
        C
    };

    Enemy(Algorithm algorithm, Player* player, Point position);

    ~Enemy() override;

    void run();
    void draw(sf::RenderWindow &window, double diffTime) override;
    void collide(int damage) override;
    bool isDead() override {return _isDead;}
    void update(double diffTime) override;
    bool isOutOfPlay();
    int getSize() override;
    Point getPosition() override;
    void setInitialPosition(const Point& position);
    void setPosition(const Point& position);
    Point getPreviousPosition();

    static Semaphore* isDeadSemaphore;
    static Semaphore* moveSemaphore;

    int damageGiven = 1;

    static bool avoidCollision(Enemy* enemy1, Enemy* enemy2);

    static int ENEMY_SPEED;

    void inverseDirection();

    bool avoidingCollision = false;
    static int ENEMY_SIZE;

    void insertInGame();

    Point previousPosition;
private:
    static int HALF_ENEMY_SIZE;
    static float SHOT_COOLDOWN;
    static float RELIVE_TIME;
    static float DIAGONAL_TIME;
    static Vector SHOT_SPEED;
    static int MINIMUM_DISTANCE;
    static int TARGET_DISTANCE;
    static float RANDOM_MOVE_TIME;

    std::unique_ptr<Clock> shotClock;
    std::unique_ptr<Clock> reliveClock;
    std::unique_ptr<Clock> diagonalClock;
    std::unique_ptr<Clock> randomMoveClock;

    Player* player;
    bool _isDead = false;
    Enemy* previousEnemyCollided = nullptr;
    Algorithm algorithm;

    void processDirectionAlgorithm();
    Shot::Direction directionAlgorithmA();
    Shot::Direction directionAlgorithmB();
    void move(double diffTime);
    void handleOutOfBounds();
    void updateSprite();
    void shoot();
    Shot::Direction getRandomDirection();

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
