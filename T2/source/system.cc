#include <iostream>
#include <ucontext.h>
#include <stdio.h>
#include "../headers/system.h"
#include "../headers/traits.h"
#include "../headers/debug.h"

__BEGIN_API

void System::init(){

    // db<System>(INF) << "SYSTEM INICIADO.\n";

    setvbuf(stdout, 0, _IONBF, 0); //setvbuf(FILE *stream, char *buf, int type, size_t size);
}

__END_API