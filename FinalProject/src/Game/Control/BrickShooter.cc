#include "Game/Control/BrickShooter.h"
#include "Concurrency/traits.h"

__BEGIN_API

int BrickShooter::killedEnemies = 0;
Player* BrickShooter::player;
KeyboardHandler* BrickShooter::keyboardHandler;
CollisionHandler* BrickShooter::collisionHandler;
Window* BrickShooter::window;
vector<Enemy*> BrickShooter::enemies;
Thread* BrickShooter::playerThread;
Thread* BrickShooter::collisionHandlerThread;
Thread* BrickShooter::keyboardHandlerThread;
Thread* BrickShooter::windowThread;
vector<Thread*> BrickShooter::enemiesThreads;
Info::Info* BrickShooter::info;

void BrickShooter::play(void * name)
{
    BrickShooter::init();

    windowThread = new Thread(BrickShooter::windowThreadFunction);
    playerThread = new Thread(BrickShooter::playerThreadFunction);
    keyboardHandlerThread = new Thread(BrickShooter::keyboardHandlerThreadFunction);
    collisionHandlerThread = new Thread(BrickShooter::collisionHandlerThreadFunction);

    int index = 0;
    for (auto enemy : BrickShooter::enemies)
    {
        enemiesThreads.push_back(new Thread(BrickShooter::enemiesThreadFunction, index));
        index++;
    }

    windowThread->join();
    playerThread->join();
    keyboardHandlerThread->join();
    collisionHandlerThread->join();
    for (auto enemyThread : enemiesThreads)
    {
        enemyThread->join();
    }

    delete windowThread;
    delete playerThread;
    delete keyboardHandlerThread;
    delete collisionHandlerThread;
    for (auto enemyThread : enemiesThreads)
    {
        delete enemyThread;
    }
}

void BrickShooter::playerThreadFunction()
{
    player->run();
    delete player;
    playerThread->thread_exit(0);
}

void BrickShooter::collisionHandlerThreadFunction()
{
    collisionHandler->run();
    delete collisionHandler;
    collisionHandlerThread->thread_exit(0);
}

void BrickShooter::keyboardHandlerThreadFunction()
{
    keyboardHandler->run();
    delete keyboardHandler;
    keyboardHandlerThread->thread_exit(0);
}

void BrickShooter::enemiesThreadFunction(int index)
{
    enemies[index]->run();
    delete enemies[index];
    enemiesThreads[index]->thread_exit(0);
}

void BrickShooter::windowThreadFunction()
{
    window->run();
    delete window;
    windowThread->thread_exit(0);
}

void BrickShooter::init()
{
    info = new Info::Info();
    info->lives = 3;
    info->score = 0;
    info->level = 1;
    window = new Window();
    keyboardHandler = new KeyboardHandler(window);
    player = new Player(keyboardHandler);
    player->setInitialPosition(Point(407, 330));
    collisionHandler = new CollisionHandler();
    enemies.push_back(new Enemy(Enemy::Algorithm::A, player));
    enemies[0]->setInitialPosition(Point(0, 0));
    enemies.push_back(new Enemy(Enemy::Algorithm::B, player));
    enemies[1]->setInitialPosition(Point(814, 0));
    enemies.push_back(new Enemy(Enemy::Algorithm::A, player));
    enemies[2]->setInitialPosition(Point(0, 660));
    enemies.push_back(new Enemy(Enemy::Algorithm::B, player));
    enemies[3]->setInitialPosition(Point(814, 660));
}

bool BrickShooter::shouldLevelUp()
{
    return killedEnemies % Config::enemiesPerLevel == 0 && info->level <= 3;
}

void BrickShooter::increaseScore()
{
    Info::incraseScore(*info);
}

void BrickShooter::increaseLevel(const vector<Enemy*>& enemiesToIncrease)
{
    Info::increaseLevel(*info);
    killedEnemies = 0;
    for (auto enemy : enemiesToIncrease)
    {
        Enemy::ENEMY_SPEED += 100;
    }
}

void BrickShooter::restart()
{
    killedEnemies = 0;

    info->lives = Config::lives;
    info->score = 0;
    info->level = 1;

    player->insertInGame();

    Config::gameOverSemaphore->p();
    Config::gameOver = false;
    Config::gameOverSemaphore->v();
}

void BrickShooter::pause()
{
    Config::pausedSemaphore->p();

    Config::paused = !Config::paused;
    if (Config::paused)
    {
        playerThread->suspend();
        collisionHandlerThread->suspend();
        for (auto enemy : enemiesThreads)
        {
            enemy->suspend();
        }
    }
    else
    {
        playerThread->resume();
        collisionHandlerThread->resume();
        for (auto enemy : enemiesThreads)
        {
            enemy->resume();
        }
    }

    Config::pausedSemaphore->v();
}

__END_API
