#ifndef POLYGONY_H
#define POLYGONY_H


#include <math.h>


class Polygony
{
public:
    Polygony();
    ~Polygony();
    void draw();
protected:
private:
    float s = 1;
    float h;
    float r;
    float b;
    float a;
    float x;
    float y;
};

#endif // POLYGONY_H
