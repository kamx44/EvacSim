#ifndef SIGHTSENSOR_H
#define SIGHTSENSOR_H
#include "Sensor.h"
#include "MainDirection.h"
#include <unordered_map>


class SightSensor : public Sensor
{
    public:
        //sensorType type;
        SightSensor(sensorType type,
               OBJECT_TYPE sensor_type,
               glm::vec2 position,
               float radius);
        virtual ~SightSensor();
        virtual void update(float dt);
        virtual void draw();
        virtual void delObject();
        void addObjectIdPos(unsigned int visibleObjectId, b2Vec2 position);
        void addObjectToCheckExitContainer(unsigned int visibleObjectId, b2Vec2 position);
        bool removeObjectFromCheckExitContainer(unsigned int visibleObjectIdExit, unsigned int visibleObjectIdSensor);
        void removeObjectIdPos(unsigned int visibleObjectId);
        void createObstacleSensorsExit();
        void setVisibleObjectsIdPosContainer(std::vector<std::pair<unsigned int,b2Vec2> >& _visibleObjects);
        void setParentMainDirection(MainDirection &mainDir);
        void setMainExitDirection(unsigned int mainExitId,b2Vec2 position);
    protected:
    private:
        //int32 size;
        b2CircleShape circle;
        //float radius;
        std::vector<std::pair<unsigned int,b2Vec2> >* visibleObjectsIdPosContainer;
        std::unordered_map<unsigned int, std::pair<bool,b2Vec2> > checkExitContainer;
        std::unordered_map<unsigned int, Sensor*> obstacleSensorContainer;
        //float radiusMovable;
        MainDirection* mainDirection;
};

#endif // SIGHTSENSOR_H
