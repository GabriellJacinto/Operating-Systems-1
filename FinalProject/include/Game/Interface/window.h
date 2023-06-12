#ifndef window_h
#define window_h

#include <iostream>
#include <png.h>
#include <SFML/Graphics.hpp>
#include "Concurrency/traits.h"
#include <list>
#include "Game/Interface/Drawable.h"
#include "Concurrency/semaphore.h"

__BEGIN_API

class BrickShooter;

class Window
{
public:
    Window();

    void run();

    void quit();

    void pause();

    void drawElements(double d);

    static void addElementToDraw(Drawable* element);

    static void removeElementToDraw(Drawable* element);

    static Semaphore* toBeDrawnSemaphore;
    static Semaphore* infoSemaphore;

    sf::RenderWindow* getWindow()
    {
        return window;
    }

    ~Window();

private:
    void drawInfo();
    void drawPause();
    void drawGameOver();
    void drawBackground();

    float lastTime;
    sf::RenderWindow* window;
    static std::list<Drawable*> toBeDrawn;
    bool paused = false;
    sf::Clock clock;
    sf::Texture maze_tex;
    sf::Sprite maze_sprite;
    sf::Font font;
};

__END_API

#endif