#ifndef SENSOR_H
#define SENSOR_H
#include "Object.h"
#include "ObjectsContainer.h"
#include <unordered_map>

enum sensorType{
    COMMUNICATE,
    SIGHT
};

class Sensor : public Object
{
    public:
        sensorType type;
        Sensor(OBJECT_TYPE sensor_type,glm::vec2 position,float radius,unsigned int idParent, ObjectsContainer* objContainer);
        virtual ~Sensor();
        void update(float dt);
        void draw();
        virtual void delObject()
        {
            //world_actor->delFromObjectsContainer(this);
        }
        b2Body* getBody();
        void addObjectId(unsigned int visibleObjectId);
        void addObjectIdPos(unsigned int visibleObjectId, b2Vec2 position);
        void addObjectToCheckExitContainer(unsigned int visibleObjectId, b2Vec2 position);
        void addObjectToCheckActorContainer(unsigned int visibleObjectId, b2Vec2 position);
        bool removeObjectFromCheckExitContainer(unsigned int visibleObjectIdExit, unsigned int visibleObjectIdSensor);
        void removeObjectFromCheckActorContainer(unsigned int visibleObjectId);
        void moveObjectFromCheckExitContainer(unsigned int visibleObjectId);
        void moveObjectFromCheckActorContainer(unsigned int visibleObjectId);
        void removeObjectId(unsigned int visibleObjectId);
        void removeObjectIdPos(unsigned int visibleObjectId);
        void sendObstacleSensor(b2Vec2 destination);
        void createObstacleSensors();
        void moveBody(b2Vec2 velocity);
        unsigned int getParentId();
        void printData(); //for testing
        void setParentVelocity(b2Vec2 velocity);
        void setDrawable(bool draw);
        void setVisibleObjectsIdPosContainer(std::vector<std::pair<unsigned int,b2Vec2> >& _visibleObjects);
        void setVisibleObjectsIdContainer(std::vector<unsigned int>& _visibleObjects);
        void setParentForce(b2Vec2* parentForce);
        void setParentSensor(Sensor* pSensor);
        Sensor* getParentSensor();
        unsigned int getWhereIWasSend();
        void setWhereIWasSend(unsigned int);


      /* void setVelocity(b2Vec2 vel){
            b
        } */
        bool rotateBody;
    protected:
    private:
        int32 size;
        b2CircleShape circle;
        float radius;
        unsigned int idParent;
        std::vector<unsigned int>* visibleObjectsIdContainer;
        std::vector<std::pair<unsigned int,b2Vec2> >* visibleObjectsIdPosContainer;
        std::unordered_map<unsigned int, std::pair<bool,b2Vec2> > checkExitContainer;
        std::unordered_map<unsigned int, std::pair<bool,b2Vec2> > checkActorContainer; // add freeSensor bool
        std::unordered_map<unsigned int, Sensor*> obstacleSensorContainer;
        float radiusMovable;
        b2Vec2* pushForce;
        bool freeNewSensor;
        ObjectsContainer* objContainer;
        Sensor* parentSensor;
        unsigned int whereIWasSend;

};

#endif // SENSOR_H
