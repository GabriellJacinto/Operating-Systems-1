#ifndef COLLISIONHANDLER_H
#define COLLISIONHANDLER_H
#include <vector>
#include "Game/Interface/Collidable.h"
#include "Concurrency/semaphore.h"
#include "Concurrency/thread.h"
#include "Concurrency/traits.h"

__BEGIN_API

class BrickShooter;
class Enemy;
class Shot;
class Player;

class CollisionHandler
{
public:
    CollisionHandler() = default;
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

private:
    static Player* player;
    static vector<Enemy*> enemies;
    static vector<Shot*> shots;

    void handleCollisions();
    void handlePlayerEnemyCollisions();
    void handleEnemyCollisions();
    void handleShotCollisions();
    bool hasCollided(Drawable* drawable1, Drawable* drawable2);

};

__END_API

#endif //COLLISIONHANDLER_H
