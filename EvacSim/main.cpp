#include "Game.h"
#include "ctime"
#include <iostream>
#include <pthread.h>
#include <thread>



// Remove SDL main hook on Windows
#ifdef _WIN32
#  undef main
#endif

using namespace std;

void termin()
{
    cout<<"Blad z dupy"<<endl;
    //  exit(0);
}

void (*old_terminate)() = set_terminate(termin);

void foo()
{
  cout<<"lamy lamy lamy"<<endl;
}

void testThread(int dupa){
    cout<<"DUPA : "<<dupa<<endl;
}

int main()
{

    try
    {
        //thread first(foo);
       // first.join();

        srand (static_cast <unsigned> (time(0)));
        Game gra;
        gra.gameOn();
    }
    catch(...)
    {
        cout<<"Jakis blad"<<endl;
    }
    return 0;
}



