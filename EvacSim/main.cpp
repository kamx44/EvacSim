#include "Game.h"
#include "ctime"
#include <iostream>


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

int main()
{

    try
    {
        //srand (static_cast <unsigned> (time(0)));
        Game gra;
        gra.gameOn();
    }
    catch(...)
    {
        cout<<"Jakis blad"<<endl;
    }
    return 0;
}



