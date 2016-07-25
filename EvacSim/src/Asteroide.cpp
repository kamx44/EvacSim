#include <GL/gl.h>
#include <GL/glu.h>
#include "Asteroide.h"
#include "Mmath.h"
#include "stdio.h"

Asteroide::Asteroide(World* world) :world_asteroide(world)
{
    idObject = getRandomId();
    object_type = OBJECT_TYPE::ASTEROIDE;
    position = getRandomVec2(-10, 10);
    size = 3;
    mass = 10;
    velocity = getRandomVec2(0,1);
    fOrientation = 33;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(position[0], position[1]);
    radius = 1.0f;
    circle.m_radius = radius;
    fixtureDef.shape = &circle;
    fixtureDef.density = 200.0f;
    fixtureDef.friction = 0.3f;
   // circle.m_p.Set(position[0], position[1]);
    body = World::addToWorld(bodyDef);
    setToWorld();
    setFixtureToBody();
    body->GetMass();
    drawable = true;
}


Asteroide::~Asteroide()
{

}

void Asteroide::draw()
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

void Asteroide::update(float dt)
{
    setParameters(dt);

    b2Vec2 pos = world_asteroide->objectsContainer.at(0)->body->GetPosition();

}
