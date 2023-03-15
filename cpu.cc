#include "cpu.h"
#include <iostream>

__BEGIN_API

void CPU::Context::save()
{
    //adicionar implementação
    //set_context
}

void CPU::Context::load()
{
    //adicionar implementação
    //get_context
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
