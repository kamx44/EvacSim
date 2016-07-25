#ifndef BOUNDARY_H
#define BOUNDARY_H
#include "Object.h"
#include "World.h"

class Boundary : public Object
{
    public:
        Boundary(World* world);
        virtual ~Boundary();
        World* world_boundary;
        void draw();
        void update(float dt);
    protected:
    private:
};

#endif // BOUNDARY_H
