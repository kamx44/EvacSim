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

    size = 3;
    mass = 10;
    velocity = getRandomVec2(-4,4);
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
    fixtureDef.filter.maskBits = 0x0008 | 0x0020;

    body = World::addToWorld(bodyDef);
    setToWorld();
    setFixtureToBody();
    body->GetMass();
    drawable = true;
    //
    communicateSensor = new Sensor(OBJECT_TYPE::SENSOR_COMMUNICATION,position,20.0f,idObject,objContainer);
    communicateSensor->setVisibleObjectsIdContainer(actorsToCommunicateIds);
    //communicateSensor->setDrawable(false);
    objContainer->addObject(communicateSensor);
    createJoint(communicateSensor->getBody());
    sightSensor = new Sensor(OBJECT_TYPE::SENSOR_SIGHT,position,30.0f,idObject,objContainer);
    sightSensor->setVisibleObjectsIdPosContainer(inSightObjectsIds);
    //sightSensor->setDrawable(false);
    objContainer->addObject(sightSensor);
    createJoint(sightSensor->getBody());
    collisionSensor = new Sensor(OBJECT_TYPE::SENSOR_COLLISION,position,2.4f,idObject,objContainer);
    collisionSensor->setParentForce(&moveForce);
    collisionSensor->setDrawable(false);
    objContainer->addObject(collisionSensor);
    createJoint(collisionSensor->getBody());
    moveSensor = new Sensor(OBJECT_TYPE::SENSOR_MOVE,position,4.0f,idObject,objContainer);
    moveSensor->setParentForce(&moveForce);
    objContainer->addObject(moveSensor);
    createJoint(moveSensor->getBody());

    //thread communicate(communication,std::ref(inSightObjectsIds),std::ref(actorsToCommunicateIds));
    body->SetTransform( body->GetPosition(), 0 );


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
        /*glBegin(GL_LINE_LOOP);
        if ( m_contacting )
            glColor3f(1,0,0);//red
        else
            glColor3f(1,1,1);//white

        for(int j=0; j<vertexCount; j++)
        {
            x = vertices[j].x;
            y = vertices[j].y;
            glVertex3f (x,y,0);

        }
        glEnd();
        glPopMatrix (); */
}

void Actor::update(float dt)
{
    setParameters(dt);
    setOrientation(fOrientation);
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
   calcSpeed();

    body->ApplyForce(multiplyB2Vec2(normalize(moveForce),10),body->GetWorldCenter(),true);
    //body->ApplyLinearImpulse(moveForce,body->GetWorldCenter(),true);
}

void Actor::calcSpeed()
{

   /*int mouse_x = Events::getInstance().mouseX;
    int mouse_y = Events::getInstance().mouseY;
    b2Vec2 target = cursor->getPosition()+(b2Vec2(mouse_x/6,-mouse_y/6));  */

    //cout<<"VEC: x:"<<moveForce.x<<" y: "<<moveForce.y<<endl;

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

    /*if(moveForce.x<-2) moveForce.x=-2;
    if(moveForce.x>2) moveForce.x=2; */
    if(moveForce.x>-0.5f && moveForce.x<0.5f){
        if(moveForce.x<=0)
            moveForce.x-=0.25;
        else
            moveForce.x+=0.25;
    }
   /* if(moveForce.y<-2) moveForce.y=-2;
    if(moveForce.y>2) moveForce.y=2; */
    if(moveForce.y>-0.5f && moveForce.y<0.5f){
        if(moveForce.y<=0)
            moveForce.y-=0.25;
        else
            moveForce.y+=0.25;
    }


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

void Actor::communication(std::vector<std::pair<unsigned int,b2Vec2> >& inSightObjectsIds,std::vector<unsigned int>& actorsToCommunicateIds){
    using std::cout;
    using std::endl;
    while(!stop_thread){

        std::vector<unsigned int> actorsToCommunicate(actorsToCommunicateIds);
        m1.lock();

        std::pair<messageType,std::pair<unsigned int, b2Vec2> > message = communicationBridge->readMessage(idObject);
        m1.unlock();

        std::vector<b2Vec2> exitContainer;
        bool iSeeThisExit = false;
        for(auto inSightObject : inSightObjectsIds){
            bool oldExit=false;
            if(message.first==messageType::EXIT && message.second.first==inSightObject.first)
                iSeeThisExit = true;
            for(auto passedExit : passedExits){
                if(passedExit == inSightObject.first){
                    oldExit=true;
                }
            }
            if(!oldExit)
                exitContainer.push_back(inSightObject.second);
        }
        if(message.first==messageType::EXIT && !iSeeThisExit){
            bool oldExit=false;
            for(auto passedExit : passedExits){
                if(passedExit == message.second.first){
                    oldExit=true;
                }
            }
            if(!oldExit)
                exitContainer.push_back(message.second.second);
        }
        if(exitContainer.size()>0){
            //LOSUJ
            int randomExit = (std::rand() % exitContainer.size());
            moveForce = multiplyB2Vec2(normalize(exitContainer[randomExit] - body->GetPosition()),10);
        }

        while(!actorsToCommunicate.empty() && !inSightObjectsIds.empty())
        {

           m1.lock();

            std::pair<unsigned int, b2Vec2> parka(inSightObjectsIds[0].first, inSightObjectsIds[0].second);
            std::pair<messageType,std::pair<unsigned int, b2Vec2> > mes(messageType::EXIT,parka);
            communicationBridge->sendMessage(actorsToCommunicate.at(0),mes);
            m1.unlock();
            actorsToCommunicate.pop_back();
        }
        m1.lock();
        if(!inSightObjectsIds.empty())
            inSightObjectsIds.pop_back();
        m1.unlock();

        std::this_thread::sleep_for( std::chrono::seconds(1) );
    }
}

void Actor::setPassedExit(unsigned int passedExitId){
    passedExits.insert(passedExitId);

}


