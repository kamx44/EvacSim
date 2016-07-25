#include <GL/gl.h>
#include <GL/glu.h>
#include "Boundary.h"
#include "Mmath.h"
#include "stdio.h"

Boundary::Boundary(World* world) : world_boundary(world)
{
    idObject = getRandomId();
    //object_type = OBJECT_TYPE::ASTEROIDE;
    position = glm::vec2(0,0);
    velocity = glm::vec2(0,0);
    fOrientation = 33;
    vertexCount = 4;
    vertices = new b2Vec2[vertexCount];
    vertices[0].Set(-100.0f, -100.0f);
    vertices[1].Set(-100.0f, 100.0f);
    vertices[2].Set(100.0f, 100.0f);
    vertices[3].Set(100.0f, -100.4f);
    b2ChainShape chain;
    chain.CreateLoop(vertices, 4);
    bodyDef.type = b2_staticBody;
    bodyDef.position.Set(position[0], position[1]);
    fixtureDef.shape = &chain;
    fixtureDef.density = 2.0f;
    fixtureDef.friction = 0.3f;
   // circle.m_p.Set(position[0], position[1]);
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
    int vc = vertexCount;
    float angle = fOrientation;

    // tego tu nie ma byæ morf
    //     world->asteroides[i]->move();


    glPushMatrix ();
    glLineWidth (3.0f);
    glTranslatef(x,y,-3);
    glRotatef( RADTODEG(angle) , 0, 0, 1 );
    glBegin (GL_POLYGON);
    glColor3b(0,192,32);
    glVertex3i (-100.0f, -100.0f,0);
    glVertex3i (-100.0f, 100.0f,0);
    glVertex3i (100.0f, 100.0f,0);
    glVertex3i (100.0f, -100.0f,0);
  /*  for(int j=0; j<vc; j++)
    {
        x = vertices[j].x;
        y = vertices[j].y;
       // if ( m_contacting )
       //     glColor3f(1,0,0);//red
       // else
            glColor3f(1,1,1);//white
        glVertex3f (x,y,0);
    } */
    glEnd();

    glPopMatrix ();
}

void Boundary::update(float dt)
{

}
