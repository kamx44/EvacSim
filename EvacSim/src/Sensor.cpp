#include <GL/gl.h>
#include <GL/glu.h>
#include "Sensor.h"
#include "Mmath.h"
#include "World.h"
#include "Actor.h"
#include <mutex>

std::mutex m1;
Sensor::Sensor(OBJECT_TYPE sensor_type,glm::vec2 position,float radius,unsigned int idParent,b2Vec2* parentForce,std::vector<unsigned int>& visibleObjects,sensorType type)
{
    idObject = getRandomId();
    object_type = sensor_type;
    pushForce = parentForce;
    std::cout<<pushForce->x<<std::endl;
    std::cout<<pushForce->y<<std::endl;
    this->type = type;
    this->position = position;
    size = 3;
    mass = 0;
    velocity = glm::vec2(0,0);
    fOrientation = 0;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(position[0], position[1]);
    this->radius = radius;
    //float radius = 8;
    if(object_type == OBJECT_TYPE::SENSOR_MOVE){
        vertexCount = 8;
        vertices = new b2Vec2[8];
        vertices[0].Set(0,0);
        int mainAngle=90;
        int j=-3;
        for (int i = 0; i < 7; i++,j++) {
            if(i>=4) mainAngle=90;
            float angle = DEGTORAD(j / 6.0 * mainAngle);// * DEGTORAD;
            vertices[i+1].Set( radius * cosf(angle), radius * sinf(angle) );
        }
        bodyShape.Set(vertices, 8);
        fixtureDef.shape = &bodyShape;
    }else{
    //tower->m_body->CreateFixture(&myFixtureDef);
        circle.m_radius = radius;
        fixtureDef.shape = &circle;
    }
    fixtureDef.density = 0.0f;
    fixtureDef.friction = 0.3f;
    if(object_type==OBJECT_TYPE::SENSOR_COLLISION)
        fixtureDef.isSensor = false;
    else
        fixtureDef.isSensor = true;
    body = World::addToWorld(bodyDef);
    setToWorld();
    setFixtureToBody();
    body->GetMass();
    drawable = true;
    this->idParent = idParent;
    this->visibleObjects = &visibleObjects;
    radiusMovable = 1.0f;
    rotateBody=true;
}

Sensor::~Sensor()
{
    //dtor
}
void Sensor::draw()
{
        float x = position[0];
        float y = position[1];
        float angle = body->GetAngle();
        glPushMatrix ();
        glLineWidth (3.0f);
        glTranslatef(x,y,0);
        glRotatef( RADTODEG(angle) , 0, 0, 1 );
        glBegin(GL_LINE_LOOP);
        if ( m_contacting )
            glColor3f(1,0,0);//red
        else
            glColor3f(0,0.5,0);//white
        if(object_type == OBJECT_TYPE::SENSOR_MOVE){
            for (int j=0; j < vertexCount; j++)
            {
                glVertex2f(vertices[j].x,vertices[j].y);
            }
        }
        else{
            for (int j=0; j < 360; j++)
            {
                float degInRad = DEGTORAD(j);
                glVertex2f(cos(degInRad)*radiusMovable,sin(degInRad)*radiusMovable);
            }
        }
        glEnd();
        glPopMatrix ();
}

void Sensor::update(float dt)
{
    if(object_type==OBJECT_TYPE::SENSOR_SIGHT){
        if(radiusMovable<radius)
            radiusMovable += 0.5f;
        else if(radiusMovable>=radius)
            radiusMovable = 1.0f;
        body->DestroyFixture(fixture);
        circle.m_radius = radiusMovable;
        fixtureDef.shape = &circle;
        fixtureDef.density = 0.0f;
        fixtureDef.friction = 0.3f;
        fixtureDef.isSensor = true;
        setFixtureToBody();
    }else{
        radiusMovable=radius;

    }

    setParameters(dt);

}

b2Body* Sensor::getBody(){
    return body;
}

unsigned int Sensor::getParentId(){
    return idParent;
}

void Sensor::addVisibleObject(unsigned int visibleObjectId){
    bool exist = false;
    for(unsigned int i=0; i<visibleObjects->size(); i++){
        if(visibleObjects->at(i) == visibleObjectId){
            exist = true;
            break;
        }
    }
    if(!exist){
        m1.lock();
        visibleObjects->push_back(visibleObjectId);
        m1.unlock();
    }
}

void Sensor::removeInvisibleObject(unsigned int visibleObjectId){
    m1.lock();
    for(unsigned int i=0; i<visibleObjects->size(); i++)
    {
        if(visibleObjectId==visibleObjects->at(i))
        {
                visibleObjects->erase(visibleObjects->begin()+i);
                //m1.unlock();
                break;
        }
    }
    m1.unlock();
}

void Sensor::printData(){
    using std::cout;
    using std::endl;
    cout<<"Parent ID: "<<idParent<<endl;
    for(unsigned int i=0; i<visibleObjects->size(); i++)
    {
        cout<<"  ID["<<i<<"]: "<<visibleObjects->at(i)<<endl;
    }
}

void Sensor::setParentVelocity(b2Vec2 pforce){
    //using std::cout;
    //using std::endl;
    std::cout<<pushForce->x<<std::endl;
    std::cout<<pushForce->y<<std::endl;
   // cout<<"stara sila: x:"<<pushForce->x<<" y:"<<pushForce->y<<endl;
    std::cout<<"nowa sila: x:"<<pforce.x<<" y:"<<pforce.y<<std::endl;
    (*pushForce) += pforce;
}

void Sensor::setDrawable(bool draw){
    drawable = draw;
}
