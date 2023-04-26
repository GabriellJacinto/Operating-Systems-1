#include <iostream>
#include <ucontext.h>
#include <queue>
#include <chrono>
#include <ctime>

#include "thread.h"


__BEGIN_API

using namespace std;

int Thread::_available_id = -1; // Se o valor for -1, significa que ainda não foi criada nenhuma thread.
                                // Primeira thread criada terá id 0.
Thread * Thread::_running;
Thread * Thread::_main;
queue<int> Thread::_released_ids;
int Thread::_numOfThreads = 0;



int Thread::id()
{
    return this->_id;
}

int Thread::rankThreadOnCurrentTime(Thread* newThread)
{
    int now = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    newThread->_link.rank(now);
}

int Thread::get_available_id()
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

Thread::~Thread()
{
    _ready.remove(&this->_link); // Remove a thread da fila de prontos.

    if (this->_context) // Libera o contexto, caso ele exista.
    {
        delete this->_context;
    }
}

void Thread::thread_exit(int exit_code)
{
    db<Thread>(INF) << "\nTHREAD " << this->_id << " DELETADA.\n";
    this->_state = FINISHING; // Seta o estado da thread como finalizando.
    Thread::_numOfThreads--; // Decrementa o número de threads criadas.
    Thread::_released_ids.push(this->_id); // Coloca o id da thread que está sendo encerrada na fila de ids liberados.
    this->yield(); // Libera o processador para outra thread(DISPACHER).
}

__END_API
