//definindo funções membros do header file
#include <iostream>
using std::cout;
using std::endl;
#include <stdexcept> //invalid_argument exception class 

#include <iomanip>
using std::setfill;
using std::setw;

#include "Time.h"

Time::Time( int hr, int min, int sec ) //Construtor de inicialização 
{
    setTime( hr, min, sec );
}

void Time::setTime( int h, int m, int s) 
{
    setHour( h );
    setMinute( m );
    setSecond( s );
}

void Time::setHour( int h )
{
    hour = h;
}

void Time::setMinute( int m )
{
    minute = m;
}

void Time::setSecond( int s )
{
    second = s;
}

void Time::printStandard()
{
    cout << hour << ":" << minute << ":" << second << endl;
}

void Time::printUniversal()
{
    cout << hour << ":" << minute << ":" << second << "AM/PM" << endl;
}

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
