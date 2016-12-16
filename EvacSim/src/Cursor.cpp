#include "Cursor.h"
#include "Renderer.h"
#include "Events.h"
#include "Mmath.h"
#include <Box2D/Box2D.h>

using namespace std;



Cursor::Cursor()
{
    object_type = OBJECT_TYPE::CURSOR;
    idObject = 1;//getRandomId();
    position[0] = 0;
    position[1] = 0;
    fOrientation = 0;
    mass = 0;
    drawable = false;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(position[0], position[1]);
    radius = 1.0f;
    circle.m_radius = radius;
    fixtureDef.shape = &circle;
    vertexCount = 3;
    vertices = new b2Vec2[3];
    fixtureDef.density = 0.5f;
    fixtureDef.friction = 0.3f;
    fixtureDef.filter.categoryBits = 0x0002;
    fixtureDef.filter.maskBits = 0x0004;

    body = World::addToWorld(bodyDef);
    setToWorld();
    setFixtureToBody();
    body->GetMass();

}



void Cursor::calcSpeed()
{

    int mouse_x = Events::getInstance().mouseX;
    int mouse_y = Events::getInstance().mouseY;
    float desiredAngle = atan2f( mouse_x, mouse_y)-M_PI;
    float bodyAngle = body->GetAngle();

    body->SetAngularVelocity(0);
    float totalRotation = desiredAngle - bodyAngle;
    while ( totalRotation < DEGTORAD(-180) ) totalRotation += DEGTORAD(360) ;
    while ( totalRotation > DEGTORAD(180) ) totalRotation -= DEGTORAD(360) ;
    float change = DEGTORAD(1) ;
    float newAngle = bodyAngle + min( change, max(-change, totalRotation));

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
}

b2Vec2 Cursor::getPosition(){
    return body->GetPosition();
}

Cursor::~Cursor()
{
    //dtor
}
