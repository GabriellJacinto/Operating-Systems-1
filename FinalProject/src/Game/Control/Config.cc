#include "Game/Control/Config.h"
#include "Concurrency/traits.h"
#include "Concurrency/semaphore.h"

__BEGIN_API

Semaphore* Config::finishedSemaphore = new Semaphore();
Semaphore* Config::pausedSemaphore = new Semaphore();
int Config::fps = 60;
int Config::windowHeight = 560;
int Config::windowWidth = 900;
int Config::playableAreaHeight = 600;
int Config::playableAreaWidth = 600;
int Config::infoAreaHeight = 600;
int Config::infoAreaWidth = 200;
bool Config::finished = false;
bool Config::paused = false;
bool Config::bossExists = false;

__END_API