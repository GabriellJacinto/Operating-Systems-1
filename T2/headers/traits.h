#ifndef traits_h
#define traits_h

//Não alterar as 3 declarações abaixo

#define __BEGIN_API             namespace SOLUTION {
#define __END_API               }
#define __USING_API            using namespace SOLUTION;

__BEGIN_API

// Declaracoes das classes criadas nos trabalhos devem ser colocadas aqui:
class CPU;
class Debug;
class Thread;
class System;
class Main;

// Declaracao da classe Traits
template<typename T> struct Traits {
    static const bool debugged = false;
};

template<> struct Traits<CPU> {
    static const int STACK_SIZE = 16000;
    static const bool debugged = false;
};

template <> struct Traits<Debug> : public Traits<void> {
    // Para habilitar um tipo de debug, basta setar sua variavel para true: (ATUALMENTE ESTA O DEFAULT DO PDF)
    static const bool error = false;
    static const bool warning = false;
    static const bool info = true;
    static const bool trace = false;
};

template <> struct Traits<Thread> : public Traits<void> {
    static const bool debugged = true;
};

template <> struct Traits<System> : public Traits<void> {
    static const bool debugged = true;
};

template <> struct Traits<Main> : public Traits<void> {
    static const bool debugged = true;
};

__END_API

#endif
