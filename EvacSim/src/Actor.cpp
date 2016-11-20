#include <GL/gl.h>
#include <GL/glu.h>
#include <iostream>
#include <thread>
#include "ObjectsContainer.h"
#include "Actor.h"
#include "Events.h"
#include "Mmath.h"
#include "stdio.h"
#include <typeinfo>



using namespace std;
extern std::mutex m1;
Actor::Actor(Sector* sector,ObjectsContainer* objContainer,CommunicationBridge* comBridge, Cursor* _cursor): the_thread()
{

    idObject = getRandomId();
    object_type = OBJECT_TYPE::ACTOR;
    position = glm::vec2(getRandomInt(sector->getLeftDownCornerX()+2,sector->getLeftDownCornerX()+8),
                         getRandomInt(sector->getRightUpCornerY()-8,sector->getRightUpCornerY()-2));
    communicationBridge = comBridge;
    cursor = _cursor;
    communicationBridge->addThread(idObject);
    mainDirection = MainDirection();

    size = 3;
    mass = 10;
    velocity = glm::vec2(0,0);//getRandomVec2(-4,4);
    moveForce = createB2vec2(getRandomVec2(-3,3));
    fOrientation = 0;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(position[0], position[1]);
    //vertexCount = 4;
    //vertices = new b2Vec2[4];
    //buildBlob(4,2,2);
    //bodyShape.Set(vertices, 4);
    //fixtureDef.shape = &bodyShape;
    //fixtureDef.filter.categoryBits = 0x0002;
    //fixtureDef.filter.maskBits = 0x0004;
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
    //
    communicateSensor = new Sensor(sensorType::NORMAL,OBJECT_TYPE::SENSOR_COMMUNICATION,position,40.0f,idObject,objContainer);
    communicateSensor->setVisibleObjectsIdContainer(actorsToCommunicateIds);
    //communicateSensor->setDrawable(false);
    objContainer->addObject(communicateSensor);
    createJoint(communicateSensor->getBody());
    sightSensor = new Sensor(sensorType::NORMAL,OBJECT_TYPE::SENSOR_SIGHT,position,40.0f,idObject,objContainer);
    sightSensor->setVisibleObjectsIdPosContainer(inSightObjectsIds);
    sightSensor->setParentSensor(this);
    sightSensor->setParentMainDirection(mainDirection);
    //sightSensor->setDrawable(false);
    objContainer->addObject(sightSensor);
    createJoint(sightSensor->getBody());
    collisionSensor = new Sensor(sensorType::NORMAL,OBJECT_TYPE::SENSOR_COLLISION,position,1.5f,idObject,objContainer);
    collisionSensor->setParentForce(&moveForce);
    collisionSensor->setParentSensor(this);
    collisionSensor->setDrawable(false);
    objContainer->addObject(collisionSensor);
    createJoint(collisionSensor->getBody());
    moveSensor = new Sensor(sensorType::NORMAL,OBJECT_TYPE::SENSOR_MOVE,position,4.0f,idObject,objContainer);
    moveSensor->setParentForce(&moveForce);
    moveSensor->setParentSensor(this);
    objContainer->addObject(moveSensor);
    createJoint(moveSensor->getBody());
    //thread communicate(communication,std::ref(inSightObjectsIds),std::ref(actorsToCommunicateIds));
    body->SetTransform( body->GetPosition(), 0 );
    previousPosition = body->GetPosition();
    constPositionCounter = 0;
}


Actor::~Actor()
{
    stop_thread = true;
    if(the_thread.joinable()) the_thread.join();
}

void Actor::draw()
{
        float x = body->GetPosition().x;
        float y = body->GetPosition().y;
        //cout<<"position: x:"<<x<<" y: "<<y<<endl;
        float angle = body->GetAngle();
        //float angle = fOrientation;
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
}

void Actor::update(float dt)
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
    if(constPositionCounter > 5){
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
        if(body->GetPosition()==previousPosition){
            constPositionCounter++;
        }else
            constPositionCounter=0;
        previousPosition = body->GetPosition();

}

void Actor::calcSpeed()
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

void Actor::createJoint(b2Body* bodyB)
{
    //jointDef.collideConnected=false;
    jointDef.Initialize(body, bodyB, body->GetWorldCenter());
    jointWeld = (b2WeldJoint*)World::world.CreateJoint( &jointDef );
}

void Actor::writeToBuffer(string message){
        readBuffer = message;
    }

void Actor::run(){
    cout<<"Read Buffer: "<<readBuffer<<endl;
}

std::pair<unsigned int,b2Vec2> Actor::takeTheMostCommonExit(std::unordered_map<unsigned int, std::pair<int,b2Vec2> > &countsExitsContainer){
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

void Actor::addToCountsExitContainer(std::unordered_map<unsigned int, std::pair<int,b2Vec2> > &countsExitsContainer,int idExit, b2Vec2 positionExit){
    if(countsExitsContainer.count(idExit)==0){
        std::pair<int,b2Vec2> newPair(1,positionExit);
        countsExitsContainer.insert(std::make_pair(idExit,newPair));
    }else{
        std::pair<int,b2Vec2> newPair(countsExitsContainer[idExit].first++,positionExit);
        countsExitsContainer[idExit]=newPair;
    }
}

void Actor::communication(std::vector<std::pair<unsigned int,b2Vec2> >& inSightObjectsIds,std::vector<unsigned int>& a){
    using std::cout;
    using std::endl;
    while(!stop_thread){

        m1.lock();
            std::vector<std::pair<messageType,std::pair<unsigned int, b2Vec2> >> messagesList = communicationBridge->readMessage(idObject);
        m1.unlock();

        std::unordered_map<unsigned int, std::pair<int,b2Vec2> > countsExitsContainer;
        for(message : messagesList){
            if(message.first == messageType::EXIT){
                addToCountsExitContainer(countsExitsContainer,message.second.first,message.second.second);
            }else if(message.first == messageType::MAIN_EXIT){
                mainDirection.setParameters(message.second.first,message.second.second,false);
                mainDirection.isMainExit = true;
            }
        }
        if(mainDirection.id==0 || mainDirection.isPassed(passedExits)){
            m1.lock(); //mozna sprawdzic ze jak bedzie iles wiecej to wtedy zmieni
                while(!inSightObjectsIds.empty()){
                    addToCountsExitContainer(countsExitsContainer,inSightObjectsIds[0].first,inSightObjectsIds[0].second);
                    inSightObjectsIds.pop_back();   //nie jestem pewien
                }
                int exitsSize = countsExitsContainer.size();
            m1.unlock();
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
            moveForce = multiplyB2Vec2(normalize(mainDirection.position - body->GetPosition()),100);
        }
        m1.lock();
        std::vector<unsigned int> actorsToCommunicate(actorsToCommunicateIds);
        m1.unlock();
        while(!actorsToCommunicate.empty() && mainDirection.id!=0)  //niewiadomo czy zeruje
        {
           m1.lock();
           std::pair<unsigned int, b2Vec2> parka(mainDirection.id, mainDirection.position);
           if(mainDirection.isMainExit){
                std::pair<messageType,std::pair<unsigned int, b2Vec2> > mes(messageType::MAIN_EXIT,parka);
                communicationBridge->sendMessage(actorsToCommunicate.at(0),mes);
           }
           else{
                std::pair<messageType,std::pair<unsigned int, b2Vec2> > mes(messageType::EXIT,parka);
                communicationBridge->sendMessage(actorsToCommunicate.at(0),mes);
           }
           m1.unlock();
           actorsToCommunicate.pop_back();
        }
        //std::this_thread::sleep_for( std::chrono::seconds(1) );
    }
}

void Actor::setPassedExit(unsigned int passedExitId){
    passedExits.insert(passedExitId);

}

void Actor::resetToBeggining(){
    m1.lock();
        inSightObjectsIds.clear();
        actorsToCommunicateIds.clear();
        passedExits.clear();
        communicationBridge->clearAgentMessages(idObject);
        mainDirection = MainDirection();
        moveForce = createB2vec2(getRandomVec2(-3,3));
    m1.unlock();
}




