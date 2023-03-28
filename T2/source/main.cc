#include "../headers/thread.h"
#include "../headers/system.h"
#include "../headers/main_class.h"

__USING_API 

int main(void)
{
    System::init();
    Main *m = new Main();
    delete m;
}