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
        C,
        D
    };

    Enemy(Algorithm algorithm, Player* player, Point position);

    ~Enemy() override;

    void run();
    void draw(sf::RenderWindow &window, double diffTime) override;
    void collide(int damage) override;
    bool isDead() override {return _isDead;}
    void update(double diffTime) override;
    int getSize() override;
    Point getPosition() override;
    Point getCenter();
    void setPosition(const Point& position);
    Point getPreviousPosition();

    static Semaphore* isDeadSemaphore;
    static Semaphore* moveSemaphore;

    bool collidedWithPlayer = false;
    int damageGiven = 1;
    bool isStuck = false;
    static int ENEMY_SPEED;
    void inverseDirection();
    Shot::Direction inverseDirection(Shot::Direction dir);
    void setDirection(Shot::Direction direction);
    static int ENEMY_SIZE;
    void insertInGame();
    void setEnemiesToAvoid(Enemy* enemy1, Enemy* enemy2, Enemy* enemy3);
    sf::FloatRect getGlobalBounds() override;
    Point previousPosition;
    static std::unique_ptr<Clock> avoidCollisionClock;

private:
    static float SHOT_COOLDOWN;
    static float RELIVE_TIME;
    static float DIAGONAL_TIME;
    static Vector SHOT_SPEED;
    static int MINIMUM_DISTANCE;
    static int TARGET_DISTANCE;
    static float RANDOM_MOVE_TIME;
    static float HIT_ANIMATION_TIME;

    std::unique_ptr<Clock> shotClock;
    std::unique_ptr<Clock> reliveClock;
    std::unique_ptr<Clock> diagonalClock;
    std::unique_ptr<Clock> randomMoveClock;
    std::unique_ptr<Clock> drawDamagedEnemyClock;

    Enemy* enemyToAvoid1 = nullptr;
    Enemy* enemyToAvoid2 = nullptr;
    Enemy* enemyToAvoid3 = nullptr;

    Player* player;
    bool _isDead = false;
    bool avoidingCollision = false;
    Algorithm algorithm;

    void processDirectionAlgorithm();
    void directionAlgorithmA();
    void directionAlgorithmB();
    void directionAlgorithmC();
    void directionAlgorithmD();

    Shot::Direction getBestDirectionToAvoidEnemies();
    void move(double diffTime);
    void handleOutOfBounds();
    void updateSprite();
    void shoot();

    void loadAndBindTexture();

    sf::Texture texture;
    sf::Sprite sprite;
    Vector speed;
    Point position;
    Shot::Direction direction;

    template<typename T>
    static bool isValueInVector(const std::vector<T>& vec, const T& value)
    {
        for (const auto& element : vec)
        {
            if (element == value)
            {
                return true;
            }
        }
        return false;
    }
};

__END_API

#endif //ENEMY_H
