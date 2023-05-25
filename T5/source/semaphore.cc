#include "cpu.h"
#include "debug.h"
#include "traits.h"

#include "semaphore.h"

__BEGIN_API

using namespace std;

void Semaphore::p()
{
    // Este método deve implementar a operacao p (ou sleep) de um semaforo. Deve-se decrementar o
    // inteiro do semaforo de forma atomica (utilizando fdec descrita abaixo) e colocar a Thread para
    // dormir caso a mesma nao conseguir acessar o semaforo (ja existir em uso por outra Thread).
}

void Semaphore::v()
{
    // Este metodo deve implementar a operacao v (ou wakeup) de um semaforo. Deve-se
    // incrementar o inteiro do semaforo de forma atomica (utilizando finc descrita abaixo) e acordar
    // uma Thread que estiver dormindo no semaforo.
}

void Semaphore::sleep()
{
    // O metodo sleep() deve colocar a Thread que nao conseguir acessar o semaforo para dormir e
    // mudar seu estado para WAITING (note que WAITING eh diferente de SUSPENDED do trabalho anterior).
}

void Semaphore::wakeup()
{
    // O metodo wakeup() deve acordar uma Thread que estava dormindo no semaforo.
}

void Semaphore::wakeup_all()
{
    // O metodo wakeup_all() deve acordar todas as Thread que estavam dormindo no semaforo.
}

Semaphore::~Semaphore()
{
    // IMPLEMENTAR
}

__END_API