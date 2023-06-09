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

class Window
{
public:
    Window();

    void run();

    void quit();

    void pause();

    void drawElements();

    static void addElementToDraw(Drawable* element);

    static void removeElementToDraw(Drawable* element);

    static Semaphore* toBeDrawnSemaphore;
    static Semaphore* infoSemaphore;

    sf::RenderWindow* getWindow()
    {
        return window;
    }

    ~Window() = default;

private:
    //send to each class
    void load_and_bind_textures();

    void drawInfo();
    void drawPause();

    float lastTime;
    sf::RenderWindow* window;
    static std::list<Drawable*> toBeDrawn;
    bool paused = false;
    sf::Clock clock;

    //send to each class
    // Maze Texture
    sf::Texture maze_tex;
    sf::Sprite maze_sprite;
    
    //Shot texture
    sf::Texture shot_tex;
    sf::Sprite shot_sprite;
    
    //Space ship texture
    sf::Texture space_ship_tex;
    sf::Sprite space_ship_sprite;

    //Enemy space ship texture
    sf::Texture enemy_ship_tex;
    sf::Sprite enemy_ship_sprite;
};

__END_API

#endif