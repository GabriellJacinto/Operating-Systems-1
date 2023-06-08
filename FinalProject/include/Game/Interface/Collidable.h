#ifndef HITTABLE_H
#define HITTABLE_H

#include "Concurrency/traits.h"
#include "Game/Interface/Drawable.h"

__BEGIN_API

    class Collidable : public Drawable
    {
    public:
        virtual ~Collidable() {}

        virtual void collide(int damage) = 0;
        virtual bool isDead() = 0;
    };

__END_API

#endif