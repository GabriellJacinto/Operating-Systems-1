#ifndef thread_h
#define thread_h

#include "cpu.h"
#include "traits.h"
#include "debug.h"
#include <queue>
#include "list.h"
#include <ctime> 
#include <chrono>

using namespace std;

__BEGIN_API

class Thread
{
protected:
    typedef CPU::Context Context;

public:
    typedef Ordered_List<Thread> Ready_Queue;

    // Thread State
    enum State {
        RUNNING,
        READY,
        FINISHING
    };

    /*
     * Construtor vazio. Necessário para inicialização, mas sem importância para a execução das Threads.
     */ 
    Thread() { }

    /*
     * Cria uma Thread passando um ponteiro para a função a ser executada
     * e os parâmetros passados para a função, que podem variar.
     * Cria o contexto da Thread.
     * PS: devido ao template, este método deve ser implementado neste mesmo arquivo .h
     */ 
    template<typename ... Tn>
    Thread(void (* entry)(Tn ...), Tn ... an);

    /*
     * Retorna a Thread que está em execução.
     */ 
    static Thread * running() { return _running; }

    /*
     * Método para trocar o contexto entre duas thread, a anterior (prev)
     * e a próxima (next).
     * Deve encapsular a chamada para a troca de contexto realizada pela class CPU.
     * Valor de retorno é negativo se houve erro, ou zero.
     */ 
    static int switch_context(Thread * prev, Thread * next);

    /*
     * Termina a thread.
     * exit_code é o código de término devolvido pela tarefa (ignorar agora, vai ser usado mais tarde).
     * Quando a thread encerra, o controle deve retornar à main. 
     */  
    void thread_exit (int exit_code);

    /*
     * Retorna o ID da thread.
     */ 
    int id();

    /*
     * NOVO MÉTODO DESTE TRABALHO.
     * Daspachante (disptacher) de threads. 
     * Executa enquanto houverem threads do usuário.
     * Chama o escalonador para definir a próxima tarefa a ser executada.
     */
    static void dispatcher(); 

    /*
     * NOVO MÉTODO DESTE TRABALHO.
     * Realiza a inicialização da class Thread.
     * Cria as Threads main e dispatcher.
     */ 
    static void init(void (*main)(void *));

    /*
     * Devolve o processador para a thread dispatcher que irá escolher outra thread pronta
     * para ser executada.
     */
    static void yield(); 

    /*
     * Destrutor de uma thread. Realiza todo os procedimentos para manter a consistência da classe.
     */ 
    ~Thread();

    /*
     * Qualquer outro método que você achar necessário para a solução.
     */ 
    Context * context() {return _context;} // retorna o contexto da thread, que é um atributo privado.

    static void rank_thread_on_current_time(Thread* new_thread); // Enfileira a thread na fila de threads prontas, ordenando-as pelo tempo de criação.

    static int get_available_id(); // retorna o id disponível para a próxima thread a ser criada.

    static Thread* get_thread_to_dispatch_ready(); // retorna a próxima thread a ser executada.

    static void prepare_dispatcher_to_run_again(); // prepara o dispatcher para ser executado novamente.

    static void check_if_next_thread_is_finished(); // verifica se a próxima thread a ser executada já terminou.

    static void return_to_main(); // retorna para a thread main.

    static void insert_thread_link_on_ready_queue(Thread* thread); // enfileira a thread na fila de threads prontas.

    static void create_main_thread(void (*main)(void *)); // Cria a thread main.

    static void create_dispatcher_thread(); // Cria a thread dispatcher.

    static int get_now_timestamp(); // retorna o tempo atual.

private:
    int _id;
    Context * volatile _context;
    static Thread * _running;
    static Thread _main; // thread principal. Não 
    static CPU::Context _main_context;
    static Thread _dispatcher;
    static Ready_Queue _ready;
    Ready_Queue::Element _link;
    volatile State _state; // Como o estado da thread pode ser alterado por outra thread, é necessário que ele seja volátil.
                           // Volatile garante que o compilador não otimize o código para esse estado.

    /*
     * Qualquer outro atributo que você achar necessário para a solução.
     */ 
    static int _available_id; // id disponível para a próxima thread a ser criada. Unsigned int porque é sempre positivo.
    static int _numOfThreads; // número de threads criadas.
    static queue<int> _released_ids; // fila de ids que foram liberados, mas ainda não foram reutilizados.
};

template <typename ... Tn> 
inline Thread::Thread(void (*entry)(Tn...), Tn... an) : _link(this, Thread::get_now_timestamp())
{
    this->_id = get_available_id();

    Thread::_numOfThreads++;

    this->_context = new Context(entry, an...);

    insert_thread_link_on_ready_queue(this);

    db<Thread>(TRC) << "THREAD " << this->_id << " CRIADA.\n";
    db<Thread>(TRC) << Thread::_numOfThreads << " THREADS EXISTENTES.\n";
    db<Thread>(TRC) << "THREADS PRONTAS: " << _ready.size() << "\n";
}

__END_API

#endif