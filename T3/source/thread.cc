#include <iostream>
#include <ucontext.h>
#include "thread.h"
#include <queue>

__BEGIN_API

int Thread::_available_id = -1; // Se o valor for -1, significa que ainda não foi criada nenhuma thread.
                                // Primeira thread criada terá id 0.

Thread * Thread::_running;

Thread * Thread::_main;

queue<int> Thread::_released_ids;

int Thread::numOfThreads = 0;

int Thread::id()
{
    return this->_id;
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

void Thread::yield()
{
    // Imprima informação usando o debug em nível TRC;
    db<Thread>(TRC) << "Thread::yield(running = " << running() << ")";

    // Escolha uma próxima thread a ser executada;
    Thread * next = Thread::_ready.remove()->object();
    Thread * prev = Thread::_running;

    // Atualiza a prioridade da tarefa que estava sendo executada (aquela que chamou yield) com o
    // timestamp atual, a fim de reinserí-la na fila de prontos atualizada (cuide de casos especiais, como
    // estado ser FINISHING ou Thread main que não devem ter suas prioridades alteradas);
    if (_running != &_main && _running->_state != FINISHING)
    {
        // Reinsira a thread que estava executando na fila de prontos;
        prev->_state = READY;
    }

    // Atualiza o ponteiro _running;
    Thread::_running = next;

    // Atualiza o estado da próxima thread a ser executada;
    next->_state = RUNNING;

    // Troque o contexto entre as threads;
    db<Thread>(TRC) << "\nTHREAD " << prev->_id << " SWITCH TO THREAD " << next->_id << ".\n";
    switch_context(prev, next);
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
    db<Thread>(INF) << "\nTHREAD " << this->_id << " DELETADA.\n";
    Thread::_numOfThreads--;
    Thread::_released_ids.push(this->_id); // Coloca o id da thread que está sendo encerrada na fila de ids liberados.
    if (this->_context)
    {
        delete this->_context;
    }
}

__END_API
