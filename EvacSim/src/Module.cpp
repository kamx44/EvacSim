#include <GL/gl.h>
#include <GL/glu.h>
#include "Module.h"
#include "Mmath.h"


Module::Module(Object* parent_,World* world_,glm::vec2 position_,b2Vec2* vertices_,int32 size_ )
    : world_module(world_),
      size(size_),
      parent(parent_)
{

    object_type = OBJECT_TYPE::MODULE;
    position=position_;
    vertices=vertices_;
    vertexCount = size;
    parentId = parent->idObject;

    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(position.x, position.y);

    bodyShape.Set(vertices,vertexCount);

    fixtureDef.shape = &bodyShape;
    fixtureDef.density = 0.5f;
    fixtureDef.friction = 0.3f;
    fOrientation = 0;

    mass = size;
    body = World::addToWorld(bodyDef);
    setToWorld();
    setFixtureToBody();
    //  body->Set
}


void Module::calcSpeed()
{
    //  fOrientation = body->GetAngle()-90;
    //  body->SetTransform(b2Vec2(0,0),fOrientation);

}

void Module::setDamage(float damage)
{
    parent->hp -= (int)damage;
    std::cout<<"Dosta³ hita : "<<(int)damage<<" zostalo: "<<parent->hp<<std::endl;
    if(parent->hp<=0)
    {
        parent->isAlive=false;
    }
}

void Module::draw()
{
    float x = position[0];
    float y = position[1];
    int vc = vertexCount;
    float angle = fOrientation;


    glPushMatrix ();
    glLineWidth (3.0f);
    glTranslatef(x,y,0);
    glRotatef( RADTODEG(angle) , 0, 0, 1 );
    glBegin (GL_LINE_LOOP);
    for(int j=0; j<vc; j++)
    {
        x = vertices[j].x;
        y = vertices[j].y;
        if ( m_contacting )
            glColor3f(1,0,0);//red
        else
            glColor3f(1,1,1);//white
        glVertex3f (x,y,0);
    }
    glEnd();

    glPopMatrix ();

}
void Module::update(float dt)
{
    setParameters(dt);
}

Module::~Module()
{
    //dtor
}
