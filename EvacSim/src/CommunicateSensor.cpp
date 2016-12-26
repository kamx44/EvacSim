#include <GL/gl.h>
#include <GL/glu.h>
#include <iostream>

#include <thread>
#include <utility>
#include "Mmath.h"
#include "CommunicateSensor.h"
#include "SensorFactory.h"



//using namespace std;
extern std::mutex m1;
CommunicateSensor::CommunicateSensor(sensorType type,OBJECT_TYPE sensor_type,glm::vec2 position,float radius) :
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

CommunicateSensor::~CommunicateSensor()
{
    /*if(!obstacleSensorContainer.empty()){
        for (auto it = obstacleSensorContainer.begin(); it != obstacleSensorContainer.end(); ) {
                unsigned int id = (*it).first;
                (*it).second->destroyBody();
                delete (*it).second;
                (*it).second = NULL;
                it = obstacleSensorContainer.erase(it);
        }
    }*/
}

void CommunicateSensor::delObject(){
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
void CommunicateSensor::draw()
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
                if(obstacleSensor.second->drawable)
                    obstacleSensor.second->draw();
            }
        }
}

void CommunicateSensor::update(float dt)
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
    createObstacleSensorsAgents();
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


void CommunicateSensor::addObjectId(unsigned int visibleObjectId){
    bool exist = false;
    for(unsigned int i=0; i<visibleObjectsIdContainer->size(); i++){
        if(visibleObjectsIdContainer->at(i) == visibleObjectId){
            exist = true;
            break;
        }
    }
    if(!exist){
        std::lock_guard<std::mutex> lock(m1);
        visibleObjectsIdContainer->push_back(visibleObjectId);
        //locker.unlock();
    }
}


void CommunicateSensor::removeObjectId(unsigned int visibleObjectId){
    std::lock_guard<std::mutex> lock(m1);
    for(unsigned int i=0; i<visibleObjectsIdContainer->size(); i++)
    {
        if(visibleObjectId==visibleObjectsIdContainer->at(i))
        {
                visibleObjectsIdContainer->erase(visibleObjectsIdContainer->begin()+i);
                break;
        }
    }
    //locker.unlock();
}


void CommunicateSensor::addObjectToCheckAgentContainer(unsigned int visibleObjectId, b2Vec2 position){
    if(checkAgentContainer.count(visibleObjectId) == 0){
        std::pair<bool,b2Vec2> positionPair(false,position);
        std::pair<unsigned int,std::pair<bool,b2Vec2>> newPair(visibleObjectId,positionPair);
        checkAgentContainer.insert(newPair);
    }
}


bool CommunicateSensor::removeObjectFromCheckAgentContainer(unsigned int visibleObjectIdAgent, unsigned int visibleObjectIdSensor){
    if(!checkAgentContainer.empty() && (checkAgentContainer.count(visibleObjectIdAgent) > 0) && (obstacleSensorContainer.count(visibleObjectIdSensor) > 0)){
        checkAgentContainer.erase(visibleObjectIdAgent);
        return true;
    }else
        return false;
}

void CommunicateSensor::setVisibleObjectsIdContainer(std::vector<unsigned int>& _visibleObjects){
    visibleObjectsIdContainer = &_visibleObjects;
}

void CommunicateSensor::createObstacleSensorsAgents(){
    for(auto onePair : checkAgentContainer){
        if(onePair.second.first == false){
            Sensor* obstacleSensor = SensorFactory::createSensor(sensorType::OBSTACLE_ACTOR,createGlmVec(body->GetPosition()),0.1f);
            std::pair<unsigned int, Sensor*> para(obstacleSensor->getId(),obstacleSensor);
            obstacleSensorContainer.insert(para);
            obstacleSensor->setParentSensor(this);
            obstacleSensor->sendObstacleSensor(onePair.second.second);
            obstacleSensor->setWhereIWasSend(onePair.first);
            std::pair<bool,b2Vec2> positionPair(true,onePair.second.second);
            checkAgentContainer[onePair.first] = positionPair;
        }
    }
}

