#include "cpu.h"
#include <iostream>

__BEGIN_API

void CPU::Context::save()
{
    ucontext_t * contextToSavePtr = &this->_context;
    getcontext(contextToSavePtr);
}

void CPU::Context::load()
{
    ucontext_t * contextToLoadPtr = &this->_context;
    setcontext(contextToLoadPtr);

}

CPU::Context::~Context()
{
    //cout << "Thread " << PID << "destruída"<<endl;
}

int CPU::switch_context(Context *from, Context *to)
{
    //troca o contexto usando os dois
    //contextos recebidos como parâmetro (de “from” para “to”) e retornar 0 caso seja bem sucedido
    //ou negativo caso aconteça um erro.

    
}

__END_API
