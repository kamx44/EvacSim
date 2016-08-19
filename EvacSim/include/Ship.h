#ifndef SHIP_H
#define SHIP_H

#include "Module.h"
#include "glm/common.hpp"
#include "PolygonGenerator.h"
#include "Missle.h"

#include "Image.h"
#include "Object.h"




class World;


class Ship : public Object
{
public:
    Ship();
    Ship(World* world);
    virtual ~Ship();
    void strzel_rakieta(KIND type);
    void calcSpeed();
    void draw();
    void update(float dt);
    b2World *world;
    World* world_ship;
    Image *ship_img;
    Image *mis_img;
    unsigned int myID;
    unsigned int misID;
    float w;
    float h;
    int32 size;
    Module* module;
    std::vector<Module*> modules;


    b2WeldJointDef jointDef;
    b2WeldJoint* jointWeld;
    virtual void delObject()
    {
       // world_ship->delFromObjectsContainer(this);
    }

    void createJoint(b2Body* bodyB);










protected:
private:




};

#endif // SHIP_H
