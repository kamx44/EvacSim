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
#include "MainDirection.h"
#include <mutex>
#include <set>

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

    void communication(std::vector<std::pair<unsigned int,b2Vec2> >&,std::vector<unsigned int>&);
    void setPassedExit(unsigned int);
    void Start(){
        // This will start the thread. Notice move semantics!
        the_thread = std::thread(&Actor::communication,this,std::ref(inSightObjectsIds),std::ref(actorsToCommunicateIds));
    }
    b2Vec2 moveForce;
    std::pair<unsigned int,b2Vec2> takeTheMostCommonExit(std::unordered_map<unsigned int, std::pair<int,b2Vec2> > &);
    void addToCountsExitContainer(std::unordered_map<unsigned int, std::pair<int,b2Vec2> > &countsExitsContainer,int idExit, b2Vec2 positionExit);
    void resetToBeggining();

protected:
private:
    std::vector<std::pair<unsigned int,b2Vec2> > inSightObjectsIds;
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
    std::set<unsigned int> passedExits;
    MainDirection mainDirection;
    b2Vec2 previousPosition;
    int constPositionCounter;
};

#endif // ACTOR_H
