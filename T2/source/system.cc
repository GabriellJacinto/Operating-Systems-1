#include <iostream>
#include <ucontext.h>
#include <stdio.h>
#include "system.h"

__BEGIN_API

void System::init(){
    setvbuf(stdout, 0, _IONBF, 0); //setvbuf(FILE *stream, char *buf, int type, size_t size);
}

__END_API