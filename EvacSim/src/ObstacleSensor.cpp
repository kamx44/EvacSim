#include <GL/gl.h>
#include <GL/glu.h>
#include "ObstacleSensor.h"
#include "Mmath.h"
#include <mutex>
#include <utility>


ObstacleSensor::ObstacleSensor(sensorType type,OBJECT_TYPE sensor_type,glm::vec2 position,float radius) :
    Sensor(type,sensor_type,position,radius)
{

    circle.m_radius = radius;
    fixtureDef.shape = &circle;
    fixtureDef.density = 0.000000001f;
    fixtureDef.friction = 0.000001f;
    fixtureDef.isSensor = false;

    if(object_type==OBJECT_TYPE::SENSOR_OBSTACLE && type==sensorType::OBSTACLE_EXIT){
        fixtureDef.filter.categoryBits = 0x0008;
        fixtureDef.filter.maskBits = 0x0020 | 0x0040;
    }else if(object_type==OBJECT_TYPE::SENSOR_OBSTACLE && type==sensorType::OBSTACLE_ACTOR){
        fixtureDef.filter.categoryBits = 0x0008;
        fixtureDef.filter.maskBits = 0x0010 | 0x0020;
    }

    drawable = true;
    radiusMovable = 1.0f;
    body = World::addToWorld(bodyDef);
    body->SetBullet(true);
    setToWorld();
    setFixtureToBody();
    body->GetMass();
}

ObstacleSensor::~ObstacleSensor()
{
}
void ObstacleSensor::draw()
{
        float x = position[0];
        float y = position[1];
        float angle = body->GetAngle();
        glPushMatrix ();
        glLineWidth (3.0f);
        glTranslatef(x,y,0);
        glRotatef( RADTODEG(angle) , 0, 0, 1 );
        glBegin(GL_LINE_LOOP);
       // glColor3f(0,0.5,0);//white
        if(type == sensorType::OBSTACLE_ACTOR){
            glColor3f(1,0,0);
        }else if(type == sensorType::OBSTACLE_EXIT){
            glColor3f(0,0,1);
        }
        for (int j=0; j < 360; j++)
        {
            float degInRad = DEGTORAD(j);
            glVertex2f(cos(degInRad)*radiusMovable,sin(degInRad)*radiusMovable);
        }
        glEnd();
        glPopMatrix ();
}

void ObstacleSensor::update(float dt)
{
    radiusMovable=radius;
    setParameters(dt);
}


void ObstacleSensor::sendObstacleSensor(b2Vec2 destination){
    b2Vec2 vel = multiplyB2Vec2(normalize(destination - body->GetPosition()),10);
    moveBody(vel);
}

void ObstacleSensor::moveBody(b2Vec2 velocity){
    body->ApplyLinearImpulse(velocity,body->GetWorldCenter(),true);

}

unsigned int ObstacleSensor::getWhereIWasSend(){
    return whereIWasSend;
}

void ObstacleSensor::setWhereIWasSend(unsigned int id){
    whereIWasSend = id;
}
