#ifndef ACTOR_H
#define ACTOR_H
#include <string>
#include <vector>
#include <thread>
#include "Sensor.h"
#include "CommunicationBridge.h"
#include "Sector.h"
#include "Object.h"
#include "World.h"
#include "Cursor.h"
#include <mutex>

class CommunicationBridge;


class Actor : public Object
{
public:
    Actor(Sector* sector,ObjectsContainer* objContainer,CommunicationBridge* comBridge, Cursor* _cursor) ;

    virtual ~Actor();
    void run();
    void writeToBuffer(std::string message);
    void update(float dt);
    void calcSpeed();
    void draw();
    void createJoint(b2Body* bodyB);
    virtual void delObject()
    {
        //world_actor->delFromObjectsContainer(this);
    }

    void communication(std::vector<unsigned int>&,std::vector<unsigned int>&);

    void Start(){
        // This will start the thread. Notice move semantics!
        the_thread = std::thread(&Actor::communication,this,std::ref(inSightObjectsIds),std::ref(actorsToCommunicateIds));
    }
    b2Vec2 moveForce;
protected:
private:
    std::vector<unsigned int> inSightObjectsIds;
    std::vector<unsigned int> actorsToCommunicateIds;
    std::string readBuffer;
    int32 size;
    b2CircleShape circle;
    float radius;
    Sensor* collisionSensor;
    Sensor* communicateSensor;
    Sensor* sightSensor;
    Sensor* moveSensor;
    b2WeldJointDef jointDef;
    b2WeldJoint* jointWeld;
    CommunicationBridge* communicationBridge;
    std::thread the_thread;
    bool stop_thread = false;
    Cursor* cursor;
    bool rotateBody;


};

#endif // ACTOR_H
