#include "Concurrency/traits.h"
#include "Game/Logic/CollisionHandler.h"
#include "Game/Logic/Point.h"
#include "Game/Control/BrickShooter.h"
#include "Game/Logic/Enemy.h"
#include "Game/Logic/Player.h"
#include "Game/Logic/Shot.h"

__BEGIN_API

Semaphore* CollisionHandler::playerSemaphore = new Semaphore();
Semaphore* CollisionHandler::enemySemaphore = new Semaphore();
Semaphore* CollisionHandler::shotsSemaphore = new Semaphore();
Player* CollisionHandler::player;
vector<Enemy*> CollisionHandler::enemies;
vector<Shot*> CollisionHandler::shots;
vector <Shot*> CollisionHandler::shotsToRemove;

CollisionHandler::CollisionHandler()
{
    enemyCollisionClock = make_unique<Clock>();
}

CollisionHandler::~CollisionHandler()
{
    delete playerSemaphore;
    delete enemySemaphore;
    delete shotsSemaphore;

    for (auto shot: shots)
    {
        delete shot;
    }

    for (auto shot: shotsToRemove)
    {
        delete shot;
    }
}

void CollisionHandler::run()
{
    while(!Config::finished)
    {
        if (!Config::gameOver && !Config::paused)
        {
            this->handleCollisions();
        }
        for (auto it = shotsToRemove.begin(); it != shotsToRemove.end();)
        {
            auto shot = *it;
            it = shotsToRemove.erase(it);
            delete shot;
        }
        Thread::yield();
    }
}

inline void CollisionHandler::handleCollisions()
{
    if (player == nullptr)
        return;
    this->handlePlayerEnemyCollisions();
    this->handleShotCollisions();
}

inline void CollisionHandler::handlePlayerEnemyCollisions()
{
    if (player == nullptr || enemies.size() == 0)
        return;
    for (auto enemy : enemies)
    {
        if (hasCollided(player, enemy))
        {
            Point prevPos = player->getPreviousPosition();
            player->setPosition(prevPos);

            Point enemyPos = enemy->getPreviousPosition();
            enemy->setPosition(enemyPos);

            if (!enemy->isDead())
                player->collide(enemy->damageGiven);

            if (player == nullptr)
                break;
        }
    }
}

inline void CollisionHandler::handleShotCollisions()
{
    handleShotShotCollisions();
    for (auto shot : shots)
    {
        if (shot->getIsPlayerShot())
        {
            handleEnemyShotCollisions(shot);
        }
        else
        {
            handlePlayerShotCollisions(shot);
        }
    }
}

inline void CollisionHandler::handleShotShotCollisions()
{
    int shotsInGame = shots.size();
    if (shotsInGame < 2)
        return;
    for (int i = 0; i < shots.size(); i++)
    {
        for (int j = i + 1; j < shots.size(); j++)
        {
            if (this->hasCollided(shots[i], shots[j]))
            {
                shots[i]->collide();
                shots[j]->collide();
            }
        }
    }
}

inline void CollisionHandler::handleEnemyShotCollisions(Shot* shot)
{
    if (player == nullptr || enemies.empty())
        return;
    for (auto enemy : enemies)
    {
        if (this->hasCollided(shot, enemy))
        {
            shot->removeFromGame();
            if (!enemy->isDead())
            {
                enemy->collide(shot->getDamage());
                BrickShooter::increaseScore();
                if (BrickShooter::shouldLevelUp()) {
                    BrickShooter::increaseLevel();
                }
            }
        }
    }
}

inline void CollisionHandler::handlePlayerShotCollisions(SOLUTION::Shot *shot)
{
    if (player == nullptr)
        return;
    if (this->hasCollided(shot, player))
    {
        shot->removeFromGame();
        player->collide(shot->getDamage());
    }
    if (enemies.empty())
        return;
    for (auto enemy : enemies)
    {
        if (this->hasCollided(shot, enemy))
        {
            shot->removeFromGame();
        }
    }
}

/* All collidable objects are squares, thus this function detects the collision between squares.
 * But since the ships do not move diagonally, it doesn't consider the diagonal distance between squares*/
inline bool CollisionHandler::hasCollided(Drawable* drawable1, Drawable* drawable2)
{
    Point pos1 = drawable1->getPosition();
    Point pos2 = drawable2->getPosition();
    int L1 = drawable1->getSize();
    int L2 = drawable2->getSize();

    Vector pathVector = pos1 - pos2;
    double distance = pathVector.length();

    if (distance < (L1 + L2)/2)
    {
        return true;
    }
    return false;
}

void CollisionHandler::addEnemy(Enemy* enemy)
{
    enemies.push_back(enemy);
}

void CollisionHandler::addShot(Shot* shot)
{
    shots.push_back(shot);
}

void CollisionHandler::addPlayer(Player* playerToAdd)
{
    CollisionHandler::player = playerToAdd;
}

void CollisionHandler::removeEnemy(Enemy* enemy)
{
    enemies.erase(std::remove(enemies.begin(), enemies.end(), enemy), enemies.end());
}

void CollisionHandler::removeShot(Shot* shot)
{
    shots.erase(std::remove(shots.begin(), shots.end(), shot), shots.end());
    if (!CollisionHandler::isPointerInVector(shotsToRemove, shot))
    {
        shotsToRemove.push_back(shot);
    }
}

void CollisionHandler::removePlayer()
{
    CollisionHandler::player = nullptr;
}

void CollisionHandler::restart()
{
    for (auto it = shots.begin(); it != shots.end();)
    {
        auto shot = *it;
        it = shots.erase(it);
        shot->removeFromGame();
    }
    player = nullptr;
}

__END_API
