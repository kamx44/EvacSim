#ifndef OBSTACLESENSOR_H
#define OBSTACLESENSOR_H
#include "Sensor.h"
#include <unordered_map>


class ObstacleSensor : public Sensor
{
    public:

        ObstacleSensor(sensorType type,
               OBJECT_TYPE sensor_type,
               glm::vec2 position,
               float radius);
        virtual ~ObstacleSensor();
        virtual void update(float dt);
        virtual void draw();

        void moveBody(b2Vec2 velocity);
        unsigned int getWhereIWasSend();
        void sendObstacleSensor(b2Vec2 destination);
        void setWhereIWasSend(unsigned int);

    protected:
    private:
        //int32 size;
        b2CircleShape circle;
        //float radius;
        //float radiusMovable;
        unsigned int whereIWasSend;


};

#endif // OBSTACLESENSOR_H
