#include <GL/gl.h>
#include <GL/glu.h>
#include "Wall.h"
#include "Mmath.h"
#include "stdio.h"


Wall::Wall(glm::vec2 startPosition,glm::vec2 endPosition, World* world) : world_wall(world)
{
    this->startPosition = startPosition;
    this->endPosition = endPosition;
    idObject = getRandomId();
    position = startPosition;
    velocity = glm::vec2(0,0);
    fOrientation = 33;
    vertexCount = 2;
    vertices = new b2Vec2[vertexCount];
    vertices[0].Set(startPosition.x, startPosition.y);
    vertices[1].Set(endPosition.x, endPosition.y);
    b2EdgeShape shape;
    shape.Set(vertices[0],vertices[1]);
    bodyDef.type = b2_staticBody;
    bodyDef.position.Set(position[0], position[1]);
    fixtureDef.shape = &shape;
    fixtureDef.density = 2.0f;
    fixtureDef.friction = 0.3f;
    body = World::addToWorld(bodyDef);
    setToWorld();
    setFixtureToBody();
    mass = body->GetMass();
    drawable = true;
}

Wall::~Wall()
{
    //dtor
}

void Wall::draw()
{
    float x = position[0];
    float y = position[1];
    glPushMatrix ();
    glLineWidth (5.0f);
    glTranslatef(x,y,0);
    glBegin (GL_LINES);
    glColor3b(255,30,30);
    glVertex3i (startPosition.x, startPosition.y,0);
    glVertex3i (endPosition.x, endPosition.y,0);
    glEnd();
    glPopMatrix ();
}

void Wall::update(float dt)
{

}
