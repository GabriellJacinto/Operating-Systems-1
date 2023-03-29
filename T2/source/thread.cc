#include <iostream>
#include <ucontext.h>
#include "../headers/thread.h"

__BEGIN_API

int Thread::id() 
{
    return this->_id;
}

int Thread::switch_context(Thread * prev, Thread * next) 
{
    Thread::_running = next; // Seta a thread em execução como a próxima thread.

    Context * prevThreadContext = prev->context();
    Context * nextThreadContext = next->context();

    int swapWorked = CPU::switch_context(prevThreadContext, nextThreadContext); // Troca o contexto para a thread em execução, que é a próxima.

    return swapWorked; // Retorna se a troca de contexto foi bem sucedida.
}

void Thread::thread_exit(int exit_code)
{
    if (this->_context)
    {
        delete this->_context;
    }
}

__END_API
