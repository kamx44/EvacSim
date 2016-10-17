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
    vertexCount = 4;
    vertices = new b2Vec2[4];
    buildBlob(4,2,2);
    bodyShape.Set(vertices, 4);
    fixtureDef.shape = &bodyShape;
    //fixtureDef.filter.categoryBits = 0x0002;
    //fixtureDef.filter.maskBits = 0x0004;
    //radius = 1.0f;
    //circle.m_radius = radius;
    //fixtureDef.shape = &circle;
    fixtureDef.density = 0.2f;
    fixtureDef.friction = 0.3f;

   // fixtureDef.filter.categoryBits = 0x0002;
   // fixtureDef.filter.maskBits = 0x0004;
    body = World::addToWorld(bodyDef);
    setToWorld();
    setFixtureToBody();
    body->GetMass();
    drawable = true;
    //communicateSensor = new Sensor(position,5.0f,sensorType::COMMUNICATE);
    //objContainer->addObject(communicateSensor);
    //createJoint(communicateSensor->getBody());
   // sightSensor = new Sensor(OBJECT_TYPE::SENSOR_SIGHT,position,20.0f,idObject,&moveForce,inSightObjectsIds,sensorType::SIGHT);
   // objContainer->addObject(sightSensor);
   // createJoint(sightSensor->getBody());
    collisionSensor = new Sensor(OBJECT_TYPE::SENSOR_COLLISION,position,2.4f,idObject,&moveForce,inSightObjectsIds,sensorType::SIGHT);
    collisionSensor->setDrawable(false);
    objContainer->addObject(collisionSensor);
    createJoint(collisionSensor->getBody());
    moveSensor = new Sensor(OBJECT_TYPE::SENSOR_MOVE,position,4.0f,idObject,&moveForce,inSightObjectsIds,sensorType::SIGHT);
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
        cout<<"position: x:"<<x<<" y: "<<y<<endl;
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

        for(int j=0; j<vertexCount; j++)
        {
            x = vertices[j].x;
            y = vertices[j].y;
            glVertex3f (x,y,0);

        }

        glEnd();
        glPopMatrix ();
        /*
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
        */

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
    body->ApplyForce(moveForce,body->GetWorldCenter(),true);
    //body->ApplyLinearImpulse(moveForce,body->GetWorldCenter(),true);
}

void Actor::calcSpeed()
{

   /* int mouse_x = Events::getInstance().mouseX;
    int mouse_y = Events::getInstance().mouseY;
b2Vec2 target = cursor->getPosition()+(b2Vec2(mouse_x/6,-mouse_y/6));  */
    //b2Vec2 vec = body->
    cout<<"VEC: x:"<<moveForce.x<<" y: "<<moveForce.y<<endl;
   // float desiredAngle = atan2f( 5*vec.x, 5*vec.y)-M_PI;
    //b2Vec2 toTarget = b2Vec2(5*vec.x,5*vec.y) - body->GetPosition();
    //b2Vec2 target = cursor->getPosition()+(b2Vec2(mouse_x/6,-mouse_y/6));
    b2Vec2 toTarget;
    toTarget.x=(1000*moveForce.x)+body->GetPosition().x;
    toTarget.y=(1000*moveForce.y)+body->GetPosition().y;
    float desiredAngle = atan2f(-toTarget.x, toTarget.y)+M_PI/2;
    float bodyAngle = this->body->GetAngle();
    //cout<<"mouse: x:"<<mouse_x<<" y: "<<mouse_y<<endl;
    //cout<<"position C: x:"<<cursor->getPosition().x<<" y: "<<cursor->getPosition().y<<endl;
    //cout<<"position A: x:"<<body->GetPosition().x<<" y: "<<body->GetPosition().y<<endl;
    //Sleep(20);
    //cout<<"desire: "<<desiredAngle<<" bodyAngle: "<<bodyAngle<<endl;
    //cout<<"Body position: x"<<body->GetPosition().x<<" y:"<<body->GetPosition().y<<endl;

    ///< calculating rotation
    //body->SetAngularVelocity(10.0f);
    float totalRotation = desiredAngle - bodyAngle;
    while ( totalRotation < DEGTORAD(-180) ) totalRotation += DEGTORAD(360) ;
    while ( totalRotation > DEGTORAD(180) ) totalRotation -= DEGTORAD(360) ;
    float change = DEGTORAD(1) ;
    float newAngle = bodyAngle + min( change, max(-change, totalRotation));

    //cout<<"newAngle: "<<newAngle<<endl;
    //if(moveSensor->rotateBody){
        this->body->SetTransform( this->body->GetPosition(), newAngle );
        moveSensor->body->SetTransform( moveSensor->body->GetPosition(), newAngle );
        collisionSensor->body->SetTransform( collisionSensor->body->GetPosition(), newAngle );
        setOrientation(this->body->GetAngle()+M_PI/2);
        setOrientation(moveSensor->body->GetAngle()+M_PI/2);
        setOrientation(collisionSensor->body->GetAngle()+M_PI/2);
    //}
   // if(bodyAngle == desiredAngle){
   //     moveSensor->rotateBody=false;
   // }
    if(moveForce.x<-2) moveForce.x=-2;
    if(moveForce.x>2) moveForce.x=2;
    if(moveForce.x>-0.5f && moveForce.x<0.5f){
        if(moveForce.x<=0)
            moveForce.x-=0.25;
        else
            moveForce.x+=0.25;
    }
    if(moveForce.y<-2) moveForce.y=-2;
    if(moveForce.y>2) moveForce.y=2;
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

void Actor::communication(std::vector<unsigned int>& inSightObjectsIds,std::vector<unsigned int>& actorsToCommunicateIds){
    using std::cout;
    using std::endl;
    while(!stop_thread){
        //cout<<"ACTOR ID: "<<idObject<<" reads"<<endl;
        std::vector<unsigned int> localInSightObjectsIds(inSightObjectsIds);
        m1.lock();
        std::pair<messageType,glm::vec2> message = communicationBridge->readMessage(idObject);
        m1.unlock();
        if(message.first==messageType::EXIT){
            //cout<<"Dostalem wiadomosc: "<<message.first<<", ["<<message.second.x<<"]["<<message.second.y<<"]"<<endl;

        }else{

        }


        while(!localInSightObjectsIds.empty())
        {
           // cout<<"  wysylam do: "<<inSightObjectsIds.at(0)<<endl;
           m1.lock();
            std::pair<messageType,glm::vec2> mes(messageType::EXIT,glm::vec2(idObject,idObject));
            communicationBridge->sendMessage(localInSightObjectsIds.at(0),mes);
            m1.unlock();
            localInSightObjectsIds.pop_back();
        }

        std::this_thread::sleep_for( std::chrono::seconds(1) );
    }
//    sleep(200);
}


