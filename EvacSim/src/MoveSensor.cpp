#include <GL/gl.h>
#include <GL/glu.h>
#include "MoveSensor.h"
#include "Mmath.h"
#include <mutex>
#include <utility>

MoveSensor::MoveSensor(sensorType type,OBJECT_TYPE sensor_type,glm::vec2 position,float radius) :
    Sensor(type,sensor_type,position,radius)
{

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

MoveSensor::~MoveSensor()
{
}
void MoveSensor::draw()
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
        for (int j=0; j < vertexCount; j++)
        {
            glVertex2f(vertices[j].x,vertices[j].y);
        }
        glEnd();
        glPopMatrix ();
}

void MoveSensor::update(float dt)
{
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
    setParameters(dt);
}

void MoveSensor::setParentVelocity(b2Vec2 pforce){
    (*pushForce) += normalize(pforce);
}

void MoveSensor::setParentForce(b2Vec2* parentForce){
    pushForce = parentForce;
}
