#include "Game/Logic/Player.h"
#include "Concurrency/traits.h"
#include "Concurrency/thread.h"
#include "Game/Control/Config.h"
#include "SFML/Graphics.hpp"
#include "Game/Interface/window.h"
#include "Game/Logic/Shot.h"
#include "Game/Control/BrickShooter.h"
#include "Game/Interface/Sounds.h"

__BEGIN_API

int Player::HALF_PLAYER_SIZE = 24;
int Player::PLAYER_SIZE = 48;
int Player::PLAYER_SPEED = 250;
float Player::SHOT_COOLDOWN = 200;
float Player::INVULNERABILITY_TIME = 3.0;
float Player::HIT_ANIMATION_TIME = 20;
Semaphore* Player::lifeSemaphore = new Semaphore();
Semaphore* Player::invulnerabilitySemaphore = new Semaphore();
Semaphore* Player::moveSemaphore = new Semaphore();

Player::Player(KeyboardHandler* keyboardHandler)
{
    this->position = Point((float)Config::playableAreaWidth / 2, (float)Config::playableAreaHeight / 2);
    this->keyboardHandler = keyboardHandler;
    this->loadAndBindTexture();
    this->shotClock = std::make_unique<Clock>();
    this->invulnerabilityTime = INVULNERABILITY_TIME;
    this->direction = Shot::Direction::UP;
    this->life = 3;
    Window::addElementToDraw(this);
    CollisionHandler::addPlayer(this);
    drawDamagedPlayerClock = std::make_unique<Clock>();
}

Player::~Player()
{
    delete Player::lifeSemaphore;
    delete Player::invulnerabilitySemaphore;
    delete Player::moveSemaphore;
}

void Player::insertInGame()
{
    this->life = 3;
    //Window::toBeDrawnSemaphore->p();
    Window::addElementToDraw(this);
    //Window::toBeDrawnSemaphore->v();
    //CollisionHandler::playerSemaphore->p();
    CollisionHandler::addPlayer(this);
    //CollisionHandler::playerSemaphore->v();
    this->keyboardHandler->saveEvents = true;
    this->position = Point((float)Config::playableAreaWidth / 2, (float)Config::playableAreaHeight / 2);
    this->updateSprite();
    this->invulnerable = false;
}

void Player::loadAndBindTexture()
{
    texture.loadFromFile("assets/sprites/space_ships/space_ship1.png");
    sprite.setTexture(texture);
    sprite.scale(-0.5, -0.5);
    sf::Vector2u textureSize = texture.getSize();
    sprite.setOrigin(textureSize.x / 2, textureSize.y / 2);
}

void Player::run()
{
    while (!Config::finished)
    {
        if (!Config::gameOver && !Config::paused)
        {
            this->processKeyboardInput();
        }
        Thread::yield();
    }
}

void Player::draw(sf::RenderWindow &window, double diffTime)
{
    if (invulnerable)
    {
        if (drawDamagedPlayerClock->getElapsedTime() < HIT_ANIMATION_TIME)
        {
            handleInvulnerability(diffTime);
            return;
        }
        else
        {
            drawDamagedPlayerClock->restart();
        }
    }
    this->update(diffTime);
    window.draw(this->sprite);
}

void Player::update(double diffTime)
{
    this->move(diffTime);

    handleInvulnerability(diffTime);
}

void Player::handleInvulnerability(double diffTime)
{
    if (this->invulnerable)
    {
        this->invulnerabilityTime -= diffTime;
        if (this->invulnerabilityTime <= 0)
        {
            this->invulnerable = false;
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
            this->sprite.setRotation(180);
            break;
        case Shot::Direction::DOWN:
            this->sprite.setRotation(0);
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
    Player::moveSemaphore->p();
    this->previousPosition = this->position;
    this->position = this->position + this->speed * diffTime;
    this->speed = Vector(0, 0);
    this->updateSprite();
    this->handleOutOfBounds();
    Player::moveSemaphore->v();
}

void Player::collide(int damage)
{
    if (invulnerable)
        return;

    this->life -= damage;
    BrickShooter::info->decreaseLives();

    this->invulnerable = true;
    this->invulnerabilityTime = INVULNERABILITY_TIME;
    this->drawDamagedPlayerClock->restart();
    Sounds::playHitSound();

    if (this->isDead())
    {
        Config::gameOver = true;
        Sounds::playGameOverSound();
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

inline void Player::processKeyboardInput() {
    //KeyboardHandler::eventQueueSemaphore->p();
    KeyboardHandler::keys eventToProcess = this->keyboardHandler->getNextKey();

    Play::KeyPress event1 = eventToProcess.moveKey;
    Play::KeyPress event2 = eventToProcess.actionKey;

    processKey(event1);
    processKey(event2);
}

void Player::processKey(Play::KeyPress eventToProcess)
{
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
    //KeyboardHandler::eventQueueSemaphore->v();
}

void Player::shoot(Shot::Direction directionToShoot)
{
    if (this->shotClock->getElapsedTime() > SHOT_COOLDOWN)
    {
        this->shotClock->restart();
        Point shotPosition = this->position;
        Shot::Direction shotDirection = directionToShoot;

        switch (shotDirection)
        {
            case Shot::Direction::UP:
                shotPosition.y -= 40;
                break;
            case Shot::Direction::DOWN:
                shotPosition.y += 40;
                break;
            case Shot::Direction::LEFT:
                shotPosition.x -= 40;
                break;
            case Shot::Direction::RIGHT:
                shotPosition.x += 40;
                break;
        }

        Shot* shot = new Shot(shotPosition, shotDirection, true); // TEST: IS IT WORKING?
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
    //Window::toBeDrawnSemaphore->p();
    Window::removeElementToDraw(this);
    //Window::toBeDrawnSemaphore->v();
    //CollisionHandler::playerSemaphore->p();
    CollisionHandler::removePlayer();
    //CollisionHandler::playerSemaphore->v();
    this->keyboardHandler->saveEvents = false;
}

void Player::handleOutOfBounds()
{
    float leftBound = this->position.x - PLAYER_SIZE*0.75;
    float rightBound = this->position.x + PLAYER_SIZE*1.25;

    if (rightBound > Config::playableAreaWidth)
        this->position.x -= rightBound - Config::playableAreaWidth;
    else if (leftBound < 0)
        this->position.x -= leftBound;

    float topBound = this->position.y - PLAYER_SIZE*0.75;
    float bottomBound = this->position.y + PLAYER_SIZE*1.5;

    if (bottomBound > Config::playableAreaHeight)
        this->position.y -= bottomBound - Config::playableAreaHeight;
    else if (topBound < 0)
        this->position.y -= topBound;
}

void Player::setInitialPosition(Point initialPosition)
{
    this->position = initialPosition;
    this->updateSprite();
}

Point Player::getPreviousPosition()
{
    return this->previousPosition;
}

void Player::setPosition(const Point &newPosition)
{
    this->position = newPosition;
}

sf::FloatRect Player::getGlobalBounds()
{
    return this->sprite.getGlobalBounds();
}

__END_API
