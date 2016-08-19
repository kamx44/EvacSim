#ifndef BOUNDARY_H
#define BOUNDARY_H
#include "Object.h"
#include "World.h"

class Boundary : public Object
{
    public:
        Boundary();
        virtual ~Boundary();
        void draw();
        void update(float dt);
    protected:
    private:
};

#endif // BOUNDARY_H
