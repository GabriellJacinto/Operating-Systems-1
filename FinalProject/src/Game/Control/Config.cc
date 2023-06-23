#include "Game/Control/Config.h"
#include "Concurrency/traits.h"
#include "Concurrency/semaphore.h"

__BEGIN_API

Semaphore* Config::finishedSemaphore = new Semaphore();
Semaphore* Config::pausedSemaphore = new Semaphore();
Semaphore* Config::gameOverSemaphore = new Semaphore();

int Config::fps = 60;
int Config::windowHeight = 560;
int Config::windowWidth = 814;
int Config::playableAreaHeight = 600;
int Config::playableAreaWidth = 600;
int Config::infoAreaWidth = 569;
int Config::enemiesPerLevel = 4;
int Config::maxLevel = 3;
int Config::lives = 3;
bool Config::finished = false;
bool Config::paused = false;
bool Config::gameOver = false;

void Config::deleteSemaphores()
{
    delete finishedSemaphore;
    delete pausedSemaphore;
    delete gameOverSemaphore;
}

__END_API