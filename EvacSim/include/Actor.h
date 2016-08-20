#ifndef ACTOR_H
#define ACTOR_H
#include "Sensor.h"
#include "Sector.h"
#include "Object.h"
#include "World.h"

//class World;

class Actor : public Object
{
public:
    Actor(Sector* sector,ObjectsContainer* objContainer);

    virtual ~Actor();
    void update(float dt);
    void draw();
    void createJoint(b2Body* bodyB);
    virtual void delObject()
    {
        //world_actor->delFromObjectsContainer(this);
    }
protected:
private:
    int32 size;
    b2CircleShape circle;
    float radius;
    Sensor* communicateSensor;
    Sensor* sightSensor;
    b2WeldJointDef jointDef;
    b2WeldJoint* jointWeld;
};

#endif // ACTOR_H
