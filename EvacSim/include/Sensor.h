#ifndef SENSOR_H
#define SENSOR_H
#include "Object.h"

enum sensorType{
    COMMUNICATE,
    SIGHT
};

class Sensor : public Object
{
    public:
        sensorType type;
        Sensor(glm::vec2 position,float radius, sensorType type);
        virtual ~Sensor();
        void update(float dt);
        void draw();
        virtual void delObject()
        {
            //world_actor->delFromObjectsContainer(this);
        }
        b2Body* getBody();
    protected:
    private:
        int32 size;
        b2CircleShape circle;
        float radius;
};

#endif // SENSOR_H
