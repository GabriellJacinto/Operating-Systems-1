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
int Thread::_numOfThreads = 0;
queue<int> Thread::_released_ids;
Thread *Thread::_running;
Thread Thread::_main;
CPU::Context Thread::_main_context;
Thread Thread::_dispatcher;
Thread::Ready_Queue Thread::_ready;

void Thread::init(void (*main)(void *))
{
    // Cria a thread main, passando main() e a string "Main" como parâmetros.
    // A string é argumento da função main().
    create_main_thread(main);

    // Cria a thread dispatcher, que será responsável por escolher a próxima thread a ser executada.
    create_dispatcher_thread();

    // Pega o contexto da main() do main.cc e salva em _main_context.
    new (&_main_context) CPU::Context();

    // Troca o contexto da main() do main.cc para o contexto da Thread::_main criada aqui.
    CPU::switch_context(&_main_context, _main.context());
}

void Thread::create_main_thread(void (*main)(void *))
{
    new (&_main) Thread(main, (void *)"Main");
    _running = &_main;
    _main._state = RUNNING;
}

void Thread::create_dispatcher_thread()
{
    new (&_dispatcher) Thread(&dispatcher);
}

int Thread::id()
{
    return this->_id;
}

int Thread::get_now_timestamp()
{
    return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
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

void Thread::dispatcher()
{
    while (!_ready.empty())
    {
        // Escolhe a próxima thread a ser executada.
        // E já a prepara, setando seu estado e o ponteiro _running.
        Thread* nextThreadToRun = get_thread_to_dispatch_ready();

        // Torna o dispatcher a thread que está executando no momento.
        prepare_dispatcher_to_run_again();

        db<Thread>(TRC) << "THREAD" << nextThreadToRun->_id << "EM EXECUÇÂo" << "\n";
        db<Thread>(TRC) << "READY QUEUE: " << _ready.size() << "\n";

        // Troca o contexto para a próxima thread a ser executada.
        // Ou seja, a partir daqui, a próxima thread a ser executada é a que acabou de ser escolhida.
        // Assim, a próxima linha após a troca de contexto só será executada quando a thread escolhida terminar sua execução
        // e o despachante voltar a ser executado.
        Thread::switch_context(&_dispatcher, nextThreadToRun);

        // Ao voltar ao despachante, verifica se a próxima thread a ser executada (que está no começo da fila)
        // terminou sua execução. Se sim, a removerá da fila de prontos. 
        check_if_next_thread_is_finished();
    }

    // Caso a fila esteja vazia, acabaram a execução todas as threads.
    // Assim, o despachante retorna para a thread main.
    return_to_main();
}

Thread* Thread::get_thread_to_dispatch_ready()
{
    {
        db<Thread>(TRC) << "ESCOLHENDO THREAD A SER DESPACHADA.\n";
        Thread* next = _ready.remove_head()->object();
        next->_state = RUNNING;
        _running = next;
        
        return next;
    }
}

void Thread::prepare_dispatcher_to_run_again()
{
    db<Thread>(TRC) << "PREPARANDO DESPACHANTE PARA EXECUTAR NOVAMENTE.\n";
    _dispatcher._state = READY;
    _ready.insert(&Thread::_dispatcher._link);
}

void Thread::check_if_next_thread_is_finished()
{
    db<Thread>(TRC) << "VERIFICANDO SE A PRÓXIMA THREAD A SER EXECUTADA TERMINOU SUA EXECUÇÃO.\n";
    if (!_ready.empty())
    {
        Thread* next = _ready.head()->object();
        if(next->_state == FINISHING)
        {
            _ready.remove_head();
        }
    }
}

void Thread::return_to_main()
{
    db<Thread>(TRC) << "THREAD MAIN EM EXECUÇÃO" << "\n";
    cout << "THREAD MAIN EM EXECUÇÃO" << "\n";
    _dispatcher._state = FINISHING;
    switch_context(&_dispatcher, &_main);
}

void Thread::yield()
{
    // Imprima informação usando o debug em nível TRC;
    db<Thread>(TRC) << "Thread::yield(running = " << ")"; // Imprime a thread que está executando.

    // Escolha uma próxima thread a ser executada;
    // Como o Dispacher não chama o yield, ele não é rankeado novamente, então sua prioridade sempre será a maior.
    // Logo, next aponta ao Dispacher. E este, portanto, dispara a próxima thread a ser executada.
    Thread * next = _ready.remove()->object();
    Thread * prev = _running;

    // Atualiza a prioridade da tarefa que estava sendo executada (aquela que chamou yield) com o
    // timestamp atual, a fim de reinserí-la na fila de prontos atualizada (cuide de casos especiais, como
    // estado ser FINISHING ou Thread main que não devem ter suas prioridades alteradas);
    if (_running != &_main && _running->_state != FINISHING)
    {   
        _running->_link.rank(get_now_timestamp()); // Atualiza a prioridade da thread que estava executando.
        db<Thread>(TRC) << "\nTHREAD " << _running->_id << " UPDATED WITH TIMESTAMP = " << _running->_link.rank() << ".\n";

        // Reinsira a thread que estava executando na fila de prontos;
        _running->_state = READY;
        _ready.insert(&_running->_link);
        db<Thread>(TRC) << "\nTHREAD " << _running->_id << " REINSERTED IN LIST.\n";
    }

    // Atualiza o ponteiro _running;
    _running = next;

    // Atualiza o estado da próxima thread a ser executada;
    next->_state = RUNNING;

    // Troque o contexto entre as threads;
    db<Thread>(TRC) << "\nTHREAD " << prev->_id << " SWITCH TO THREAD " << next->_id << ".\n";
    switch_context(prev, next);
}

int Thread::switch_context(Thread * prev, Thread * next) 
{
    db<Thread>(TRC) << "Thread::switch_context("<<")";
    Context * prevThreadContext = prev->context();
    Context * nextThreadContext = next->context();

    int swapWorked = CPU::switch_context(prevThreadContext, nextThreadContext); // Troca o contexto para a thread em execução, que é a próxima.

    return swapWorked; // Retorna se a troca de contexto foi bem sucedida.
}

void Thread::insert_thread_link_on_ready_queue(Thread* thread)
{
    if (thread->_id != _main._id)
    {
        _ready.insert(&thread->_link);
    }
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
    db<Thread>(INF) << "THREAD " << this->_id << " DELETADA.\n";
    _numOfThreads--; // Decrementa o número de threads criadas.
    _released_ids.push(this->_id); // Coloca o id da thread que está sendo encerrada na fila de ids liberados.
    this->_state = FINISHING; // Seta o estado da thread como finalizando.
    yield(); // Libera o processador para outra thread(DISPACHER).
}

__END_API
