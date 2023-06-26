
#ifndef _POINT_H
#define _POINT_H

#include <cmath>
#include <ctime>
#include <cstdlib>
#include "Concurrency/traits.h"

__BEGIN_API

class Vector;

class Point
{
public:
    double x;

    double y;

    Point();

    Point(double a, double b);

    ~Point();

    Point operator+(Vector v);
    Point operator-(Vector v);
    Point operator+(Point p);
    Vector operator-(Point p);
    bool operator==(Point p);
    double distance(Point p);
    void rollRandom();
    void rollRandomX();
    void rollRandomY();
};

__END_API

#endif
