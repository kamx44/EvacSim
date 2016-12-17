#include <GL/gl.h>
#include <GL/glu.h>
#include <mutex>
#include <utility>
#include "Mmath.h"
#include "CollisionSensor.h"


CollisionSensor::CollisionSensor(sensorType type,OBJECT_TYPE sensor_type,glm::vec2 position,float radius) :
    Sensor(type,sensor_type,position,radius)
{

    circle.m_radius = radius;
    fixtureDef.shape = &circle;
    fixtureDef.density = 0.0f;
    fixtureDef.friction = 0.3f;
    fixtureDef.isSensor = false;

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

CollisionSensor::~CollisionSensor()
{
}
void CollisionSensor::draw()
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
        for (int j=0; j < 360; j++)
        {
            float degInRad = DEGTORAD(j);
            glVertex2f(cos(degInRad)*radiusMovable,sin(degInRad)*radiusMovable);
        }
        glEnd();
        glPopMatrix ();
}

void CollisionSensor::update(float dt)
{
    radiusMovable=radius;
    setParameters(dt);
}

void CollisionSensor::setParentVelocity(b2Vec2 pforce){
    (*pushForce) += normalize(pforce);
}

void CollisionSensor::setParentForce(b2Vec2* parentForce){
    pushForce = parentForce;
}
