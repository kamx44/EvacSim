#include <GL/gl.h>
#include <GL/glu.h>
#include <mutex>
#include <utility>
#include "Mmath.h"
#include "SightSensor.h"
#include "SensorFactory.h"

extern std::mutex m1;
SightSensor::SightSensor(sensorType type,OBJECT_TYPE sensor_type,glm::vec2 position,float radius) :
    Sensor(type,sensor_type,position,radius)
{

    circle.m_radius = radius;
    fixtureDef.shape = &circle;
    fixtureDef.density = 0.0f;
    fixtureDef.friction = 0.3f;
    fixtureDef.isSensor = true;
    fixtureDef.filter.categoryBits = 0x0008;
    fixtureDef.filter.maskBits = 0x0010 | 0x0020 | 0x0040;

    drawable = true;
    radiusMovable = 1.0f;
    body = World::addToWorld(bodyDef);
    body->SetBullet(true);
    setToWorld();
    setFixtureToBody();
    body->GetMass();
}

SightSensor::~SightSensor()
{
    /*if(!obstacleSensorContainer.empty()){
        for (auto it = obstacleSensorContainer.begin(); it != obstacleSensorContainer.end(); ) {
                unsigned int id = (*it).first;
                (*it).second->destroyBody();
                delete (*it).second;
                (*it).second = NULL;
                it = obstacleSensorContainer.erase(it);
        }
    } */
}

void SightSensor::delObject(){
    if(!obstacleSensorContainer.empty()){
        for (auto it = obstacleSensorContainer.begin(); it != obstacleSensorContainer.end(); ) {
                unsigned int id = (*it).first;
                (*it).second->destroyBody();
                delete (*it).second;
                (*it).second = NULL;
                it = obstacleSensorContainer.erase(it);
        }
    }
}

void SightSensor::draw()
{
        float x = position[0];
        float y = position[1];
        float angle = body->GetAngle();
        glPushMatrix ();
        glLineWidth (3.0f);
        glTranslatef(x,y,0);
        glRotatef( RADTODEG(angle) , 0, 0, 1 );
        glBegin(GL_LINE_LOOP);
        glColor3f(0,0.5,0);//white
        for (int j=0; j < 360; j++)
        {
            float degInRad = DEGTORAD(j);
            glVertex2f(cos(degInRad)*radiusMovable,sin(degInRad)*radiusMovable);
        }
        glEnd();
        glPopMatrix ();

        if(!obstacleSensorContainer.empty()){
            for(auto obstacleSensor : obstacleSensorContainer){
                obstacleSensor.second->draw();
            }
        }
}

void SightSensor::update(float dt)
{
    if(radiusMovable<radius)
        radiusMovable += 5.0f;
    else if(radiusMovable>=radius)
        radiusMovable = 1.0f;
    body->DestroyFixture(fixture);
    circle.m_radius = radiusMovable;
    fixtureDef.shape = &circle;
    fixtureDef.density = 0.0f;
    fixtureDef.friction = 0.3f;
    fixtureDef.isSensor = true;
    setFixtureToBody();

    setParameters(dt);
    createObstacleSensorsExit();
    if(!obstacleSensorContainer.empty()){
        for (auto it = obstacleSensorContainer.begin(); it != obstacleSensorContainer.end(); ) {
            if((*it).second->isAlive == false){
                unsigned int id = (*it).first;
                (*it).second->destroyBody();
                delete (*it).second;
                (*it).second = NULL;
                it = obstacleSensorContainer.erase(it);
            }else{
                (*it).second->update(0.1f);
                it++;
            }
        }
    }

}

void SightSensor::addObjectIdPos(unsigned int visibleObjectId,b2Vec2 position){
    std::pair<unsigned int,b2Vec2> visibleObject(visibleObjectId,position);
    bool exist = false;
    for(unsigned int i=0; i<visibleObjectsIdPosContainer->size(); i++){
        if(visibleObjectsIdPosContainer->at(i).first == visibleObjectId){
            exist = true;
            break;
        }
    }
    std::lock_guard<std::mutex> lock(m1);
    if(!exist){
        visibleObjectsIdPosContainer->push_back(visibleObject);
    }

}


void SightSensor::removeObjectIdPos(unsigned int visibleObjectId){
    std::lock_guard<std::mutex> lock(m1);
    for(unsigned int i=0; i<visibleObjectsIdPosContainer->size(); i++)
    {
        if(visibleObjectId==visibleObjectsIdPosContainer->at(i).first)
        {
                visibleObjectsIdPosContainer->erase(visibleObjectsIdPosContainer->begin()+i);
                break;
        }
    }

}
void SightSensor::addObjectToCheckExitContainer(unsigned int visibleObjectId, b2Vec2 position){
    if(checkExitContainer.count(visibleObjectId) == 0){
        std::pair<bool,b2Vec2> positionPair(false,position);
        std::pair<unsigned int,std::pair<bool,b2Vec2>> newPair(visibleObjectId,positionPair);
        checkExitContainer.insert(newPair);
     }
}


bool SightSensor::removeObjectFromCheckExitContainer(unsigned int visibleObjectIdExit, unsigned int visibleObjectIdSensor){
    if(!checkExitContainer.empty() && (checkExitContainer.count(visibleObjectIdExit) > 0) && (obstacleSensorContainer.count(visibleObjectIdSensor) > 0)){
        checkExitContainer.erase(visibleObjectIdExit);
        return true;
    }else
        return false;
}

void SightSensor::setVisibleObjectsIdPosContainer(std::vector<std::pair<unsigned int,b2Vec2> >& _visibleObjects){
    visibleObjectsIdPosContainer = &_visibleObjects;
}

void SightSensor::createObstacleSensorsExit(){
    for(auto onePair : checkExitContainer){
        if(onePair.second.first == false){
            Sensor* obstacleSensor = SensorFactory::createSensor(sensorType::OBSTACLE_EXIT,createGlmVec(body->GetPosition()),0.1f);
            std::pair<unsigned int, Sensor*> para(obstacleSensor->getId(),obstacleSensor);
            obstacleSensorContainer.insert(para);
            obstacleSensor->setParentSensor(this);
            obstacleSensor->sendObstacleSensor(onePair.second.second);
            obstacleSensor->setWhereIWasSend(onePair.first);
            std::pair<bool,b2Vec2> positionPair(true,onePair.second.second);
            checkExitContainer[onePair.first] = positionPair;
        }
    }
}

void SightSensor::setParentMainDirection(MainDirection &mainDir){
    mainDirection = &mainDir;
}

void SightSensor::setMainExitDirection(unsigned int mainExitId,b2Vec2 position){
    if(!mainDirection->isMainExit)
        mainDirection->setParameters(mainExitId,position,false);
}
