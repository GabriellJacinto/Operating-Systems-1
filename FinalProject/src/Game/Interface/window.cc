#include "Game/Interface/window.h"
#include "Game/Control/keyboardController.h"
#include "Concurrency/traits.h"
#include <string>
#include "Game/Control/Play.h"
#include "Game/Control/Config.h"
#include "Concurrency/thread.h"
#include "Game/Logic/Info.h"
#include "Game/Control/BrickShooter.h"
#include "Game/Interface/Sounds.h"

__BEGIN_API

vector<Drawable*> Window::toBeDrawn = vector<Drawable*>();
Semaphore* Window::toBeDrawnSemaphore = new Semaphore();
Semaphore* Window::infoSemaphore = new Semaphore();

Window::Window()
{
    maze_tex.loadFromFile("assets/sprites/maze/screen.png");
    maze_sprite.setTexture(maze_tex);
    maze_sprite.scale(1.5, 1.5);
    font.loadFromFile("assets/fonts/arial.ttf");
}

Window::~Window()
{
    delete toBeDrawnSemaphore;
    delete infoSemaphore;
}

void Window::close()
{
    this->closed = true;
}

void Window::quit()
{
    window->close();
}

void Window::pause()
{
    this->paused = !this->paused;
}

inline void Window::drawElements(double d)
{
    //toBeDrawnSemaphore->p();
    for (Drawable* element : toBeDrawn)
    {
        element->draw(*window, d);
    }
}

inline void Window::drawInfo()
{
    Info* info = BrickShooter::info;

    int score = info->score;
    int level = info->level;
    int lives = info->lives;

    sf::Text infoText;
    infoText.setFont(font);
    infoText.setCharacterSize(24);
    infoText.setFillColor(sf::Color::White);
    infoText.setPosition((float)Config::infoAreaWidth+75, 225);

    std::string infoString = "Score: " + std::to_string(score) + "\n"
                             + "Level: " + std::to_string(level) + "\n"
                             + "Lives: " + std::to_string(lives);

    infoText.setString(infoString);

    // Draw the text on the screen
    window->draw(infoText);
}


void Window::drawBackground()
{
    window->draw(maze_sprite);
}

void Window::drawPause()
{
    sf::Text pauseText;
    pauseText.setFont(font);
    pauseText.setCharacterSize(24);
    pauseText.setFillColor(sf::Color::White);
    pauseText.setPosition((float)Config::infoAreaWidth+35, 100);
    pauseText.setString("Game Paused");

    // Draw the text on the screen
    window->draw(pauseText);
}

void Window::drawGameOver()
{
    sf::Text gameOverText;
    gameOverText.setFont(font);
    gameOverText.setCharacterSize(24);
    gameOverText.setFillColor(sf::Color::White);
    gameOverText.setPosition((float)Config::infoAreaWidth+55, 100);
    gameOverText.setString("Game Over");

    // Draw the text on the screen
    window->draw(gameOverText);
}

void Window::addElementToDraw(Drawable* element)
{
   // toBeDrawnSemaphore->p();
    toBeDrawn.push_back(element);
    //toBeDrawnSemaphore->v();
}

void Window::removeElementToDraw(Drawable* element)
{
    //toBeDrawnSemaphore->p();
    toBeDrawn.erase(std::remove(toBeDrawn.begin(), toBeDrawn.end(), element), toBeDrawn.end());
    //toBeDrawnSemaphore->v();
}

void Window::run()
{
    sf::RenderWindow renderWindow(sf::VideoMode(Config::windowWidth, Config::windowHeight), "BrickShooters");
    this->window = &renderWindow;
    window->setKeyRepeatEnabled(true);
    window->setFramerateLimit(Config::fps);
    this->clock = sf::Clock();
    Sounds::playBackgroundMusic();
    while (window->isOpen())
    {
        float currentTime = clock.getElapsedTime().asSeconds();
        double diffTime = currentTime - this->lastTime;
        this->lastTime = currentTime;

        window->clear();
        drawBackground();

        if (Config::gameOver)
        {
            drawGameOver();
            drawInfo();
        }
        else if (this->paused)
        {
            drawPause();

            //infoSemaphore->p();
            drawInfo();
            //infoSemaphore->v();
        }
        else
        {
            //toBeDrawnSemaphore->p();
            drawElements(diffTime);
            //toBeDrawnSemaphore->v();

            //infoSemaphore->p();
            drawInfo();
            //infoSemaphore->v();
        }
        window->display();

        Thread::yield();

        if (this->closed)
        {
            window->close();
        }
    }
}

__END_API
