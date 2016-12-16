#include "Game.h"
#include "ctime"
#include <iostream>
#include <pthread.h>
#include <thread>
#include <chrono>



// Remove SDL main hook on Windows
#ifdef _WIN32
#  undef main
#endif

using namespace std;

void termin()
{
    cout<<"Blad"<<endl;
    //  exit(0);
}

void (*old_terminate)() = set_terminate(termin);


int main()
{
    try
    {


// code to benchmark


        srand (static_cast <unsigned> (time(0)));
        Game gra;
        auto beginTime = std::chrono::high_resolution_clock::now();
        gra.gameOn();
        auto endTime = std::chrono::high_resolution_clock::now();
        double time = (double)(std::chrono::duration_cast<std::chrono::milliseconds>(endTime-beginTime).count()) / 1000;
        std::cout <<std::endl<<"Czas symulacji: "<< time << "s" << std::endl;
    }
    catch(...)
    {
        cout<<"Jakis blad"<<endl;
    }
    return 0;
}



