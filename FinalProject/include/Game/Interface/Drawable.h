#ifndef DRAWABLE_H
#define DRAWABLE_H

#include <SFML/Graphics.hpp>
#include "Concurrency/traits.h"

__BEGIN_API

class Point;

class Drawable : public sf::Drawable
{
public:
    virtual ~Drawable() {}
    virtual void draw(sf::RenderWindow window, double diffTime) = 0;
    virtual void update(double diffTime) = 0;
    virtual int getSize() = 0;
    virtual Point getPosition() = 0;
};

__END_API

#endif //DRAWABLE_H
