#include "Concurrency/traits.h"
#include "Game/Logic/Enemy.h"
#include "Game/Logic/Player.h"
#include <cstdlib>
#include <ctime>

__BEGIN_API

int Enemy::HALF_ENEMY_SIZE = 24;
int Enemy::ENEMY_SIZE = 20;
int Enemy::ENEMY_SPEED = 50;
float Enemy::SHOT_COOLDOWN = 250;
float Enemy::DIAGONAL_TIME = 500;
float Enemy::RELIVE_TIME = 2000;
Vector Enemy::SHOT_SPEED = Vector(0, 500);
Semaphore* Enemy::isDeadSemaphore = new Semaphore();
Semaphore* Enemy::moveSemaphore = new Semaphore();
int Enemy::MINIMUM_DISTANCE = 100;
int Enemy::TARGET_DISTANCE = 150;
float Enemy::RANDOM_MOVE_TIME = 1000;

// MAKE ENEMIES NOT GO TO OUTSIDE OFTEN

Enemy::Enemy(Algorithm algorithm, Player* player, Point position)
{
    this->position = position;
    this->player = player;
    this->algorithm = algorithm;
    this->loadAndBindTexture();
    this->shotClock = std::make_unique<Clock>();
    this->reliveClock = std::make_unique<Clock>();
    this->diagonalClock = std::make_unique<Clock>();
    this->randomMoveClock = std::make_unique<Clock>();
    this->direction = Shot::Direction::UP;
    Window::addElementToDraw(this);
    CollisionHandler::addEnemy(this);
}

Enemy::~Enemy()
{
    if (Enemy::isDeadSemaphore != nullptr )
    {
        delete Enemy::isDeadSemaphore;
        Enemy::isDeadSemaphore = nullptr;
    }

    if (Enemy::moveSemaphore != nullptr)
    {
        delete Enemy::moveSemaphore;
        Enemy::moveSemaphore = nullptr;
    }
}

void Enemy::insertInGame()
{
    if (!_isDead)
    {
        return;
    }
    _isDead = false;
    //Window::toBeDrawnSemaphore->p();
    Window::addElementToDraw(this);
    //Window::toBeDrawnSemaphore->v();
    //CollisionHandler::enemySemaphore->p();
    CollisionHandler::addEnemy(this);
    //CollisionHandler::enemySemaphore->v();
}

void Enemy::loadAndBindTexture()
{
    texture.loadFromFile("assets/sprites/space_ships/enemy_space_ship1.png");
    sprite.setTexture(texture);
    sprite.scale(-0.5, -0.5);
    sf::Vector2u textureSize = texture.getSize();
    sprite.setOrigin(textureSize.x / 2, textureSize.y / 2);
}

void Enemy::run()
{
    while (!Config::finished)
    {
        //Enemy::isDeadSemaphore->p();
        if (!this->_isDead)
        {
            this->processDirectionAlgorithm();
        }
        else
        {
            if (this->reliveClock->getElapsedTime() > RELIVE_TIME)
            {
                this->insertInGame();
            }
        }
        Thread::yield();
    }
}

void Enemy::processDirectionAlgorithm()
{
    Shot::Direction directionToGo;
    if(!this->avoidingCollision)
    {
        if (this->algorithm == Algorithm::A) {
            directionToGo = this->directionAlgorithmA();
        } else if(this->algorithm == Algorithm::B){
            directionToGo = this->directionAlgorithmB();
        }
    }
    else
    {
        directionToGo = this->direction;
    }
    switch (directionToGo)
    {
        case Shot::Direction::UP:
            this->speed.y -= ENEMY_SPEED;
            this->direction = Shot::Direction::UP;
            break;
        case Shot::Direction::DOWN:
            this->speed.y += ENEMY_SPEED;
            this->direction = Shot::Direction::DOWN;
            break;
        case Shot::Direction::LEFT:
            this->speed.x -= ENEMY_SPEED;
            this->direction = Shot::Direction::LEFT;
            break;
        case Shot::Direction::RIGHT:
            this->speed.x += ENEMY_SPEED;
            this->direction = Shot::Direction::RIGHT;
            break;
    }
    this->shoot();
}

void Enemy::shoot()
{
    if (this->shotClock->getElapsedTime() > SHOT_COOLDOWN)
    {
        this->shotClock->restart();
        Point shotPosition = this->position;
        Shot::Direction shotDirection = this->direction;

        switch (shotDirection)
        {
            case Shot::Direction::UP:
                shotPosition.x += 7;
                shotPosition.y -= 25;
                break;
            case Shot::Direction::DOWN:
                shotPosition.x += 7;
                shotPosition.y += 40;
                break;
            case Shot::Direction::LEFT:
                shotPosition.x -= 25;
                shotPosition.y += 7;
                break;
            case Shot::Direction::RIGHT:
                shotPosition.x += 40;
                shotPosition.y += 7;
                break;
        }

        Shot* shot = new Shot(shotPosition, shotDirection, false);
    }
}

Shot::Direction Enemy::getRandomDirection()
{
    // Seed the random number generator
    std::srand(std::time(nullptr));

    // Generate a random number between 0 and 3
    int randomNum = std::rand() % 4;

    // Map the random number to a direction
    switch (randomNum) {
        case 0:
            return Shot::Direction::LEFT;
        case 1:
            return Shot::Direction::RIGHT;
        case 2:
            return Shot::Direction::UP;
        case 3:
            return Shot::Direction::DOWN;
        default:
            // Handle an unexpected random number (should not occur)
            return Shot::Direction::UP;  // or any default direction
    }
}

void Enemy::draw(sf::RenderWindow &window, double diffTime)
{
    window.draw(this->sprite);
    this->update(diffTime);
}

void Enemy::collide(int damage)
{
    //Enemy::isDeadSemaphore->p();
    this->reliveClock->restart();
    this->_isDead = true;
    //Enemy::isDeadSemaphore->v();
    this->removeFromGame();
}

void Enemy::update(double diffTime)
{
    this->move(diffTime);
}

void Enemy::move(double diffTime)
{
    //Enemy::moveSemaphore->p();
    this->previousPosition = this->position;
    this->position = this->position + this->speed * diffTime;
    this->updateSprite();
    this->speed = Vector(0, 0);
    this->handleOutOfBounds();
    //Enemy::moveSemaphore->v();
}

void Enemy::updateSprite()
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

Shot::Direction Enemy::directionAlgorithmA()
{
    Point A = this->getPosition();
    Point B = this->player->position;

    double dx = B.x - A.x;
    double dy = B.y - A.y;

    double minDistanceToBoundary = 20.0;

    double distanceToPlayer = std::sqrt(dx * dx + dy * dy);
    if (distanceToPlayer < minDistanceToBoundary)
        return directionAlgorithmB();

    double normalizedDx = dx / distanceToPlayer;
    double normalizedDy = dy / distanceToPlayer;

    double targetX = B.x - minDistanceToBoundary * normalizedDx;
    double targetY = B.y - minDistanceToBoundary * normalizedDy;

    bool isDiagonal = std::abs(targetX - A.x) > std::abs(targetY - A.y);

    if (!isDiagonal)
    {
        if (std::abs(targetX - A.x) > std::abs(targetY - A.y)) {
            if (targetX > A.x)
                return Shot::RIGHT;
            else
                return Shot::LEFT;
        } else {
            if (targetY > A.y)
                return Shot::DOWN;
            else
                return Shot::UP;
        }
    }
    else if(this->diagonalClock->getElapsedTime() > DIAGONAL_TIME)
    {
        this->diagonalClock->restart();

        if (std::abs(targetX - A.x) > std::abs(targetY - A.y)) {
            if (targetX > A.x)
                return Shot::RIGHT;
            else
                return Shot::LEFT;
        } else {
            if (targetY > A.y)
                return Shot::DOWN;
            else
                return Shot::UP;
        }
    }
    return this->direction;
}

Shot::Direction Enemy::directionAlgorithmB()
{
        Point currentPosition = this->getPosition();
        double minDistanceToBoundary = 50;  // Adjust this value as desired

        bool closeToLeftBoundary = currentPosition.x < 20 + minDistanceToBoundary;
        bool closeToRightBoundary = currentPosition.x > Config::playableAreaWidth - 20 - minDistanceToBoundary;
        bool closeToTopBoundary = currentPosition.y < 20 + minDistanceToBoundary;
        bool closeToBottomBoundary = currentPosition.y > Config::playableAreaHeight - 20 - minDistanceToBoundary;

        // If the enemy is already close to a boundary, return the opposite direction
        if (closeToLeftBoundary) {
            return Shot::RIGHT;}
        else if (closeToRightBoundary) {
            return Shot::LEFT;}
        else if (closeToTopBoundary){
            return Shot::DOWN;}
        else if (closeToBottomBoundary) {
            return Shot::UP;}

    if (randomMoveClock->getElapsedTime() > RANDOM_MOVE_TIME)
    {
        randomMoveClock->restart();
        // Seed the random number generator
        std::srand(std::time(nullptr));

        // Generate a random number between 0 and 3
        int randomNum = std::rand() % 4;


        // Randomly select a direction while avoiding getting too close to the boundaries
        switch (randomNum) {
            case 0:
                return Shot::LEFT;
            case 1:
                return Shot::RIGHT;
            case 2:
                return Shot::UP;
            case 3:
                return Shot::DOWN;
        }
    }
    return this->direction;
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
    //Window::toBeDrawnSemaphore->p();
    Window::removeElementToDraw(this);
    //Window::toBeDrawnSemaphore->v();
    //CollisionHandler::enemySemaphore->p();
    CollisionHandler::removeEnemy(this);
    //CollisionHandler::enemySemaphore->v();
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

void Enemy::setInitialPosition(const Point& initialPosition)
{
    this->position = initialPosition;
}

Point Enemy::getPreviousPosition()
{
    return this->previousPosition;
}

void Enemy::setPosition(const Point &newPosition)
{
    cout << "Enemy::setPosition" << endl;
    cout << "newPosition.x: " << newPosition.x << endl;
    this->position = newPosition;
}

bool Enemy::avoidCollision(Enemy* enemy1, Enemy* enemy2)
{
    Vector enemyVector = enemy2->getPosition() - enemy1->getPosition();

    double distance = enemyVector.length();

    if (distance < MINIMUM_DISTANCE && !enemy1->avoidingCollision && !enemy2->avoidingCollision)
    {
        enemy1->avoidingCollision = true;
        enemy2->avoidingCollision = true;

        if (enemy1->direction == enemy2->direction)
        {
            enemy1->inverseDirection();
        }
        else
        {
            enemy1->inverseDirection();
            enemy2->inverseDirection();
        }

        //Enemy::moveSemaphore->v();
        return true;
    }
    else if (distance >= TARGET_DISTANCE && enemy1->avoidingCollision && enemy2->avoidingCollision)
    {
        enemy1->avoidingCollision = false;
        enemy2->avoidingCollision = false;

        return false;
    }
    return false;
}

void Enemy::inverseDirection()
{
    Shot::Direction dir = this->direction;
    if (dir == Shot::UP)
        this->direction = Shot::DOWN;
    else if (dir == Shot::DOWN)
        this->direction = Shot::UP;
    else if (dir == Shot::LEFT)
        this->direction = Shot::RIGHT;
    else if (dir == Shot::RIGHT)
        this->direction = Shot::LEFT;
}

__END_API
