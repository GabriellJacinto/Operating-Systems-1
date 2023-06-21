#ifndef controller_h
#define controller_h

#include "Concurrency/traits.h"
#include <SFML/Graphics.hpp>
#include "Game/Logic/Player.h"
#include "Game/Logic/Enemy.h"
#include "Game/Logic/Info.h"
#include "Game/Control/keyboardController.h"
#include "Game/Control/Config.h"
#include "Game/Interface/window.h"
#include "Game/Logic/CollisionHandler.h"

__BEGIN_API

class BrickShooter
{
public:
    BrickShooter() = default;
    ~BrickShooter() = default;

    static void play(void * name);
    static void init();
    static void end();

    static void restart();
    static void pause();

    static void increaseScore();
    static void increaseLevel(const vector<Enemy*>& enemies);

    static bool shouldLevelUp();
    static Info* info;

private:

    //Objects
    static Player* player;
    static KeyboardHandler* keyboardHandler;
    static CollisionHandler* collisionHandler;
    static Window* window;
    static vector<Enemy*> enemies;

    //Threads
    static Thread* playerThread;
    static Thread* collisionHandlerThread;
    static Thread* keyboardHandlerThread;
    static Thread* windowThread;
    static vector<Thread*> enemiesThreads;

    static int killedEnemies;

    static void playerThreadFunction();
    static void collisionHandlerThreadFunction();
    static void keyboardHandlerThreadFunction();
    static void windowThreadFunction();
    static void enemiesThreadFunction(int index);
};

__END_API

#endif //controller_h
