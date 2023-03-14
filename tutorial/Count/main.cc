#include <iostream>
using std::cout;
using std::endl;

#include "counter.h"

int main()
{
    Count counter;
    Count *counterPtr = &counter; // Ponteiro para counter
    Count &counterRef = counter; // Referência para counter

    cout << "Set x to 1 and print using the object's name: ";
    counter.setX( 1 );
    counterRef.print();

    cout << "Set x to 3 and print using a pointer to an object: ";
    counterPtr->setX( 3 );
    counterPtr->print();

    return 0;
}