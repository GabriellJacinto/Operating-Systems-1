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

void CollisionHandler::handleCollisions()
{
    if (player == nullptr)
        return;
    this->handlePlayerEnemyCollisions();
    this->handleShotCollisions();
}

void CollisionHandler::handlePlayerEnemyCollisions()
{
   // playerSemaphore->p();
    //enemySemaphore->p();
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

    //enemySemaphore->v();
    //playerSemaphore->v();
}

void CollisionHandler::handleShotCollisions()
{
    if (shots.size() < 2)
        return;
    for (int i = 0; i < shots.size()-1; i++)
    {
        for (int j = i + 1; j < shots.size()-1; j++)
        {
            if ((shots[i]->getIsPlayerShot() && !shots[j]->getIsPlayerShot()) ||
                (!shots[i]->getIsPlayerShot() && !shots[j]->getIsPlayerShot()))
            {
                if (this->hasCollided(shots[i], shots[j]))
                {
                    shots[i]->collide();
                    shots[j]->collide();
                }
            }
        }
    }
    for (auto shot : shots)
    {
        if (shot->getIsPlayerShot())
        {
            if (player == nullptr || enemies.empty())
                break;
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
                            BrickShooter::increaseLevel(enemies);
                        }
                    }
                }
            }
        }
        else
        {
            if (player == nullptr)
                break;
            if (this->hasCollided(shot, player))
            {
                shot->removeFromGame();
                player->collide(shot->getDamage());
            }
            if (enemies.empty())
                break;
            for (auto enemy : enemies)
            {
                if (this->hasCollided(shot, enemy))
                {
                    shot->removeFromGame();
                }
            }
        }
    }
}

bool CollisionHandler::hasCollided(Drawable *drawable1, Drawable *drawable2)
{
    sf::FloatRect firstBounds = drawable1->getGlobalBounds();
    sf::FloatRect secondBounds = drawable2->getGlobalBounds();

    bool isColliding = firstBounds.intersects(secondBounds);
    return isColliding;
}

void CollisionHandler::addEnemy(Enemy* enemy)
{
    //enemySemaphore->p();
    enemies.push_back(enemy);
    //enemySemaphore->v();
}

void CollisionHandler::addShot(Shot* shot)
{
    //shotsSemaphore->p();
    shots.push_back(shot);
    //shotsSemaphore->v();
}

void CollisionHandler::addPlayer(Player* playerToAdd)
{
    //playerSemaphore->p();
    CollisionHandler::player = playerToAdd;
    //playerSemaphore->v();
}

void CollisionHandler::removeEnemy(Enemy* enemy)
{
    //enemySemaphore->p();
    enemies.erase(std::remove(enemies.begin(), enemies.end(), enemy), enemies.end());
    //enemySemaphore->v();
}

void CollisionHandler::removeShot(Shot* shot)
{
    //shotsSemaphore->p();
    shots.erase(std::remove(shots.begin(), shots.end(), shot), shots.end());
    if (!CollisionHandler::isPointerInVector(shotsToRemove, shot))
    {
        shotsToRemove.push_back(shot);
    }
   //shotsSemaphore->v();
}

void CollisionHandler::removePlayer()
{
    //playerSemaphore->p();
    CollisionHandler::player = nullptr;
    //playerSemaphore->v();
}

void CollisionHandler::restart()
{
    shots.clear();
    enemies.clear();
    player = nullptr;
}

__END_API
