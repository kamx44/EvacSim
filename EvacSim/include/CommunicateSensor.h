#ifndef COMMUNICATESENSOR_H
#define COMMUNICATESENSOR_H
#include "Sensor.h"
#include <mutex>
#include <unordered_map>

class Sensor;

class CommunicateSensor : public Sensor
{
    public:
        //sensorType type;
        CommunicateSensor(sensorType type,
               OBJECT_TYPE sensor_type,
               glm::vec2 position,
               float radius);
        virtual ~CommunicateSensor();
        virtual void update(float dt);
        virtual void draw();
        virtual void delObject();
        void addObjectId(unsigned int visibleObjectId);
        void addObjectToCheckAgentContainer(unsigned int visibleObjectId, b2Vec2 position);
        bool removeObjectFromCheckAgentContainer(unsigned int visibleObjectIdAgent, unsigned int visibleObjectIdSensor);
        void removeObjectId(unsigned int visibleObjectId);
        void createObstacleSensorsAgents();
        void setVisibleObjectsIdContainer(std::vector<unsigned int>& _visibleObjects);
    protected:
    private:
        //int32 size;
        //b2CircleShape circle;
        //float radius;
        std::vector<unsigned int>* visibleObjectsIdContainer;
        std::unordered_map<unsigned int, std::pair<bool,b2Vec2> > checkAgentContainer; // add freeSensor bool
        std::unordered_map<unsigned int, Sensor*> obstacleSensorContainer;
        //float radiusMovable;

};

#endif // COMMUNICATESENSOR_H
