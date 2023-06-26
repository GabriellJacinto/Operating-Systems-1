#ifndef VECTOR_H
#define VECTOR_H

#include "Concurrency/traits.h"

__BEGIN_API

class Point;
class Vector
{
public:
    double x;
    double y;

    Vector();
    Vector(double a, double b);
    ~Vector();

    Vector operator*(double scalar);
    Vector operator/(double scalar);
    Vector operator+(Vector v);
    void rollRandom();
    void rollReallyRandom();
    int length();

    void rollRandomY();
    void rollRandomX();
    void reflectY();
    void reflectX();
    void flip();
    void Angle(Point p, Point p2, double time);
};

__END_API

#endif