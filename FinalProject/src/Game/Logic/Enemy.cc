#include "Concurrency/traits.h"
#include "Game/Logic/Enemy.h"
#include <cstdlib>  // For std::rand and std::srand
#include <ctime>    // For std::time

__BEGIN_API

//TODO: IMPLEMENT LAST FUNCS AND IMPLEMENT SHOTS BASED ON CLOCK FROM PROCESSALGO

int Enemy::HALF_ENEMY_SIZE = 24;
int Enemy::ENEMY_SIZE = 48;
int Enemy::ENEMY_SPEED = 250;
float Enemy::SHOT_COOLDOWN = 500;
float Enemy::RELIVE_TIME = 2000;
Vector Enemy::SHOT_SPEED = Vector(0, 500);
Semaphore* Enemy::isDeadSemaphore = new Semaphore();

Enemy::Enemy(Algorithm algorithm, Player* player)
{
    this->player = player;
    this->algorithm = algorithm;
    this->loadAndBindTexture();
    this->shotClock = std::make_unique<Clock>();
    this->direction = Shot::Direction::UP;
    this->insertInGame();
}

Enemy::~Enemy()
{
    delete Enemy::isDeadSemaphore;
}

void Enemy::insertInGame()
{
    Window::toBeDrawnSemaphore->p();
    Window::addElementToDraw(this);
    Window::toBeDrawnSemaphore->v();
    CollisionHandler::shipsSemaphore->p();
    CollisionHandler::addShip(this);
    CollisionHandler::shipsSemaphore->v();
}

void Enemy::loadAndBindTexture()
{
    texture.loadFromFile("assets/Sprites/space_ships/enemy_space_ship1.png");
    sprite.setTexture(texture);
    sprite.scale(-0.5, -0.5);
    sf::Vector2u textureSize = texture.getSize();
    sprite.setOrigin(textureSize.x / 2, textureSize.y / 2);
}

void Enemy::run()
{
    while (!Config::finished)
    {
        Enemy::isDeadSemaphore->p();
        if (!this->_isDead)
        {
            this->processDirectionAlgorithm();
        }
        else
        {
            if (reliveTime > RELIVE_TIME)
            {
                this->reliveTime -= this->reliveClock->getElapsedTime();
            }
            else
            {
                this->_isDead = false;
                this->insertInGame();
            }
        }

        Thread::yield();
    }
}

void Enemy::processDirectionAlgorithm()
{
    Shot::Direction directionToGo;
    if (this->algorithm == Algorithm::A) {
        directionToGo = this->directionAlgorithmA();
    }
    else
    {
        directionToGo = this->directionAlgorithmB();
    }
    this->shoot(directionToGo);
}

void Enemy::shoot(Shot::Direction directionToShoot)
{
    if (this->shotClock->getElapsedTime() > SHOT_COOLDOWN)
    {
        this->shotClock->restart();
        Point shotPosition = this->position;
        Shot::Direction shotDirection = directionToShoot;
        Shot* shot = new Shot(shotPosition, SHOT_SPEED, shotDirection, true); // TEST: IS IT WORKING?
    }
}

void Enemy::draw(sf::RenderWindow window, double diffTime)
{
    window.draw(this->sprite);
    this->update(diffTime);
}

void Enemy::collide(int damage)
{
    Enemy::isDeadSemaphore->p();
    this->reliveClock->restart();
    this->_isDead = true;
    this->reliveTime = RELIVE_TIME;
    Enemy::isDeadSemaphore->v();
    this->removeFromGame();
}

void Enemy::update(double diffTime)
{
    this->move(diffTime);

}

void Enemy::move(double diffTime)
{
    this->position = this->position + this->speed * diffTime;
    this->updateSprite();
    this->speed = Vector(0, 0);
    this->handleOutOfBounds();
}

void Enemy::updateSprite()
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

Shot::Direction Enemy::directionAlgorithmA() {
    Point A = this->getPosition();
    Point B = this->player->position;

    double dx = B.x - A.x;
    double dy = B.y - A.y;

    if (std::abs(dx) > std::abs(dy)) {
        if (dx > 0)
            return Shot::RIGHT;
        else
            return Shot::LEFT;
    } else {
        if (dy > 0)
            return Shot::UP;
        else
            return Shot::DOWN;
    }
}

Shot::Direction Enemy::directionAlgorithmB()
{
    // Seed the random number generator
    std::srand(std::time(nullptr));

    // Generate a random number between 0 and 3
    int randomNum = std::rand() % 4;

    // Map the random number to a direction
    switch (randomNum) {
        case 0:
            return Shot::LEFT;
        case 1:
            return Shot::RIGHT;
        case 2:
            return Shot::UP;
        case 3:
            return Shot::DOWN;
        default:
            // Handle an unexpected random number (should not occur)
            return Shot::LEFT;  // or any default direction
    }
}

bool Enemy::isOutOfPlay()
{
    return !(this->_isDead);
}

int Enemy::getSize()
{
    return Enemy::ENEMY_SIZE;
}

Point Enemy::getPosition()
{
    return this->position;
}

void Enemy::removeFromGame()
{
    Window::toBeDrawnSemaphore->p();
    Window::removeElementToDraw(this);
    Window::toBeDrawnSemaphore->v();
    CollisionHandler::shipsSemaphore->p();
    CollisionHandler::removeShip(this);
    CollisionHandler::shipsSemaphore->v();
}

void Enemy::handleOutOfBounds()
{
    if (this->position.x > Config::playableAreaWidth - ENEMY_SIZE)
        this->position.x = Config::playableAreaWidth - ENEMY_SIZE;
    else if (this->position.x < ENEMY_SIZE)
        this->position.x = ENEMY_SIZE;
    if (this->position.y > Config::playableAreaHeight - ENEMY_SIZE)
        this->position.y = Config::playableAreaHeight - ENEMY_SIZE;
    else if (this->position.y < ENEMY_SIZE)
        this->position.y = ENEMY_SIZE;
}

__END_API
