#include "Game/Control/BrickShooter.h"
#include "Concurrency/system.h"
#include "Concurrency/traits.h"
#include "Game/Control/Config.h"

__USING_API

int main(void)
{
    // Necessary to avoid stack related errors. Small stack makes SFML unable to run. But big ones lead to segmentation faults at the end of the program.
    sf::RenderWindow window(sf::VideoMode(Config::windowWidth, Config::windowHeight), "Brick Shooter");
    window.close();

    // Starts the game
    System::init(&BrickShooter::play);

    return 0;
}
