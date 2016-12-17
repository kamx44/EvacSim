#ifndef MOVESENSOR_H
#define MOVESENSOR_H
#include "Sensor.h"
#include <unordered_map>



class MoveSensor : public Sensor
{
    public:
        //sensorType type;
        MoveSensor(sensorType type,
               OBJECT_TYPE sensor_type,
               glm::vec2 position,
               float radius);
        virtual ~MoveSensor();
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

#endif // MOVESENSOR_H
