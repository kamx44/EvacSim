#include <GL/gl.h>
#include <GL/glu.h>
#include "Boundary.h"
#include "Mmath.h"
#include "stdio.h"

Boundary::Boundary()
{
    idObject = getRandomId();
    object_type = OBJECT_TYPE::BOUNDARY;
    position = glm::vec2(0,0);
    velocity = glm::vec2(0,0);
    fOrientation = 33;
    vertexCount = 4;
    vertices = new b2Vec2[vertexCount];
    vertices[0].Set(-200.0f, -200.0f);
    vertices[1].Set(-200.0f, 200.0f);
    vertices[2].Set(200.0f, 200.0f);
    vertices[3].Set(200.0f, -200.0f);
    b2ChainShape chain;
    chain.CreateLoop(vertices, 4);
    bodyDef.type = b2_staticBody;
    bodyDef.position.Set(position[0], position[1]);
    fixtureDef.shape = &chain;
    fixtureDef.density = 2.0f;
    fixtureDef.friction = 0.3f;
    fixtureDef.filter.categoryBits = 0x0004;
    fixtureDef.filter.maskBits = 0x0002;
    body = World::addToWorld(bodyDef);
    setToWorld();
    setFixtureToBody();
    mass = body->GetMass();
    drawable = true;
}

Boundary::~Boundary()
{
    //dtor
}

void Boundary::draw()
{
    float x = position[0];
    float y = position[1];
    float angle = fOrientation;

    glPushMatrix ();
    glLineWidth (3.0f);
    glTranslatef(x,y,-3);
    glRotatef( RADTODEG(angle) , 0, 0, 1 );
    glBegin (GL_LINE_LOOP);
    glColor3f(GLbyte(0),GLbyte(192),GLbyte(32));
    glVertex3f(-200.0f, -200.0f,0);
    glVertex3f(-200.0f, 200.0f,0);
    glVertex3f(200.0f, 200.0f,0);
    glVertex3f(200.0f, -200.0f,0);

    glEnd();
    glPopMatrix ();
}

void Boundary::update(float )
{

}
