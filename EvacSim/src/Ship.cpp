#include "Ship.h"
#include "Renderer.h"
#include "Events.h"
#include "Mmath.h"
#include <Box2D/Box2D.h>

using namespace std;



Ship::Ship(World* world) :world_ship(world)
{
    object_type = OBJECT_TYPE::SHIP;
    idObject = getRandomId();
    isPlayer = true;
    ship_img = new Image("data/img/statek.png");
    mis_img = new Image("data/img/rakieta.png");
    //object_type = TEXTURE;
    hp=1000000;
    misID = mis_img->imageID;
    myID = ship_img->imageID;

    position[0] = 0;
    position[1] = 0;
    w=6.5;
    h=6.5;

    myID = ship_img->imageID;
    maxSpeed = 50;
    fOrientation = 0;
    mass = 0;

    //shape of ship
    vector<vector<b2Vec2*>> polCon = world->polygonGenerator.getPolygonContainer("statek.plist.xml");
    for(unsigned int i=0; i<polCon.size(); i++)
    {
        vector<b2Vec2*> vert2 = polCon[i];
        int32 mSize = vert2.size();
        b2Vec2* mVertices = new b2Vec2[mSize];
        for(int i=0; i<mSize; i++)
        {
            glm::vec2 v = createGlmVec((*vert2[i]));
            v -= glm::vec2(2.9,2.9);
            mVertices[i]=createB2vec2(v);
        }
        module = new Module(this,world_ship,glm::vec2(0,0),mVertices,mSize);
        modules.push_back(module);
        world_ship->addToObjectsContainer(module);
        mass+=module->body->GetMass();
        if(i==0)
        {
            vertexCount = mSize;
            vertices = new b2Vec2[vertexCount];
            vertices = mVertices;
            body = module->body;
        }
        else
            createJoint(module->body);
    }
}


void Ship::createJoint(b2Body* bodyB)
{

    //jointDef.collideConnected=false;
    //jointDef.Initialize(body, bodyB, body->GetWorldCenter());

    //jointWeld = (b2WeldJoint*)world_ship->world.CreateJoint( &jointDef );

}



void Ship::strzel_rakieta(KIND type)
{
/*    Missle* missle = new Missle(this,type,position,position+(orientation2*glm::vec2(20,20)));

    missle->type=type;
    missle->myID=misID;
    missle->setToWorld(world_ship->world);
    missle->setFixtureToBody();
    world_ship->addToObjectsContainer(missle);
*/
}



void Ship::calcSpeed()
{
    body->SetLinearVelocity(b2Vec2(0,0));
    glm::vec2 force(0);
    force += glm::vec2(0,2000)  * glm::vec2(Events::getInstance().keypadKeys[SDLK_w]);
    force -= glm::vec2(0,2000)  * glm::vec2(Events::getInstance().keypadKeys[SDLK_s]);
    force += glm::vec2(-2000,0)  * glm::vec2(Events::getInstance().keypadKeys[SDLK_a]);
    force += glm::vec2(2000,0)  * glm::vec2(Events::getInstance().keypadKeys[SDLK_d]);
    body->ApplyForce(createB2vec2(force), body->GetWorldCenter(), 1);
}

void Ship::update(float dt)
{
    setParameters(dt);
    calcSpeed();
}

void Ship::draw()
{
    float x = position[0];
    float y = position[1];
    float angle = fOrientation;
    int vc = vertexCount;
    glPushMatrix ();
    glLineWidth (3.0f);

    glTranslatef(x,y,0);

    glRotatef( RADTODEG(angle) , 0, 0, 1 );
    glBegin (GL_LINE_LOOP);
    for(int j=0; j<vc; j++)
    {
        x = vertices[j].x;
        y = vertices[j].y;
        //  glColor3f (0.7,1,1);
        if ( m_contacting )
            glColor3f(1,0,0);//red
        else
            glColor3f(1,1,1);//white
        glVertex3f (x,y,0);
    }
    glEnd();
    glPopMatrix ();
}


Ship::~Ship()
{
    //dtor
}
