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
class Lists;
class Semaphore;
class KeyboardHandler;
class Window;
class fpsController;
class BrickShooter;

// Declaracao da classe Traits
template<typename T> struct Traits {
    static const bool debugged = false;
};

template<> struct Traits<CPU> {
    static const int STACK_SIZE = 256*1024;
    static const bool debugged = false;
};

template <> struct Traits<Debug> : public Traits<void> {
    // Para habilitar um tipo de debug, basta setar sua variavel para true: (ATUALMENTE ESTA O DEFAULT DO PDF)
    static const bool error = false;
    static const bool warning = false;
    static const bool info = false;
    static const bool trace = false;
};

template <> struct Traits<Thread> : public Traits<void> {
    static const bool debugged = false;
};

template <> struct Traits<System> : public Traits<void> {
    static const bool debugged = false;
};

template <> struct Traits<Main> : public Traits<void> {
    static const bool debugged = false;
};

template <> struct Traits<Lists> : public Traits<void> {
    static const bool debugged = false;
};

template <> struct Traits<Semaphore> : public Traits<void> {
    static const bool debugged = false;
};

template <> struct Traits<KeyboardHandler> : public Traits<void> {
    static const bool debugged = false;
};

template <> struct Traits<Window> : public Traits<void> {
    static const bool debugged = false;
};

template <> struct Traits<fpsController> : public Traits<void> {
    static const bool debugged = false;
};

template <> struct Traits<BrickShooter> : public Traits<void> {
    static const bool debugged = false;
};

__END_API

#endif
