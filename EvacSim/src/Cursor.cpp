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
    //size = 3;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(position[0], position[1]);
   // radius = 1.0f;
   // circle.m_radius = radius;
  //  fixtureDef.shape = &circle;
    vertexCount = 3;
    vertices = new b2Vec2[3];

   /* vertices[0].Set(-5,-5);
    vertices[1].Set(-5,5);
    vertices[2].Set(5,5);
    vertices[3].Set(5,-5); */
    //vertices[0].Set(-1,-1);
   // vertices[1].Set(0,1);
   // vertices[2].Set(1,-1);
    buildBlob(3,5,5);
    bodyShape.Set(vertices, 3);
    fixtureDef.shape = &bodyShape;
    fixtureDef.density = 0.5f;
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

    int mouse_x = Events::getInstance().mouseX;
    int mouse_y = Events::getInstance().mouseY;
//    float bodyAngle = body->GetAngle();
//    b2Vec2 toTarget = b2Vec2(mouse_x,mouse_y) - body->GetPosition();
//    float desiredAngle = atan2f( -toTarget.x, toTarget.y);
    float desiredAngle = atan2f( mouse_x, mouse_y)-M_PI;
    float bodyAngle = body->GetAngle();

    ///< calculating rotation
    //cout<<"desire: "<<desiredAngle<<" bodyAngle: "<<bodyAngle<<endl;
    body->SetAngularVelocity(0);
    float totalRotation = desiredAngle - bodyAngle;
    while ( totalRotation < DEGTORAD(-180) ) totalRotation += DEGTORAD(360) ;
    while ( totalRotation > DEGTORAD(180) ) totalRotation -= DEGTORAD(360) ;
    float change = DEGTORAD(1) ;
    float newAngle = bodyAngle + min( change, max(-change, totalRotation));

   // body->SetTransform( body->GetPosition(), newAngle );
   // bodyAngle = body->GetAngle();
   // setOrientation(body->GetAngle()+M_PI/2);
   // body->SetLinearVelocity(b2Vec2(0,0));
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
    //setParameters(dt);
}

void Cursor::draw()
{
    float x = position[0];
    float y = position[1];
    float angle = body->GetAngle();
    //float angle = fOrientation;
    glPushMatrix ();
    glLineWidth (3.0f);
    glTranslatef(x,y,0);
    glRotatef( RADTODEG(angle) , 0, 0, 1 );
    glBegin(GL_LINE_LOOP);
    if ( m_contacting )
        glColor3f(1,0,0);//red
    else
        glColor3f(1,1,1);//white

    for(int j=0; j<vertexCount; j++)
    {
        x = vertices[j].x;
        y = vertices[j].y;
        glVertex3f (x,y,0);

    }

    glEnd();
    glPopMatrix ();
}

b2Vec2 Cursor::getPosition(){
    return body->GetPosition();
}

Cursor::~Cursor()
{
    //dtor
}
