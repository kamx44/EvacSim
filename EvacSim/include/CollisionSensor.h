#ifndef COLLISIONSENSOR_H
#define COLLISIONSENSOR_H
#include "Sensor.h"
#include <unordered_map>



class CollisionSensor : public Sensor
{
    public:
        //sensorType type;
        CollisionSensor(sensorType type,
               OBJECT_TYPE sensor_type,
               glm::vec2 position,
               float radius);
        virtual ~CollisionSensor();
        virtual void update(float dt);
        virtual void draw();

        void setParentVelocity(b2Vec2 velocity);
        void setParentForce(b2Vec2* parentForce);

    protected:
    private:
        //int32 size;
        b2CircleShape circle;
        //float radius;
        //float radiusMovable;
        b2Vec2* pushForce;
};

#endif // COLLISIONSENSOR_H
