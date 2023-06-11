#include "Game/Logic/Player.h"
#include "Concurrency/traits.h"
#include "Concurrency/thread.h"
#include "Game/Control/Config.h"
#include "SFML/Graphics.hpp"
#include "Game/Interface/window.h"
#include "Game/Logic/Shot.h"

__BEGIN_API

int Player::HALF_PLAYER_SIZE = 24;
int Player::PLAYER_SIZE = 48;
int Player::PLAYER_SPEED = 250;
float Player::SHOT_COOLDOWN = 10;
Vector Player::SHOT_SPEED = Vector(0, 500);
float Player::INVULNERABILITY_TIME = 1000;
Semaphore* Player::lifeSemaphore = new Semaphore();
Semaphore* Player::invulnerabilitySemaphore = new Semaphore();

Player::Player(KeyboardHandler* keyboardHandler)
{
    this->keyboardHandler = keyboardHandler;
    this->loadAndBindTexture();
    this->shotClock = std::make_unique<Clock>();
    this->invulnerabilityTime = INVULNERABILITY_TIME;
    this->direction = Shot::Direction::UP;
    this->insertInGame();
}

Player::~Player()
{
    delete Player::lifeSemaphore;
    delete Player::invulnerabilitySemaphore;
}

void Player::insertInGame()
{
    Window::toBeDrawnSemaphore->p();
    Window::addElementToDraw(this);
    Window::toBeDrawnSemaphore->v();
    CollisionHandler::shipsSemaphore->p();
    CollisionHandler::addShip(this);
    CollisionHandler::shipsSemaphore->v();
}

void Player::loadAndBindTexture()
{
    texture.loadFromFile("assets/Sprites/space_ships/space_ship1.png");
    sprite.setTexture(texture);
    sprite.scale(-0.5, -0.5);
    sf::Vector2u textureSize = texture.getSize();
    sprite.setOrigin(textureSize.x / 2, textureSize.y / 2);
}

void Player::run()
{
    while (!Config::finished)
    {
        this->processKeyboardInput();
        Thread::yield();
    }
}

void Player::draw(sf::RenderWindow window, double diffTime)
{
    window.draw(this->sprite);
    this->update(diffTime);
}

void Player::update(double diffTime)
{
    this->move(diffTime);
    Player::invulnerabilitySemaphore->p();
    if (this->invulnerable)
    {
        Player::invulnerabilitySemaphore->v();
        this->invulnerabilityTime -= diffTime;
        if (this->invulnerabilityTime <= 0)
        {
            Player::invulnerabilitySemaphore->p();
            this->invulnerable = false;
            Player::invulnerabilitySemaphore->v();
            this->invulnerabilityTime = INVULNERABILITY_TIME;
        }
    }
}

void Player::updateSprite()
{
    this->sprite.setPosition(this->position.x, this->position.y);
    switch (this->direction)
    {
        case Shot::Direction::UP:
            this->sprite.setRotation(0);
            break;
        case Shot::Direction::DOWN:
            this->sprite.setRotation(180);
            break;
        case Shot::Direction::LEFT:
            this->sprite.setRotation(90);
            break;
        case Shot::Direction::RIGHT:
            this->sprite.setRotation(270);
            break;
    }
}

void Player::move(double diffTime)
{
    this->position = this->position + this->speed * diffTime;
    this->updateSprite();
    this->speed = Vector(0, 0);
    this->handleOutOfBounds();
}

void Player::collide(int damage)
{
    Player::invulnerabilitySemaphore->p();
    if (invulnerable)
    {
        Player::invulnerabilitySemaphore->v();
        return;
    }
    Player::lifeSemaphore->p();
    this->life -= damage;
    Player::lifeSemaphore->v();
    Player::invulnerabilitySemaphore->p();
    this->invulnerable = true;
    Player::invulnerabilitySemaphore->v();

    if (this->isDead())
    {
        Config::gameOverSemaphore->p();
        Config::gameOver = true;
        Config::gameOverSemaphore->v();
        this->removeFromGame();
    }
}

bool Player::isOutOfPlay()
{
    return !(this->isDead());
}

bool Player::isDead()
{
    return this->life <= 0; // CHECK
}

void Player::processKeyboardInput()
{
    KeyboardHandler::eventQueueSemaphore->p();
    Play::KeyPress eventToProcess = this->keyboardHandler->getNextKey();
    switch (eventToProcess) {
        case Play::KeyPress::UP:
            this->speed.y -= PLAYER_SPEED;
            this->direction = Shot::Direction::UP;
            break;
        case Play::KeyPress::DOWN:
            this->speed.y += PLAYER_SPEED;
            this->direction = Shot::Direction::DOWN;
            break;
        case Play::KeyPress::LEFT:
            this->speed.x -= PLAYER_SPEED;
            this->direction = Shot::Direction::LEFT;
            break;
        case Play::KeyPress::RIGHT:
            this->speed.x += PLAYER_SPEED;
            this->direction = Shot::Direction::RIGHT;
            break;
        case Play::KeyPress::SHOOT:
            this->shoot(this->direction);
            break;
    }
    KeyboardHandler::eventQueueSemaphore->v();
}

void Player::shoot(Shot::Direction direction)
{
    if (this->shotClock->getElapsedTime() > SHOT_COOLDOWN)
    {
        this->shotClock->restart();
        Point shotPosition = this->position;
        Shot::Direction shotDirection = direction;
        Shot* shot = new Shot(shotPosition, SHOT_SPEED, shotDirection, true); // TEST: IS IT WORKING?
    }
}

int Player::getSize()
{
    return PLAYER_SIZE;
}

int Player::getLife() const
{
    return this->life;
}

Point Player::getPosition()
{
    return this->position;
}

void Player::removeFromGame()
{
    Window::toBeDrawnSemaphore->p();
    Window::removeElementToDraw(this);
    Window::toBeDrawnSemaphore->v();
    CollisionHandler::shipsSemaphore->p();
    CollisionHandler::removeShip(this);
    CollisionHandler::shipsSemaphore->v();
}

void Player::handleOutOfBounds()
{
    if (this->position.x > Config::playableAreaWidth - PLAYER_SIZE)
        this->position.x = Config::playableAreaWidth - PLAYER_SIZE;
    else if (this->position.x < PLAYER_SIZE)
        this->position.x = PLAYER_SIZE;
    if (this->position.y > Config::playableAreaHeight - PLAYER_SIZE)
        this->position.y = Config::playableAreaHeight - PLAYER_SIZE;
    else if (this->position.y < PLAYER_SIZE)
        this->position.y = PLAYER_SIZE;
}

__END_API
