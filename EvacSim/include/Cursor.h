#ifndef CURSOR_H
#define CURSOR_H

#include "Module.h"
#include "glm/common.hpp"
#include "PolygonGenerator.h"


#include "Image.h"
#include "Object.h"




class World;


class Cursor : public Object
{
public:
    Cursor();

    virtual ~Cursor();
    void calcSpeed();
    void draw();
    void update(float dt);

     virtual void delObject()
    {
        //world_actor->delFromObjectsContainer(this);
    }


protected:
private:
    int32 size;
    b2CircleShape circle;
    float radius;




};

#endif // CURSOR_H