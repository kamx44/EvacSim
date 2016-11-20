#include <GL/gl.h>
#include <GL/glu.h>
#include "Sensor.h"
#include "Mmath.h"
#include "World.h"
#include "Actor.h"
#include <mutex>
#include <utility>

std::mutex m1;
Sensor::Sensor(sensorType type,OBJECT_TYPE sensor_type,glm::vec2 position,float radius,unsigned int idParent, ObjectsContainer* objContainer)
{
    idObject = getRandomId();
    this->objContainer = objContainer;
    this->type = type;
    object_type = sensor_type;
    this->position = position;
    size = 3;
    mass = 0;
    velocity = glm::vec2(0,0);
    fOrientation = 0;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(position[0], position[1]);
    this->radius = radius;
    if(object_type == OBJECT_TYPE::SENSOR_MOVE){
        vertexCount = 8;
        vertices = new b2Vec2[8];
        vertices[0].Set(0,0);
        int mainAngle=90;
        int j=-3;
        for (int i = 0; i < 7; i++,j++) {
            if(i>=4) mainAngle=90;
            float angle = DEGTORAD(j / 6.0 * mainAngle);// * DEGTORAD;
            vertices[i+1].Set( radius * cosf(angle), radius * sinf(angle) );
        }
        bodyShape.Set(vertices, 8);
        fixtureDef.shape = &bodyShape;
    }else{
        circle.m_radius = radius;
        fixtureDef.shape = &circle;
    }
    fixtureDef.density = 0.0f;
    fixtureDef.friction = 0.3f;
    if(object_type==OBJECT_TYPE::SENSOR_COLLISION || object_type==OBJECT_TYPE::SENSOR_OBSTACLE)
        fixtureDef.isSensor = false;
    else
        fixtureDef.isSensor = true;

    if(object_type==OBJECT_TYPE::SENSOR_OBSTACLE && type==sensorType::OBSTACLE_EXIT){
        fixtureDef.filter.categoryBits = 0x0008;
        fixtureDef.filter.maskBits = 0x0020 | 0x0040;
    }else if(object_type==OBJECT_TYPE::SENSOR_OBSTACLE && type==sensorType::OBSTACLE_ACTOR){
        fixtureDef.filter.categoryBits = 0x0008;
        fixtureDef.filter.maskBits = 0x0010 | 0x0020;
    }else{
        fixtureDef.filter.categoryBits = 0x0008;
        fixtureDef.filter.maskBits = 0x0010 | 0x0020 | 0x0040;
    }

    drawable = true;
    freeNewSensor = false;
    this->idParent = idParent;
    radiusMovable = 1.0f;
    rotateBody=true;
    body = World::addToWorld(bodyDef);
    body->SetBullet(true);
    setToWorld();
    setFixtureToBody();
    body->GetMass();
}

Sensor::~Sensor()
{
    //dtor
}
void Sensor::draw()
{
        float x = position[0];
        float y = position[1];
        float angle = body->GetAngle();
        glPushMatrix ();
        glLineWidth (3.0f);
        glTranslatef(x,y,0);
        glRotatef( RADTODEG(angle) , 0, 0, 1 );
        glBegin(GL_LINE_LOOP);
        if ( m_contacting )
            glColor3f(1,0,0);//red
        else
            glColor3f(0,0.5,0);//white
        if(type == sensorType::OBSTACLE_ACTOR){
            glColor3f(1,0,0);
        }else if(type == sensorType::OBSTACLE_EXIT){
            glColor3f(0,0,1);
        }
        if(object_type == OBJECT_TYPE::SENSOR_MOVE){
            for (int j=0; j < vertexCount; j++)
            {
                glVertex2f(vertices[j].x,vertices[j].y);
            }
        }
        else{
            for (int j=0; j < 360; j++)
            {
                float degInRad = DEGTORAD(j);
                glVertex2f(cos(degInRad)*radiusMovable,sin(degInRad)*radiusMovable);
            }
        }
        glEnd();
        glPopMatrix ();
}

void Sensor::update(float dt)
{
    if(object_type==OBJECT_TYPE::SENSOR_SIGHT || object_type==OBJECT_TYPE::SENSOR_COMMUNICATION){
        if(radiusMovable<radius)
            radiusMovable += 0.5f;
        else if(radiusMovable>=radius)
            radiusMovable = 1.0f;
        body->DestroyFixture(fixture);
        circle.m_radius = radiusMovable;
        fixtureDef.shape = &circle;
        fixtureDef.density = 0.0f;
        fixtureDef.friction = 0.3f;
        fixtureDef.isSensor = true;
        setFixtureToBody();
    }else if(object_type==OBJECT_TYPE::SENSOR_MOVE){
        if(radiusMovable<radius)
            radiusMovable += 0.25f;
        else if(radiusMovable>=radius)
            radiusMovable = 1.0f;
        body->DestroyFixture(fixture);
        int mainAngle=90;
        int j=-3;
        for (int i = 0; i < 7; i++,j++) {
            if(i>=4) mainAngle=90;
            float angle = DEGTORAD(j / 6.0 * mainAngle);// * DEGTORAD;
            vertices[i+1].Set( radiusMovable * cosf(angle), radiusMovable * sinf(angle) );
        }
        bodyShape.Set(vertices, 8);
        fixtureDef.shape = &bodyShape;
        fixtureDef.density = 0.0f;
        fixtureDef.friction = 0.3f;
        fixtureDef.isSensor = true;
        setFixtureToBody();
    }else{
        radiusMovable=radius;

    }

    setParameters(dt);

    if(object_type==OBJECT_TYPE::SENSOR_SIGHT){ //create one function
        createObstacleSensorsExit();
    } else if(object_type==OBJECT_TYPE::SENSOR_COMMUNICATION){
        createObstacleSensorsActors();
    }


}

b2Body* Sensor::getBody(){
    return body;
}

unsigned int Sensor::getParentId(){
    return idParent;
}


void Sensor::addObjectId(unsigned int visibleObjectId){
    bool exist = false;
    for(unsigned int i=0; i<visibleObjectsIdContainer->size(); i++){
        if(visibleObjectsIdContainer->at(i) == visibleObjectId){
            exist = true;
            break;
        }
    }
    if(!exist){
        m1.lock();
        visibleObjectsIdContainer->push_back(visibleObjectId);
        m1.unlock();
    }
}

void Sensor::addObjectIdPos(unsigned int visibleObjectId,b2Vec2 position){
    std::pair<unsigned int,b2Vec2> visibleObject(visibleObjectId,position);
    bool exist = false;
    for(unsigned int i=0; i<visibleObjectsIdPosContainer->size(); i++){
        if(visibleObjectsIdPosContainer->at(i).first == visibleObjectId){
            exist = true;
            break;
        }
    }
    m1.lock();
    if(!exist){
        visibleObjectsIdPosContainer->push_back(visibleObject);
    }
    m1.unlock();
//    if(visibleObjectsIdPosContainer.count(visibleObjectId) == 0){
//        std::pair<unsigned int,b2Vec2> visibleObject(visibleObjectId,position);

//    }
}

void Sensor::removeObjectId(unsigned int visibleObjectId){
    m1.lock();
    for(unsigned int i=0; i<visibleObjectsIdContainer->size(); i++)
    {
        if(visibleObjectId==visibleObjectsIdContainer->at(i))
        {
                visibleObjectsIdContainer->erase(visibleObjectsIdContainer->begin()+i);
                break;
        }
    }
    m1.unlock();
}

void Sensor::removeObjectIdPos(unsigned int visibleObjectId){
    m1.lock();
    for(unsigned int i=0; i<visibleObjectsIdPosContainer->size(); i++)
    {
        if(visibleObjectId==visibleObjectsIdPosContainer->at(i).first)
        {
                visibleObjectsIdPosContainer->erase(visibleObjectsIdPosContainer->begin()+i);
                break;
        }
    }
    m1.unlock();
}
void Sensor::addObjectToCheckExitContainer(unsigned int visibleObjectId, b2Vec2 position){
    if(checkExitContainer.count(visibleObjectId) == 0){
        std::pair<bool,b2Vec2> positionPair(false,position);
        std::pair<unsigned int,std::pair<bool,b2Vec2>> newPair(visibleObjectId,positionPair);
        checkExitContainer.insert(newPair);
     }
}

void Sensor::addObjectToCheckActorContainer(unsigned int visibleObjectId, b2Vec2 position){
    if(checkActorContainer.count(visibleObjectId) == 0){
        std::pair<bool,b2Vec2> positionPair(false,position);
        std::pair<unsigned int,std::pair<bool,b2Vec2>> newPair(visibleObjectId,positionPair);
        checkActorContainer.insert(newPair);
    }
}

bool Sensor::removeObjectFromCheckExitContainer(unsigned int visibleObjectIdExit, unsigned int visibleObjectIdSensor){
    //if(!checkExitContainer.empty() && (checkExitContainer.count(visibleObjectIdExit) > 0) && (obstacleSensorContainer.count(visibleObjectIdSensor) > 0)){
        checkExitContainer.erase(visibleObjectIdExit);
        obstacleSensorContainer.erase(visibleObjectIdSensor);
        return true;
    //}else
    //    return false;
}

bool Sensor::removeObjectFromCheckActorContainer(unsigned int visibleObjectIdActor, unsigned int visibleObjectIdSensor){
    if(!checkActorContainer.empty() && (checkActorContainer.count(visibleObjectIdActor) > 0) && (obstacleSensorContainer.count(visibleObjectIdSensor) > 0)){
        checkActorContainer.erase(visibleObjectIdActor);
        obstacleSensorContainer.erase(visibleObjectIdSensor);
        return true;
    }else
        return false;
}


void Sensor::sendObstacleSensor(b2Vec2 destination){
    b2Vec2 vel = multiplyB2Vec2(normalize(destination - body->GetPosition()),100);
    moveBody(vel);
    //std::cout<<"SSS"<<std::endl;
    //obstacleSensor->moveBody(vel);
}

void Sensor::moveBody(b2Vec2 velocity){
    body->ApplyLinearImpulse(velocity,body->GetWorldCenter(),true);

}

void Sensor::printData(){
/*    using std::cout;
    using std::endl;
    cout<<"Parent ID: "<<idParent<<endl;
    for(unsigned int i=0; i<visibleObjects->size(); i++)
    {
        cout<<"  ID["<<i<<"]: "<<visibleObjects->at(i)<<endl;
    } */
}

void Sensor::setParentVelocity(b2Vec2 pforce){
    //using std::cout;
    //using std::endl;
   // std::cout<<pushForce->x<<std::endl;
  //  std::cout<<pushForce->y<<std::endl;
   // cout<<"stara sila: x:"<<pushForce->x<<" y:"<<pushForce->y<<endl;
   // std::cout<<"nowa sila: x:"<<pforce.x<<" y:"<<pforce.y<<std::endl;

    (*pushForce) += normalize(pforce);
}

void Sensor::setParentSensor(Object* pSensor){
    parentSensor = pSensor;
}

void Sensor::setDrawable(bool draw){
    drawable = draw;
}

void Sensor::setVisibleObjectsIdContainer(std::vector<unsigned int>& _visibleObjects){
    visibleObjectsIdContainer = &_visibleObjects;
}

void Sensor::setVisibleObjectsIdPosContainer(std::vector<std::pair<unsigned int,b2Vec2> >& _visibleObjects){
    visibleObjectsIdPosContainer = &_visibleObjects;
}

void Sensor::setParentForce(b2Vec2* parentForce){
    pushForce = parentForce;
}

Object* Sensor::getParentSensor(){
    if(parentSensor){
        return parentSensor;
    }else{
        return nullptr;
    }
}

void Sensor::createObstacleSensorsExit(){
    for(auto onePair : checkExitContainer){
       // std::cout<<onePair.first<<" "<<onePair.second.first<<std::endl;
        if(onePair.second.first == false){
            Sensor* obstacleSensor = new Sensor(sensorType::OBSTACLE_EXIT,OBJECT_TYPE::SENSOR_OBSTACLE,createGlmVec(body->GetPosition()),0.1f,1,objContainer);
            //obstacleSensor->setDrawable(false);
            std::cout<<"Tworze sensor EXIT"<<std::endl;
            std::pair<unsigned int, Sensor*> para(obstacleSensor->getId(),obstacleSensor);
            obstacleSensorContainer.insert(para);
            //std::make_pair(obstacleSensor->getId(),obstacleSensor)
            obstacleSensor->setParentSensor(this);
            objContainer->addObject(obstacleSensor);
            obstacleSensor->sendObstacleSensor(onePair.second.second);
            obstacleSensor->setWhereIWasSend(onePair.first);
            std::pair<bool,b2Vec2> positionPair(true,onePair.second.second);
            checkExitContainer[onePair.first] = positionPair;
        }
    }
}
void Sensor::createObstacleSensorsActors(){
    for(auto onePair : checkActorContainer){
       // std::cout<<onePair.first<<" "<<onePair.second.first<<std::endl;
        if(onePair.second.first == false){
            Sensor* obstacleSensor = new Sensor(sensorType::OBSTACLE_ACTOR,OBJECT_TYPE::SENSOR_OBSTACLE,createGlmVec(body->GetPosition()),0.1f,1,objContainer);
            //obstacleSensor->type = sensorType::OBSTACLE_ACTOR;
            std::cout<<"Tworze sensor ACTOR"<<std::endl;
            std::pair<unsigned int, Sensor*> para(obstacleSensor->getId(),obstacleSensor);
            obstacleSensorContainer.insert(para);
            //std::make_pair(obstacleSensor->getId(),obstacleSensor)
            obstacleSensor->setParentSensor(this);
            objContainer->addObject(obstacleSensor);
            obstacleSensor->sendObstacleSensor(onePair.second.second);
            obstacleSensor->setWhereIWasSend(onePair.first);
            std::pair<bool,b2Vec2> positionPair(true,onePair.second.second);
            checkActorContainer[onePair.first] = positionPair;
        }
    }
}
unsigned int Sensor::getWhereIWasSend(){
    return whereIWasSend;
}

void Sensor::setWhereIWasSend(unsigned int id){
    whereIWasSend = id;
}

void Sensor::setParentMainDirection(MainDirection &mainDir){
    mainDirection = &mainDir;
}

void Sensor::setMainExitDirection(unsigned int mainExitId,b2Vec2 position){
    if(!mainDirection->isMainExit)
        mainDirection->setParameters(mainExitId,position,false);
}
