#include "Cursor.h"
#include "Renderer.h"
#include "Events.h"
#include "Mmath.h"
#include <Box2D/Box2D.h>

using namespace std;



Cursor::Cursor()
{
    object_type = OBJECT_TYPE::CURSOR;
    idObject = getRandomId();
    hp=1000000;
    position[0] = 0;
    position[1] = 0;
    maxSpeed = 50;
    fOrientation = 0;
    mass = 0;

    //shape of cursor
    size = 3;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(position[0], position[1]);
    radius = 1.0f;
    circle.m_radius = radius;
    fixtureDef.shape = &circle;
    fixtureDef.density = 0.2f;
    fixtureDef.friction = 0.3f;
    fixtureDef.filter.categoryBits = 0x0002;
    fixtureDef.filter.maskBits = 0x0004;

    body = World::addToWorld(bodyDef);
    setToWorld();
    setFixtureToBody();
    body->GetMass();
    drawable = true;
}



void Cursor::calcSpeed()
{
    body->SetLinearVelocity(b2Vec2(0,0));
    glm::vec2 force(0);
    force += glm::vec2(0,2000)  * glm::vec2(Events::getInstance().keypadKeys[SDLK_w]);
    force -= glm::vec2(0,2000)  * glm::vec2(Events::getInstance().keypadKeys[SDLK_s]);
    force += glm::vec2(-2000,0)  * glm::vec2(Events::getInstance().keypadKeys[SDLK_a]);
    force += glm::vec2(2000,0)  * glm::vec2(Events::getInstance().keypadKeys[SDLK_d]);
    body->ApplyForce(createB2vec2(force), body->GetWorldCenter(), 1);
}

void Cursor::update(float dt)
{
    setParameters(dt);
    calcSpeed();
}

void Cursor::draw()
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


Cursor::~Cursor()
{
    //dtor
}
