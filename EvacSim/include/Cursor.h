#ifndef CURSOR_H
#define CURSOR_H

#include "Module.h"
#include "glm/common.hpp"
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
    virtual void delObject(){}
    b2Vec2 getPosition();


protected:
private:
    //int32 size;
    b2CircleShape circle;
    float radius;




};

#endif // CURSOR_H
