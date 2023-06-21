#include "Concurrency/traits.h"
#include "Game/Logic/Enemy.h"
#include "Game/Logic/Player.h"
#include <cstdlib>
#include <ctime>
#include <random>
#include "Game/Interface/Sounds.h"

__BEGIN_API

int Enemy::ENEMY_SIZE = 48;
int Enemy::ENEMY_SPEED = 25;
float Enemy::SHOT_COOLDOWN = 1000;
float Enemy::DIAGONAL_TIME = 1000;
float Enemy::RELIVE_TIME = 2000;
Semaphore* Enemy::isDeadSemaphore = new Semaphore();
Semaphore* Enemy::moveSemaphore = new Semaphore();
int Enemy::MINIMUM_DISTANCE = 100;
int Enemy::TARGET_DISTANCE = 150;
float Enemy::RANDOM_MOVE_TIME = 1000;
float Enemy::HIT_ANIMATION_TIME = 20;

std::unique_ptr<Clock> Enemy::avoidCollisionClock = std::make_unique<Clock>();

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
    this->drawDamagedEnemyClock = std::make_unique<Clock>();
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
        if (!Config::gameOver && !Config::paused)
        {
            if (!this->_isDead) {
                this->processDirectionAlgorithm();
            } else {
                if (this->reliveClock->getElapsedTime() > RELIVE_TIME) {
                    this->_isDead = false;
                }
            }
        }
        Thread::yield();
    }
}

inline void Enemy::processDirectionAlgorithm()
{
    Shot::Direction directionToGo;

    if (!this->avoidingCollision)
    {
        if (this->algorithm == Algorithm::A) {
            this->directionAlgorithmA();
        } else if (this->algorithm == Algorithm::B) {
            this->directionAlgorithmB();
        } else if (this->algorithm == Algorithm::C) {
            this->directionAlgorithmC();
        } else if (this->algorithm == Algorithm::D) {
            this->directionAlgorithmD();
        }
    }

    directionToGo = this->getBestDirectionToAvoidEnemies();

    Point currentPosition = this->getPosition();
    double minDistanceToBoundary = 25;  // Adjust this value as desired
    bool closeToLeftBoundary = currentPosition.x < 20 + minDistanceToBoundary;
    bool closeToRightBoundary = currentPosition.x > Config::playableAreaWidth - 20 - minDistanceToBoundary;
    bool closeToTopBoundary = currentPosition.y < 20 + minDistanceToBoundary;
    bool closeToBottomBoundary = currentPosition.y > Config::playableAreaHeight - 20 - minDistanceToBoundary;

    // If the enemy is already close to a boundary, return the opposite direction
    if (closeToLeftBoundary) {
        directionToGo = Shot::RIGHT;}
    else if (closeToRightBoundary) {
        directionToGo = Shot::LEFT;}
    else if (closeToTopBoundary){
        directionToGo = Shot::DOWN;}
    else if (closeToBottomBoundary) {
        directionToGo = Shot::UP;}

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
        Shot* shot = new Shot(shotPosition, shotDirection, false);
    }
}

void Enemy::draw(sf::RenderWindow &window, double diffTime)
{
    if (_isDead)
    {
        if (drawDamagedEnemyClock->getElapsedTime() < HIT_ANIMATION_TIME)
        {
            return;
        }
        else
        {
            drawDamagedEnemyClock->restart();
        }
    }
    window.draw(this->sprite);
    this->update(diffTime);
}

void Enemy::collide(int damage)
{
    //Enemy::isDeadSemaphore->p();
    this->reliveClock->restart();
    this->_isDead = true;
    //Enemy::isDeadSemaphore->v();
    Sounds::playHitSound();
}

void Enemy::update(double diffTime)
{
    if (!this->isStuck)
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

void Enemy::directionAlgorithmA()
{
    Point A = this->getPosition();
    Point B = this->player->position;

    double dx = B.x - A.x;
    double dy = B.y - A.y;

    double minDistanceToBoundary = 100.0;

    double distanceToPlayer = std::sqrt(dx * dx + dy * dy);
    if (distanceToPlayer < minDistanceToBoundary)
    {
        return directionAlgorithmB();
    }
    double normalizedDx = dx / distanceToPlayer;
    double normalizedDy = dy / distanceToPlayer;

    double targetX = B.x - minDistanceToBoundary * normalizedDx;
    double targetY = B.y - minDistanceToBoundary * normalizedDy;

    bool isDiagonal = std::abs(targetX - A.x) > std::abs(targetY - A.y);

    if (!isDiagonal)
    {
        if (std::abs(targetX - A.x) > std::abs(targetY - A.y)) {
            if (targetX > A.x)
                this->direction = Shot::RIGHT;
            else
                this->direction = Shot::LEFT;
        } else {
            if (targetY > A.y)
                this->direction = Shot::DOWN;
            else
                this->direction = Shot::UP;
        }
    }
    else if(this->diagonalClock->getElapsedTime() > DIAGONAL_TIME)
    {
        this->diagonalClock->restart();

        if (std::abs(targetX - A.x) > std::abs(targetY - A.y)) {
            if (targetX > A.x)
                this->direction = Shot::RIGHT;
            else
                this->direction = Shot::LEFT;
        } else {
            if (targetY > A.y)
                this->direction = Shot::DOWN;
            else
                this->direction = Shot::UP;
        }
    }
}

void Enemy::directionAlgorithmB()
{
    if (randomMoveClock->getElapsedTime() > RANDOM_MOVE_TIME)
    {
        randomMoveClock->restart();

        std::random_device rd;
        ::mt19937 gen(rd());

        std::uniform_int_distribution<> dis(0, 3);
        int dir = dis(gen);

        switch (dir) {
            case 0:
                this->direction = Shot::LEFT;
                break;
            case 1:
                this->direction = Shot::RIGHT;
                break;
            case 2:
                this->direction = Shot::UP;
                break;
            case 3:
                this->direction = Shot::DOWN;
                break;
        }
    }
}

void Enemy::directionAlgorithmC()
{
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
                this->direction = Shot::LEFT;
                break;
            case 1:
                this->direction = Shot::RIGHT;
                break;
            case 2:
                this->direction = Shot::UP;
                break;
            case 3:
                this->direction = Shot::DOWN;
                break;
        }
    }
}

void Enemy::directionAlgorithmD()
{
    Point A = this->getPosition();
    Point B = this->player->position;

    double dx = B.x - A.x;
    double dy = B.y - A.y;

    double distanceToPlayer = std::sqrt(dx * dx + dy * dy);

    double normalizedDx = dx / distanceToPlayer;
    double normalizedDy = dy / distanceToPlayer;

    double targetX = B.x - normalizedDx;
    double targetY = B.y - normalizedDy;

    bool isDiagonal = std::abs(targetX - A.x) > std::abs(targetY - A.y);

    if (!isDiagonal)
    {
        if (std::abs(targetX - A.x) > std::abs(targetY - A.y)) {
            if (targetX > A.x)
                this->direction = Shot::RIGHT;
            else
                this->direction = Shot::LEFT;
        } else {
            if (targetY > A.y)
                this->direction = Shot::DOWN;
            else
                this->direction = Shot::UP;
        }
    }
    else if(this->diagonalClock->getElapsedTime() > DIAGONAL_TIME)
    {
        this->diagonalClock->restart();

        if (std::abs(targetX - A.x) > std::abs(targetY - A.y)) {
            if (targetX > A.x)
                this->direction = Shot::RIGHT;
            else
                this->direction = Shot::LEFT;
        } else {
            if (targetY > A.y)
                this->direction = Shot::DOWN;
            else
                this->direction = Shot::UP;
        }
    }
}

Shot::Direction Enemy::getBestDirectionToAvoidEnemies()
{
    Point enemy1Position = this->enemyToAvoid1->getCenter();
    Point enemy2Position = this->enemyToAvoid2->getCenter();
    Point enemy3Position = this->enemyToAvoid3->getCenter();

    Point currentPosition = this->getPosition();

    double distanceToEnemy1 = (currentPosition - enemy1Position).length();
    double distanceToEnemy2 = (currentPosition - enemy2Position).length();
    double distanceToEnemy3 = (currentPosition - enemy3Position).length();

    std::vector<double> distances = {distanceToEnemy1, distanceToEnemy2, distanceToEnemy3};
    std::vector<Enemy*> enemies = {this->enemyToAvoid1, this->enemyToAvoid2, this->enemyToAvoid3};

    int minIndex = std::min_element(distances.begin(), distances.end()) - distances.begin();

    Enemy* closestEnemy = enemies[minIndex];

    double minDistanceToBoundary = 25;  // Adjust this value as desired

    // Used to check if the enemy is stuck in a corner
    bool closeToLeftBoundary = currentPosition.x < 20 + minDistanceToBoundary;
    bool closeToRightBoundary = currentPosition.x > Config::playableAreaWidth - 20 - minDistanceToBoundary;
    bool closeToTopBoundary = currentPosition.y < 20 + minDistanceToBoundary;
    bool closeToBottomBoundary = currentPosition.y > Config::playableAreaHeight - 20 - minDistanceToBoundary;

    if (!closeToLeftBoundary && !closeToRightBoundary && !closeToTopBoundary && !closeToBottomBoundary)
        isStuck = false;

    if (distances[minIndex] < MINIMUM_DISTANCE)
    {
        Point currentPosition = this->getPosition();

        this->avoidingCollision = true;
        Shot::Direction closestEnemyDirection = closestEnemy->direction;

        vector<Shot::Direction> verticalDirs = {Shot::UP, Shot::DOWN};
        vector<Shot::Direction> horizontalDirs = {Shot::LEFT, Shot::RIGHT};
        if (closestEnemy->getPosition().x > currentPosition.x && closestEnemy->getPosition().y > currentPosition.y)
        {
            if (closeToLeftBoundary || closeToTopBoundary)
                isStuck = true;

            if (isValueInVector(verticalDirs, closestEnemyDirection))
            {
                return Shot::LEFT;
            }
            else if (isValueInVector(horizontalDirs, closestEnemyDirection))
            {
                return Shot::UP;
            }
        }
        else if (closestEnemy->getPosition().x < currentPosition.x && closestEnemy->getPosition().y > currentPosition.y)
        {
            if (closeToRightBoundary || closeToTopBoundary)
                isStuck = true;

            if (isValueInVector(verticalDirs, closestEnemyDirection))
            {
                return Shot::RIGHT;
            }
            else if (isValueInVector(horizontalDirs, closestEnemyDirection))
            {
                return Shot::UP;
            }
        }
        else if (closestEnemy->getPosition().x <= currentPosition.x && closestEnemy->getPosition().y <= currentPosition.y)
        {
            if (closeToRightBoundary || closeToBottomBoundary)
                isStuck = true;

            if (isValueInVector(verticalDirs, closestEnemyDirection))
            {
                return Shot::RIGHT;
            }
            else if (isValueInVector(horizontalDirs, closestEnemyDirection))
            {
                return Shot::DOWN;
            }
        }
        else if (closestEnemy->getPosition().x >= currentPosition.x && closestEnemy->getPosition().y <= currentPosition.y)
        {
            if (closeToLeftBoundary || closeToBottomBoundary)
                isStuck = true;

            if (isValueInVector(verticalDirs, closestEnemyDirection))
            {
                return Shot::LEFT;
            }
            else if (isValueInVector(horizontalDirs, closestEnemyDirection))
            {
                return Shot::DOWN;
            }
        }
    }
    else
    {
        if (distances[minIndex] > TARGET_DISTANCE)
        {
            this->avoidingCollision = false;
        }
        return this->direction;
    }
}

Shot::Direction Enemy::inverseDirection(Shot::Direction dir)
{
    switch (dir) {
        case Shot::LEFT:
            return Shot::RIGHT;
        case Shot::RIGHT:
            return Shot::LEFT;
        case Shot::UP:
            return Shot::DOWN;
        case Shot::DOWN:
            return Shot::UP;
    }
}

void Enemy::setEnemiesToAvoid(Enemy *enemy1, Enemy *enemy2, Enemy *enemy3)
{
    this->enemyToAvoid1 = enemy1;
    this->enemyToAvoid2 = enemy2;
    this->enemyToAvoid3 = enemy3;
}

int Enemy::getSize()
{
    return Enemy::ENEMY_SIZE;
}

Point Enemy::getPosition()
{
    return this->position;
}

Point Enemy::getCenter()
{
    return {this->position.x + ENEMY_SIZE / 2, this->position.y + ENEMY_SIZE / 2};
}

void Enemy::handleOutOfBounds()
{
    float leftBound = this->position.x - ENEMY_SIZE*0.75;
    float rightBound = this->position.x + ENEMY_SIZE*1.25;

    if (rightBound > Config::playableAreaWidth)
        this->position.x -= rightBound - Config::playableAreaWidth;
    else if (leftBound < 0)
        this->position.x -= leftBound;

    float topBound = this->position.y - ENEMY_SIZE*0.75;
    float bottomBound = this->position.y + ENEMY_SIZE*1.5;

    if (bottomBound > Config::playableAreaHeight)
        this->position.y -= bottomBound - Config::playableAreaHeight;
    else if (topBound < 0)
        this->position.y -= topBound;
}

Point Enemy::getPreviousPosition()
{
    return this->previousPosition;
}

void Enemy::setPosition(const Point &newPosition)
{
    this->position = newPosition;
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

void Enemy::setDirection(Shot::Direction dir)
{
    this->direction = dir;
}

sf::FloatRect Enemy::getGlobalBounds()
{
    return this->sprite.getGlobalBounds();
}

__END_API
