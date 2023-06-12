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

CollisionHandler::~CollisionHandler()
{
    delete playerSemaphore;
    delete enemySemaphore;
    delete shotsSemaphore;
}

void CollisionHandler::run()
{
    while(!Config::finished)
    {
        this->handleCollisions();
        Thread::yield();
    }
}

void CollisionHandler::handleCollisions()
{
    this->handlePlayerEnemyCollisions();
    this->handleEnemyCollisions();
    this->handleShotCollisions();
}

void CollisionHandler::handlePlayerEnemyCollisions()
{
    if (player == nullptr || enemies.empty())
        return;

    playerSemaphore->p();
    enemySemaphore->p();

    for (auto enemy : enemies)
    {
        if (hasCollided(player, enemy))
        {
            Point prevPos = player->getPreviousPosition();
            player->setPosition(prevPos);

            Point enemyPos = enemy->getPreviousPosition();
            enemy->setPosition(enemyPos);

            player->collide(enemy->damageGiven);
        }
    }

    enemySemaphore->v();
    playerSemaphore->v();
}

void CollisionHandler::handleEnemyCollisions()
{
    if (enemies.size() < 2 )
        return;
    for (int i = 0; i < enemies.size(); i++)
    {
        for (int j = i + 1; j < enemies.size(); j++)
        {
            Enemy::avoidCollision(enemies[i], enemies[j]);

            // Shouldn't be necessary, but just in case...
            if (this->hasCollided(enemies[i], enemies[j]))
            {
                Enemy::moveSemaphore->p();
                Point prevPos = enemies[i]->getPreviousPosition();
                enemies[i]->setPosition(prevPos);

                prevPos = enemies[j]->getPreviousPosition();
                enemies[j]->setPosition(prevPos);
                Enemy::moveSemaphore->v();
            }
        }
    }
}

void CollisionHandler::handleShotCollisions()
{
    if (shots.size() < 2)
        return;
    for (int i = 0; i < enemies.size(); i++)
    {
        for (int j = i + 1; j < enemies.size(); j++)
        {
            if (shots[i]->getIsPlayerShot() || shots[j]->getIsPlayerShot())
            {
                if (this->hasCollided(shots[i], shots[j]))
                {
                    shots[i]->removeFromGame();
                    shots[j]->removeFromGame();
                }
            }
        }
    }
    for (auto shot : shots)
    {
        if (shot->getIsPlayerShot())
        {
            for (auto enemy : enemies)
            {
                if (this->hasCollided(shot, enemy))
                {
                    shot->removeFromGame();
                    enemy->collide(shot->getDamage());
                    BrickShooter::increaseScore();
                    if (BrickShooter::shouldLevelUp())
                    {
                        BrickShooter::increaseLevel(enemies);
                    }
                }
            }
        }
        else
        {
            if (this->hasCollided(shot, player))
            {
                shot->removeFromGame();
                player->collide(shot->getDamage());
            }
        }
    }
}

bool CollisionHandler::hasCollided(Drawable *drawable1, Drawable *drawable2)
{
    int firstSize = drawable1->getSize();
    Point firstPos = drawable1->getPosition();
    int secondSize = drawable2->getSize();
    Point secondPos = drawable2->getPosition();

    return (abs(firstPos.x - secondPos.x) < (firstSize + secondSize) &&
            abs(firstPos.y - secondPos.y) < (firstSize + secondSize));
}

void CollisionHandler::addEnemy(Enemy* enemy)
{
    enemySemaphore->p();
    enemies.push_back(enemy);
    enemySemaphore->v();
}

void CollisionHandler::addShot(Shot* shot)
{
    shotsSemaphore->p();
    shots.push_back(shot);
    shotsSemaphore->v();
}

void CollisionHandler::addPlayer(Player* playerToAdd)
{
    playerSemaphore->p();
    CollisionHandler::player = playerToAdd;
    playerSemaphore->v();
}

void CollisionHandler::removeEnemy(Enemy* enemy)
{
    enemySemaphore->p();
    enemies.erase(std::remove(enemies.begin(), enemies.end(), enemy), enemies.end());
    enemySemaphore->v();
}

void CollisionHandler::removeShot(Shot* shot)
{
    shotsSemaphore->p();
    shots.erase(std::remove(shots.begin(), shots.end(), shot), shots.end());
    shotsSemaphore->v();
}

void CollisionHandler::removePlayer()
{
    playerSemaphore->p();
    CollisionHandler::player = nullptr;
    playerSemaphore->v();
}

__END_API
