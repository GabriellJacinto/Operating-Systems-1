#include <iostream>
#include <ucontext.h>
#include "../headers/thread.h"
#include <queue>

__BEGIN_API

unsigned int Thread::_available_id = 0;

Thread * Thread::_running;

Thread * Thread::_main;

queue<unsigned int> Thread::_released_ids;

int Thread::id()
{
    return this->_id;
}

unsigned int Thread::get_available_id()
{
    if (Thread::_released_ids.empty())
        Thread::_available_id++;
    else
    {
        Thread::_available_id = Thread::_released_ids.front();
        Thread::_released_ids.pop();
    }

    return Thread::_available_id;
} // retorna o id da thread, que é um atributo privado.

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
    Thread::_released_ids.push(this->_id); // Coloca o id da thread que está sendo encerrada na fila de ids liberados.
    if (this->_context)
    {
        delete this->_context;
    }
}

__END_API
