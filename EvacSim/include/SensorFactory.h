#ifndef SENSORFACTORY_H
#define SENSORFACTORY_H

#include "CollisionSensor.h"
#include "SightSensor.h"
#include "CommunicateSensor.h"
#include "MoveSensor.h"
#include "ObstacleSensor.h"


class SensorFactory
{
    public:
        SensorFactory();
        virtual ~SensorFactory();
        static Sensor* createSensor(sensorType sType,glm::vec2 position,float radius){
            if(sType==sensorType::COMMUNICATE_SENSOR){
                CommunicateSensor* sensor = new CommunicateSensor(sType, OBJECT_TYPE::SENSOR_COMMUNICATION, position, radius);
                return sensor;
            }
            else if(sType==sensorType::SIGHT_SENSOR){
                SightSensor* sensor = new SightSensor(sType, OBJECT_TYPE::SENSOR_SIGHT, position, radius);
                return sensor;
            }
            else if(sType==sensorType::COLLISION_SENSOR){
                CollisionSensor* sensor = new CollisionSensor(sType, OBJECT_TYPE::SENSOR_COLLISION, position, radius);
                return sensor;
            }
            else if(sType==sensorType::MOVE_SENSOR){
                MoveSensor* sensor = new MoveSensor(sType, OBJECT_TYPE::SENSOR_MOVE, position, radius);
                return sensor;
            }
            else if(sType==sensorType::OBSTACLE_ACTOR){
                ObstacleSensor* sensor = new ObstacleSensor(sType, OBJECT_TYPE::SENSOR_OBSTACLE, position, radius);
                return sensor;
            }
            else if(sType==sensorType::OBSTACLE_EXIT){
                ObstacleSensor* sensor = new ObstacleSensor(sType, OBJECT_TYPE::SENSOR_OBSTACLE, position, radius);
                return sensor;
            }
            return nullptr;
        }
    protected:

    private:
};

#endif // SENSORFACTORY_H
