#ifndef ASTEROIDE_H
#define ASTEROIDE_H

#include "Object.h"
#include "World.h"

//class World;

class Asteroide : public Object
{
public:
    Asteroide(World* world);

    virtual ~Asteroide();
    void update(float dt);
    World* world_asteroide;
    int32 size;
    b2CircleShape circle;
    float radius;
    void draw();
    virtual void delObject()
    {
        world_asteroide->delFromObjectsContainer(this);
    }
protected:
private:
};

#endif // ASTEROIDE_H
