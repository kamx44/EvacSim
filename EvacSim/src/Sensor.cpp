#include <GL/gl.h>
#include <GL/glu.h>
#include "Sensor.h"
#include "Mmath.h"
#include "World.h"

Sensor::Sensor(glm::vec2 position,float radius,sensorType type)
{
    idObject = getRandomId();
    object_type = OBJECT_TYPE::SENSOR;
    this->type = type;
    this->position = position;
    size = 3;
    mass = 0;
    velocity = glm::vec2(0,0);
    fOrientation = 33;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(position[0], position[1]);
    this->radius = radius;
    circle.m_radius = radius;
    fixtureDef.shape = &circle;
    fixtureDef.density = 0.0f;
    fixtureDef.friction = 0.3f;
    fixtureDef.isSensor = true;
    body = World::addToWorld(bodyDef);
    setToWorld();
    setFixtureToBody();
    body->GetMass();
    drawable = true;
}

Sensor::~Sensor()
{
    //dtor
}
void Sensor::draw()
{
        float x = position[0];
        float y = position[1];
        glPushMatrix ();
        glLineWidth (3.0f);
        glTranslatef(x,y,0);
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

void Sensor::update(float dt)
{
    setParameters(dt);
}

b2Body* Sensor::getBody(){
    return body;
}
