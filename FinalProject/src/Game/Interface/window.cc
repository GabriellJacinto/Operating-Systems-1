#include "Game/Interface/window.h"
#include "Game/Control/keyboardController.h"
#include "Concurrency/traits.h"
#include <string>
#include "Game/Control/Play.h"
#include "Game/Control/Config.h"
#include "Concurrency/thread.h"

__BEGIN_API

list<Drawable*> Window::toBeDrawn = list<Drawable*>();
Semaphore* Window::toBeDrawnSemaphore = new Semaphore();
Semaphore* Window::infoSemaphore = new Semaphore();

Window::Window()
{
    sf::RenderWindow renderWindow(sf::VideoMode(900, 560), "BrickShooters");
    this->window = &renderWindow;
    window->setKeyRepeatEnabled(true);
    window->setFramerateLimit(Config::fps);
}

void Window::quit()
{
    window->close();
}

void Window::pause()
{
    this->paused = !this->paused;
}

void Window::drawElements()
{

}

void Window::drawInfo()
{

}

void Window::addElementToDraw(Drawable* element)
{
    toBeDrawnSemaphore->p();
    toBeDrawn.push_back(element);
    toBeDrawnSemaphore->v();
}

void Window::removeElementToDraw(Drawable* element)
{
    toBeDrawnSemaphore->p();
    toBeDrawn.remove(element);
    toBeDrawnSemaphore->v();
}

void Window::load_and_bind_textures()
{
    // Bind map textures
    maze_tex.loadFromFile("assets/sprites/maze/screen.png");
    maze_sprite.setTexture(maze_tex);
    maze_sprite.scale(1.5, 1.5);

    shot_tex.loadFromFile("assets/sprites/space_ships/shot.png");
    shot_sprite.setTexture(shot_tex);
    shot_sprite.scale(-0.5, -0.5);

    space_ship_tex.loadFromFile("assets/sprites/space_ships/space_ship1.png");
    space_ship_sprite.setTexture(space_ship_tex);
    space_ship_sprite.scale(-0.5, -0.5);

    enemy_ship_tex.loadFromFile("assets/sprites/space_ships/enemy_space_ship1.png");
    enemy_ship_sprite.setTexture(enemy_ship_tex);
    enemy_ship_sprite.scale(-0.5, -0.5);
}

void Window::run()
{
    while (window->isOpen())
    {
        float currentTime = clock.getElapsedTime().asSeconds();
        double diffTime = currentTime - this->lastTime;

        window->clear();

        if (this->paused)
        {
            drawPause();

            infoSemaphore->p();
            drawInfo();
            infoSemaphore->v();
        }
        else
        {
            toBeDrawnSemaphore->p();
            drawElements();
            toBeDrawnSemaphore->v();

            infoSemaphore->p();
            drawInfo();
            infoSemaphore->v();
        }

        window->display();

        Thread::yield();

//        window.draw(maze_sprite);
//
//        space_ship_sprite.setPosition(220, 365);
//        window.draw(space_ship_sprite);
//
//        enemy_ship_sprite.setPosition(245, 150);
//        window.draw(enemy_ship_sprite);
//
//        shot_sprite.setPosition(204, 400);
//        window.draw(shot_sprite);

    }
}

__END_API
