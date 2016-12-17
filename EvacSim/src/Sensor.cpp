#include <GL/gl.h>
#include <GL/glu.h>
#include "Sensor.h"
#include "Mmath.h"
#include <mutex>
#include <utility>


Sensor::Sensor(sensorType type,OBJECT_TYPE sensor_type,glm::vec2 position,float radius)
{
    idObject = getRandomId();
    this->type = type;
    object_type = sensor_type;
    this->position = position;
    velocity = glm::vec2(0,0);
    fOrientation = 0;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(position[0], position[1]);
    this->radius = radius;

    //drawable = true;
    //radiusMovable = 1.0f;
    //body = World::addToWorld(bodyDef);
    //body->SetBullet(true);
    //setToWorld();
    //setFixtureToBody();
}

Sensor::~Sensor()
{

}

b2Body* Sensor::getBody(){
    return body;
}

void Sensor::setParentSensor(Object* pSensor){
    parentSensor = pSensor;
}

void Sensor::setDrawable(bool draw){
    drawable = draw;
}

Object* Sensor::getParentSensor(){
    if(parentSensor){
        return parentSensor;
    }else{
        return nullptr;
    }
}

