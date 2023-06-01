#include "Game/window.h"
#include "Game/keyboardHandler.h"
#include "Concurrency/traits.h"
#include <string>

__BEGIN_API

Window::Window()
{
    load_and_bind_textures();
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

void Window::draw_texture(unsigned int texture, int length, int height, float angle)
{

}

void Window::run()
{
    sf::RenderWindow window(sf::VideoMode(900, 560), "SFML works!");

    //Link: https://www.sfml-dev.org/tutorials/2.5/window-events.php
    //https://www.sfml-dev.org/documentation/2.5.1/classsf_1_1Keyboard.php
    window.setKeyRepeatEnabled(false);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            string pressedKey = KeyboardHandler::getPressedKey(event);

        }

        window.clear();
        window.draw(maze_sprite);
        
        space_ship_sprite.setPosition(220, 365);
        window.draw(space_ship_sprite);
        
        enemy_ship_sprite.setPosition(245, 150);
        window.draw(enemy_ship_sprite);

        shot_sprite.setPosition(204, 400);
        window.draw(shot_sprite);
        
        window.display();
    }
}

__END_API
