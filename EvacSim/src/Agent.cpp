#include <GL/gl.h>
#include <GL/glu.h>
#include <iostream>
#include <thread>
#include "ObjectsContainer.h"
#include "Agent.h"
#include "Events.h"
#include "Mmath.h"
#include "stdio.h"
#include <typeinfo>
#include "SensorFactory.h"



using namespace std;
extern std::mutex m1;
Agent::Agent(Sector* sector,ObjectsContainer* objContainer,CommunicationBridge* comBridge, Cursor* _cursor): the_thread()
{

    idObject = getRandomId();
    this->objContainer = objContainer;
    object_type = OBJECT_TYPE::ACTOR;
    position = glm::vec2(getRandomInt(sector->getLeftDownCornerX()+2,sector->getLeftDownCornerX()+8),
                         getRandomInt(sector->getRightUpCornerY()-8,sector->getRightUpCornerY()-2));
    communicationBridge = comBridge;
    cursor = _cursor;
    communicationBridge->addThread(idObject);
    mainDirection = MainDirection();

    size = 3;
    mass = rand() % 50 +50;
    velocity = glm::vec2(0,0);
    moveForce = createB2vec2(getRandomVec2(-3,3));
    fOrientation = 0;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(position[0], position[1]);
    radius = 1.0f;
    circle.m_radius = radius;
    fixtureDef.shape = &circle;
    fixtureDef.density = 0.2f;
    fixtureDef.friction = 0.3f;
    fixtureDef.filter.categoryBits = 0x0010;
    fixtureDef.filter.maskBits = 0x0008 | 0x0020 | 0x0040;
    body = World::addToWorld(bodyDef);
    setToWorld();
    setFixtureToBody();
    body->GetMass();
    drawable = true;
    //Sensor* s = new Sensor(sensorType::COMMUNICATE_SENSOR,OBJECT_TYPE::SENSOR_COMMUNICATION,position,40.0f);
    communicateSensor = SensorFactory::createSensor(sensorType::COMMUNICATE_SENSOR,position,40.0f);
    communicateSensor->setVisibleObjectsIdContainer(actorsToCommunicateIds);
    //communicateSensor->setDrawable(false);
    //objContainer->addObject(communicateSensor);
    sensors.insert(communicateSensor);
    createJoint(communicateSensor->getBody());
    sightSensor = SensorFactory::createSensor(sensorType::SIGHT_SENSOR,position,40.0f);
    sightSensor->setVisibleObjectsIdPosContainer(inSightObjectsIds);
    sightSensor->setParentSensor(this);
    sightSensor->setParentMainDirection(mainDirection);
    //sightSensor->setDrawable(false);
    //objContainer->addObject(sightSensor);
    sensors.insert(sightSensor);
    createJoint(sightSensor->getBody());
    collisionSensor = SensorFactory::createSensor(sensorType::COLLISION_SENSOR,position,1.5f);
    collisionSensor->setParentForce(&moveForce);
    collisionSensor->setParentSensor(this);
    collisionSensor->setDrawable(true);
    //objContainer->addObject(collisionSensor);
    sensors.insert(collisionSensor);
    createJoint(collisionSensor->getBody());
    moveSensor = SensorFactory::createSensor(sensorType::MOVE_SENSOR,position,4.0f);
    moveSensor->setParentForce(&moveForce);
    moveSensor->setParentSensor(this);
    //objContainer->addObject(moveSensor);
    sensors.insert(moveSensor);
    createJoint(moveSensor->getBody());
    body->SetTransform( body->GetPosition(), 0 );
    previousPosition = body->GetPosition();
    constPositionCounter = 0;
}


Agent::~Agent()
{
    stop_thread = true;
    if(the_thread.joinable()) the_thread.join();
    if(collisionSensor != NULL){
            collisionSensor->isAlive=false;
            collisionSensor->delObject();
            collisionSensor->destroyBody();
            delete collisionSensor;
            collisionSensor = NULL;
            //objContainer->deleteObject(collisionSensor);
    }
    if(communicateSensor != NULL){
            communicateSensor->isAlive=false;
            communicateSensor->delObject();
            communicateSensor->destroyBody();
            delete communicateSensor;
            communicateSensor = NULL;
            //objContainer->deleteObject(communicateSensor);
    }
    if(sightSensor != NULL){
            sightSensor->isAlive=false;
            sightSensor->delObject();
            sightSensor->destroyBody();
            delete sightSensor;
            sightSensor = NULL;
            //objContainer->deleteObject(sightSensor);
    }
    if(moveSensor != NULL){
            moveSensor->isAlive=false;
            moveSensor->delObject();
            moveSensor->destroyBody();
            delete moveSensor;
            moveSensor = NULL;
            //objContainer->deleteObject(moveSensor);
    }
}

void Agent::delObject(){
    stop_thread = true;
    if(the_thread.joinable()) the_thread.join();
    if(collisionSensor != NULL){
            collisionSensor->isAlive=false;
            collisionSensor->delObject();

            //World::world.DestroyJoint(collisionSensor->getBody()->GetJointList()->joint);
            //collisionSensor->destroyBody();
            //delete collisionSensor;
            //collisionSensor = NULL;
            //objContainer->deleteObject(collisionSensor);
    }
    if(communicateSensor != NULL){
            communicateSensor->isAlive=false;
            communicateSensor->delObject();
            //World::world.DestroyJoint(communicateSensor->getBody()->GetJointList()->joint);
            //communicateSensor->destroyBody();
            //delete communicateSensor;
            //communicateSensor = NULL;
            //objContainer->deleteObject(communicateSensor);
    }
    if(sightSensor != NULL){
            sightSensor->isAlive=false;
            sightSensor->delObject();
            //World::world.DestroyJoint(sightSensor->getBody()->GetJointList()->joint);
            //sightSensor->destroyBody();
            //delete sightSensor;
            //sightSensor = NULL;
            //objContainer->deleteObject(sightSensor);
    }
    if(moveSensor != NULL){
            moveSensor->isAlive=false;
            moveSensor->delObject();
            //World::world.DestroyJoint(moveSensor->getBody()->GetJointList()->joint);
            //moveSensor->destroyBody();
            //delete moveSensor;
            //moveSensor = NULL;
            //objContainer->deleteObject(moveSensor);
    }
}

void Agent::draw()
{
        float x = body->GetPosition().x;
        float y = body->GetPosition().y;
        float angle = body->GetAngle();
        glPushMatrix ();
        glLineWidth (3.0f);
        glTranslatef(x,y,0);
        glRotatef( RADTODEG(angle) , 0, 0, 1 );
        glBegin(GL_LINE_LOOP);
        if ( m_contacting )
            glColor3f(1,0,0);//red
        else
            glColor3f(1,1,1);//white
        for (int j=0; j < 360; j++)
        {
            float degInRad = DEGTORAD(j);
            glVertex2f(cos(degInRad)*radius,sin(degInRad)*radius);
        }
        glEnd();
        glPopMatrix ();
        if(!sensors.empty()){
            for(auto sensor : sensors)
                sensor->draw();
        }

}

void Agent::update(float dt)
{
    //position = createGlmVec(body->GetPosition());
    //fOrientation = (body->GetAngle());
    //-------------------------------------------------------
    // Integrate velocity (implicit linear velocity)
    //-------------------------------------------------------
    //velocity		+= force  * (inverseMass    * dt);
    //angularVelocity	+= torque * (inverseInertia * dt);

    //-------------------------------------------------------
    // clear forces
    //-------------------------------------------------------
    //force		 = glm::vec2(0.0f, 0.0f);
   //torque	 = 0.0f;
    //body->SetAngularVelocity(DEGTORAD(45));
    //body->ApplyAngularImpulse(33,true);
    //body->
    //cout<<"FORCE: x:"<<moveForce.x<<" y: "<<moveForce.y<<endl;
    if(constPositionCounter > 2){
        resetToBeggining();
        constPositionCounter = 0;
    }
    setParameters(dt);
    setOrientation(fOrientation);
    calcSpeed();
    b2Vec2 norm = normalize(moveForce);
    if(norm.x>-0.5f && norm.x<0.5f){
        if(norm.x<=0)
            norm.x-=0.25;
        else
            norm.x+=0.25;
    }
    if(norm.y>-0.5f && norm.y<0.5f){
        if(norm.y<=0)
            norm.y-=0.25;
        else
            norm.y+=0.25;
    }

    body->ApplyForce(multiplyB2Vec2(norm,10),body->GetWorldCenter(),true);

    float x1 = round(body->GetPosition().x*10000);
    float y1 = round(body->GetPosition().y*10000);
    float x2 = round(previousPosition.x*10000);
    float y2 = round(previousPosition.y*10000);
    if(x1==x2 && y1==y2){
        constPositionCounter++;
    }else
        constPositionCounter=0;
    previousPosition = body->GetPosition();
    if(!sensors.empty()){
        for(auto sensor : sensors)
            sensor->update(dt);
    }

}

void Agent::calcSpeed()
{
    b2Vec2 toTarget;
    toTarget.x=(1000*moveForce.x)+body->GetPosition().x;
    toTarget.y=(1000*moveForce.y)+body->GetPosition().y;
    float desiredAngle = atan2f(-toTarget.x, toTarget.y)+M_PI/2;
    float bodyAngle = this->body->GetAngle();

    ///< calculating rotation

    float totalRotation = desiredAngle - bodyAngle;
    while ( totalRotation < DEGTORAD(-180) ) totalRotation += DEGTORAD(360) ;
    while ( totalRotation > DEGTORAD(180) ) totalRotation -= DEGTORAD(360) ;
    float change = DEGTORAD(1) ;
    float newAngle = bodyAngle + min( change, max(-change, totalRotation));

    this->body->SetTransform( this->body->GetPosition(), newAngle );
    moveSensor->body->SetTransform( moveSensor->body->GetPosition(), newAngle );
    collisionSensor->body->SetTransform( collisionSensor->body->GetPosition(), newAngle );
    communicateSensor->body->SetTransform(communicateSensor->body->GetPosition(), newAngle);
    sightSensor->body->SetTransform(sightSensor->body->GetPosition(), newAngle);
    setOrientation(this->body->GetAngle()+M_PI/2);
    setOrientation(moveSensor->body->GetAngle()+M_PI/2);
    setOrientation(collisionSensor->body->GetAngle()+M_PI/2);
    setOrientation(communicateSensor->body->GetAngle()+M_PI/2);
    setOrientation(sightSensor->body->GetAngle()+M_PI/2);
}

void Agent::createJoint(b2Body* bodyB)
{
    //jointDef.collideConnected=false;
    jointDef.Initialize(body, bodyB, body->GetWorldCenter());
    jointWeld = (b2WeldJoint*)World::world.CreateJoint( &jointDef );
}

void Agent::writeToBuffer(string message){
        readBuffer = message;
    }

void Agent::run(){
    cout<<"Read Buffer: "<<readBuffer<<endl;
}

std::pair<unsigned int,b2Vec2> Agent::takeTheMostCommonExit(std::unordered_map<unsigned int, std::pair<int,b2Vec2> > &countsExitsContainer){
    int maxNumber = -100;
    b2Vec2 positionExit(-1000,-1000);
    unsigned int id = 9999999;
    for(auto exit : countsExitsContainer){
        if(maxNumber < exit.second.first){
            maxNumber = exit.second.first;
            positionExit = exit.second.second;
            id = exit.first;
        }
    }
    if(countsExitsContainer.count(id)>0){
        countsExitsContainer.erase(id);
    }
    if(id == 9999999){
        throw "ERROR: countsExitContainer is empty";
    }else{
        return std::make_pair(id,positionExit);
    }
}

void Agent::addToCountsExitContainer(std::unordered_map<unsigned int, std::pair<int,b2Vec2> > &countsExitsContainer,int idExit, b2Vec2 positionExit){
    if(countsExitsContainer.count(idExit)==0){
        std::pair<int,b2Vec2> newPair(1,positionExit);
        countsExitsContainer.insert(std::make_pair(idExit,newPair));
    }else{
        std::pair<int,b2Vec2> newPair(countsExitsContainer[idExit].first++,positionExit);
        countsExitsContainer[idExit]=newPair;
    }
}

void Agent::communication(std::vector<std::pair<unsigned int,b2Vec2> >& inSightObjectsIds,std::vector<unsigned int>& actorsToCommunicateIds){
    using std::cout;
    using std::endl;

    while(!stop_thread){
    try{
        std::unique_lock<mutex> locker(m1, std::defer_lock);
        locker.lock();
            std::vector<std::pair<messageType,std::pair<unsigned int, b2Vec2> >> messagesList = communicationBridge->readMessage(idObject);
        locker.unlock();

        std::unordered_map<unsigned int, std::pair<int,b2Vec2> > countsExitsContainer;
        for(auto message : messagesList){
            if(message.first == messageType::EXIT){
                addToCountsExitContainer(countsExitsContainer,message.second.first,message.second.second);
            }else if(message.first == messageType::MAIN_EXIT){
                mainDirection.setParameters(message.second.first,message.second.second,false);
                mainDirection.isMainExit = true;
            }
        }
        if(mainDirection.id==0 || mainDirection.isPassed(passedExits)){
            locker.lock(); //mozna sprawdzic ze jak bedzie iles wiecej to wtedy zmieni
                while(!inSightObjectsIds.empty()){
                    addToCountsExitContainer(countsExitsContainer,inSightObjectsIds[0].first,inSightObjectsIds[0].second);
                    inSightObjectsIds.pop_back();   //nie jestem pewien
                }
                int exitsSize = countsExitsContainer.size();
            locker.unlock();
            bool found = false;
            std::pair<unsigned int, b2Vec2> exit;
            while(!found && exitsSize>0){
                exit = takeTheMostCommonExit(countsExitsContainer); //inside decrease taken exit
                bool oldExit=false;
                for(auto passedExit : passedExits){
                    if(passedExit == exit.first){
                        oldExit=true;
                    }
                }
                if(!oldExit || exitsSize==1){
                    mainDirection.setParameters(exit.first,exit.second,false);
                    found = true;
                    exitsSize = countsExitsContainer.size();
                }else{
                    exitsSize = countsExitsContainer.size();
                    continue;
                }
            }
        }else if(mainDirection.id != 0){
            if(body)
                moveForce = multiplyB2Vec2(normalize(mainDirection.position - body->GetPosition()),1000);
        }
        locker.lock();
        std::vector<unsigned int> actorsToCommunicate(actorsToCommunicateIds);
        locker.unlock();
        while(!actorsToCommunicate.empty() && mainDirection.id!=0)  //niewiadomo czy zeruje
        {
           locker.lock();
           std::pair<unsigned int, b2Vec2> parka(mainDirection.id, mainDirection.position);
           if(mainDirection.isMainExit){
                std::pair<messageType,std::pair<unsigned int, b2Vec2> > mes(messageType::MAIN_EXIT,parka);
                communicationBridge->sendMessage(actorsToCommunicate.at(0),mes);
           }
           else{
                std::pair<messageType,std::pair<unsigned int, b2Vec2> > mes(messageType::EXIT,parka);
                communicationBridge->sendMessage(actorsToCommunicate.at(0),mes);
           }
           locker.unlock();
           actorsToCommunicate.pop_back();
        }
    }catch(...){
        std::cout<<"ERROR in thread"<<endl;
        return;
    }
    }
}

void Agent::setPassedExit(unsigned int passedExitId){
    passedExits.insert(passedExitId);

}

void Agent::resetToBeggining(){
    std::lock_guard<std::mutex> lock(m1);
        inSightObjectsIds.clear();
        actorsToCommunicateIds.clear();
        passedExits.clear();
        communicationBridge->clearAgentMessages(idObject);
        mainDirection = MainDirection();
        moveForce = createB2vec2(getRandomVec2(-3,3));
    //locker.unlock();
}




