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
    if (this->_stack != 0) // Se o valor apontado por _stack for diferente de 0, esse valor não será destruído no destructor padrão. 
                           // Embora o ponteiro seja destruído, o valor apontado não é. 
    {
        delete this->_stack;
    }
}

int CPU::switch_context(Context *from, Context *to)
{
    ucontext_t * currentContextPtr = &from->_context;
    ucontext_t * nextContextPtr = &to->_context;
    int swapWorked = swapcontext(currentContextPtr, nextContextPtr);
}

__END_API
