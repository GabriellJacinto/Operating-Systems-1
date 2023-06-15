#ifndef COLLISIONHANDLER_H
#define COLLISIONHANDLER_H
#include <vector>
#include "Game/Interface/Collidable.h"
#include "Concurrency/semaphore.h"
#include "Concurrency/thread.h"
#include "Concurrency/traits.h"
#include "Game/Control/Clock.h"
#include <memory>

__BEGIN_API

class BrickShooter;
class Enemy;
class Shot;
class Player;

class CollisionHandler
{
public:
    CollisionHandler();
    ~CollisionHandler();

    static Semaphore* playerSemaphore;
    static Semaphore* enemySemaphore;
    static Semaphore* shotsSemaphore;

    void run();

    static void addEnemy(Enemy* enemy);
    static void addShot(Shot* shot);
    static void addPlayer(Player* player);

    static void removeEnemy(Enemy* enemy);
    static void removeShot(Shot* shot);
    static void removePlayer();

    void restart();

private:
    static vector<Enemy*> enemies;
    static Player* player;
    static vector<Shot*> shots;
    static vector<Shot*> shotsToRemove;
    unique_ptr<Clock> enemyCollisionClock;

    void handleCollisions();
    void handlePlayerEnemyCollisions();
    void handleShotCollisions();
    void handleShotShotCollisions();
    void handleEnemyShotCollisions(Shot* shot);
    void handlePlayerShotCollisions(Shot* shot);
    bool hasCollided(Drawable* drawable1, Drawable* drawable2);

    template<typename T>
    static bool isPointerInVector(const std::vector<T*>& vec, const T* ptr)
    {
        for (const auto& element : vec)
        {
            if (element == ptr)
            {
                return true;
            }
        }
        return false;
    }

};

__END_API

#endif //COLLISIONHANDLER_H
