#include <iostream>
#include <iomanip>
#include <stdexcept> //invalid_argument exception class 
#include "Time.h"

using namespace std;

Time::Time()
{
    hour=minute=second=0;
}