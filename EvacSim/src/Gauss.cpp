#include "Gauss.h"


Gauss::Gauss()
{
    //ctor
}

Gauss::~Gauss()
{
    //dtor
}

double Gauss::normal(const double &mean, const double &std)   // funkcja wyliczajaca skladowa normalna *****************************************
{
    srand(time(0));
    static const double pii = 3.1415927;
    static const double r_max = RAND_MAX + 1;
    return std*sqrt(-2 * log((rand() + 1) / r_max))*sin(2 * pii*rand() / r_max) + mean;
}
