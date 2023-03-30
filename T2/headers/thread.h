#ifndef thread_h
#define thread_h

#include "../headers/cpu.h"
#include "../headers/traits.h"
#include "../headers/debug.h"

__BEGIN_API

class Thread
{
protected:
    typedef CPU::Context Context;

public:
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
     * Qualquer outro método que você achar necessário para a solução.
     */ 
    Context * context() {return _context;} // retorna o contexto da thread, que é um atributo privado.

    

    static unsigned int get_available_id()
    {
        if (!Thread::available_id)
            Thread::available_id = 1;
        else
            Thread::available_id++;
        return Thread::available_id++;
    } // retorna o id da thread, que é um atributo privado.

private:
    int _id;
    Context * volatile _context;
    static Thread * _running;

    /*
     * Qualquer outro atributo que você achar necessário para a solução.
     */ 
    static Thread * _main; // thread principal do programa.
    static unsigned int available_id; // id disponível para a próxima thread a ser criada. Unsigned int porque é sempre positivo.
};

template <typename ... Tn> 
Thread::Thread(void (* entry)(Tn ...), Tn ... an) 
{
    this->_id = get_available_id();
    this->_context = new Context(entry, an...);

    if (!Thread::_main)
        Thread::_main = this; // se a thread principal não foi criada, então a thread que está sendo criada é a principal.
}

__END_API

#endif
