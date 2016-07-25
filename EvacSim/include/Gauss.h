#ifndef GAUSS_H
#define GAUSS_H
#include <ctime>
#include <time.h>
#include <cstdlib>
#include <math.h>

class Gauss
{
public:
    Gauss();
    virtual ~Gauss();
    double normal(const double &mean, const double &std);
protected:
private:
};

#endif // GAUSS_H
