#include "Game/Control/BrickShooter.h"
#include "Concurrency/system.h"
#include "Concurrency/traits.h"
#include "Game/Control/Config.h"

__USING_API

int main(void)
{
//    pthread_attr_t attr;
//    pthread_attr_init(&attr);
//
//    size_t stackSize = 16 * 1024 * 1024;  // 16 MB
//    pthread_attr_setstacksize(&attr, stackSize);

    // Necessary to avoid stack related errors. Small stack makes SFML unable to run. But big ones lead to segmentation faults at the end of the program.
    sf::RenderWindow window(sf::VideoMode(Config::windowWidth, Config::windowHeight), "Brick Shooter");
    window.close();

    // Starts the game
    System::init(&BrickShooter::play);
    cout << "Game finifffshed" << endl;
    //pthread_attr_destroy(&attr);
}
