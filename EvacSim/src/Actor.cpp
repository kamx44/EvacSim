#include <GL/gl.h>
#include <GL/glu.h>
#include "ObjectsContainer.h"
#include "Actor.h"
#include "Mmath.h"
#include "stdio.h"

Actor::Actor(Sector* sector,ObjectsContainer* objContainer)
{
    idObject = getRandomId();
    object_type = OBJECT_TYPE::ACTOR;
    position = glm::vec2(getRandomInt(sector->getLeftDownCornerX()+2,sector->getLeftDownCornerX()+8),
                         getRandomInt(sector->getRightUpCornerY()-8,sector->getRightUpCornerY()-2));
    size = 3;
    mass = 10;
    velocity = getRandomVec2(-4,4);
    fOrientation = 33;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(position[0], position[1]);
    radius = 1.0f;
    circle.m_radius = radius;
    fixtureDef.shape = &circle;
    fixtureDef.density = 200.0f;
    fixtureDef.friction = 0.3f;

   // fixtureDef.filter.categoryBits = 0x0002;
   // fixtureDef.filter.maskBits = 0x0004;
    body = World::addToWorld(bodyDef);
    setToWorld();
    setFixtureToBody();
    body->GetMass();
    drawable = true;
    communicateSensor = new Sensor(position,5.0f,sensorType::COMMUNICATE);
    objContainer->addObject(communicateSensor);
    createJoint(communicateSensor->getBody());
    sightSensor = new Sensor(position,15.0f,sensorType::SIGHT);
    objContainer->addObject(sightSensor);
    createJoint(sightSensor->getBody());
    //TESTOWA OTOCZKA

    /*    b2Vec2* mVertices = new b2Vec2[4];
          mVertices[0]=b2Vec2(-5.5f, 5.5f);
          mVertices[1]=b2Vec2(5.5f, 5.5f);
          mVertices[2]=b2Vec2(5.5f, -5.5f);
          mVertices[3]=b2Vec2(-5.5f, -5.5f);
          module = new Module(world_ship,glm::vec2(0,0),mVertices,4);
          modules.push_back(module);
          createJoint(module->body);
        //  for(int i=0; i<module->body->GetFixtureList()->size();i++){
        b2Filter f = module->body->GetFixtureList()->GetFilterData();
        f.categoryBits = 0x0001;
        f.maskBits = 0x0002;
        f.groupIndex = 1;
        module->body->GetFixtureList()->SetFilterData(f); //.get(i).setSensor(true);
        module->fixtureDef.isSensor = true;
        module->fixtureDef.density = 0.01;
        module->body->CreateFixture(&module->fixtureDef);*/
}


Actor::~Actor()
{

}

void Actor::draw()
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

void Actor::update(float dt)
{
    setParameters(dt);
}

void Actor::createJoint(b2Body* bodyB)
{
    //jointDef.collideConnected=false;
    jointDef.Initialize(body, bodyB, body->GetWorldCenter());
    jointWeld = (b2WeldJoint*)World::world.CreateJoint( &jointDef );
}
