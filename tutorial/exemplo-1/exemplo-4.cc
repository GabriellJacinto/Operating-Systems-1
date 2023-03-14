#include <iostream>
using std::cout;
using std::endl;

#include "Time.h"

int main(){
    Time t1;
    Time t2( 2 );
    Time t3( 21, 34 );
    Time t4( 12, 25, 42 );
    Time t5( 27, 74, 99 );

    cout << "Constructed with:\n\nt1: all arguments defaulted\n ";
    t1.printUniversal();
    cout << "\n ";
    t1.printStandard();

    cout << "Constructed with:\n\nt2: hour input, rest defaulted\n ";
    t2.printUniversal();
    cout << "\n ";
    t2.printStandard();

    cout << "Constructed with:\n\nt3: hour, minute input\n ";
    t3.printUniversal();
    cout << "\n ";
    t3.printStandard();

    cout << "Constructed with:\n\nt4: all input\n ";
    t4.printUniversal();
    cout << "\n ";
    t4.printStandard();

    cout << "Constructed with:\n\nt5: invalid mumbo jambo\n ";
    t5.printUniversal();
    cout << "\n ";
    t5.printStandard();

    return 0;
}