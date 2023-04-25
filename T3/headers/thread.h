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

    static int get_available_id(); // retorna o id disponível para a próxima thread a ser criada.

private:
    int _id;
    Context * volatile _context;
    static Thread * _running;

    static Thread _main; 
    static CPU::Context _main_context;
    static Thread _dispatcher;
    static Ready_Queue _ready;
    Ready_Queue::Element _link;
    volatile State _state;

    /*
     * Qualquer outro atributo que você achar necessário para a solução.
     */ 
    static Thread * _main; // thread principal do programa.
    static int _available_id; // id disponível para a próxima thread a ser criada. Unsigned int porque é sempre positivo.
    static queue<int> _released_ids; // fila de ids que foram liberados, mas ainda não foram reutilizados.
};

template <typename ... Tn> 
inline Thread::Thread(void (* entry)(Tn ...), Tn ... an) 
{
    this->_id = get_available_id();
    this->_context = new Context(entry, an...);

    db<Thread>(INF) << "\nTHREAD " << this->_id << " CRIADA.\n";

    if (!Thread::_main)
        Thread::_main = this; // se a thread principal não foi criada, então a thread que está sendo criada é a principal.
}

__END_API

#endif
