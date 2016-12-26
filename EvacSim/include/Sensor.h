#ifndef SENSOR_H
#define SENSOR_H
#include "Object.h"
#include "MainDirection.h"
#include <unordered_map>

enum sensorType{
    COMMUNICATE_SENSOR,
    SIGHT_SENSOR,
    MOVE_SENSOR,
    COLLISION_SENSOR,
    OBSTACLE_EXIT,
    OBSTACLE_ACTOR
};

class Sensor : public Object
{
    public:
        sensorType type;
        Sensor(sensorType type,
               OBJECT_TYPE sensor_type,
               glm::vec2 position,
               float radius);
        virtual ~Sensor();
        virtual void update(float dt){}
        virtual void draw(){}
        virtual void delObject(){}
        b2Body* getBody();
        void setDrawable(bool draw);
        void setParentSensor(Object* pSensor);
        Object* getParentSensor();


        virtual void addObjectId(unsigned int visibleObjectId){}
        virtual void addObjectIdPos(unsigned int visibleObjectId, b2Vec2 position){}
        virtual void addObjectToCheckExitContainer(unsigned int visibleObjectId, b2Vec2 position){}
        virtual void addObjectToCheckAgentContainer(unsigned int visibleObjectId, b2Vec2 position){}
        virtual bool removeObjectFromCheckExitContainer(unsigned int visibleObjectIdExit, unsigned int visibleObjectIdSensor){return false;}
        virtual bool removeObjectFromCheckAgentContainer(unsigned int visibleObjectIdAgent, unsigned int visibleObjectIdSensor){return false;}
        virtual void removeObjectId(unsigned int visibleObjectId){}
        virtual void removeObjectIdPos(unsigned int visibleObjectId){}
        virtual void sendObstacleSensor(b2Vec2 destination){}
        virtual void createObstacleSensorsExit(){}
        virtual void createObstacleSensorsAgents(){}
        virtual void moveBody(b2Vec2 velocity){}
        virtual void setParentVelocity(b2Vec2 velocity){}
        virtual void setVisibleObjectsIdPosContainer(std::vector<std::pair<unsigned int,b2Vec2> >& _visibleObjects){}
        virtual void setVisibleObjectsIdContainer(std::vector<unsigned int>& _visibleObjects){}
        virtual void setParentForce(b2Vec2* parentForce){}
        virtual unsigned int getWhereIWasSend(){return 0;}
        virtual void setWhereIWasSend(unsigned int){}
        virtual void setParentMainDirection(MainDirection *mainDir){}
        virtual void setMainExitDirection(unsigned int mainExitId,b2Vec2 position){}


    protected:
        float radius;
        float radiusMovable;
        Object* parentSensor;
    private:


};

#endif // SENSOR_H
