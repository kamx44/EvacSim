#include "Missle.h"

#include "Mmath.h"
#include "Renderer.h"
#include <Box2D/Box2D.h>
Missle::Missle(Ship* ship)
{
    ttl = 0.2f;
}

Missle::Missle(Ship* ship,KIND kind, glm::vec2 startPos, glm::vec2 endPos)
{
    idObject = getRandomId();
    hp=3;
    state = 1;    //1-live 2-dead
    this->ship=ship;
    type = kind;
    position = startPos;
    ttl = 0.2f;
    positionTarget = endPos;
    // vertexCount = 4;
//   vertices = new b2Vec2[vertexCount];
    bodyDef.type = b2_dynamicBody;
    bodyDef.bullet = true;
    // bodyDef.angularDamping = 10.0f;
    bodyDef.fixedRotation = true;

    bodyDef.position.Set(startPos.x+(ship->orientation2.x*5),startPos.y+(ship->orientation2.y*5));//(startPos.x-endPos.x,startPos.y-endPos.y);

    if(type==LASER)
    {
        circle.m_radius = 0.1f;
        radius = 0.1f;
        fixtureDef.shape = &circle;
        fixtureDef.density = 0.5f;
        fixtureDef.friction = 0.3f;
        velocity = (endPos-startPos)*glm::vec2(10,10);
    }
    else if(type==BULLET)
    {
        circle.m_radius = 0.1f;
        radius = 0.1f;
        fixtureDef.shape = &circle;
        fixtureDef.density = 0.8f;
        fixtureDef.friction = 0.3f;
        velocity = (endPos-startPos)*glm::vec2(8,8);
    }
    else if(type==PLASMA)
    {
        circle.m_radius = 1.0f;
        radius = 1.0f;
        fixtureDef.shape = &circle;
        fixtureDef.density = 1.0f;
        fixtureDef.friction = 0.3f;
        velocity = (endPos-startPos)*glm::vec2(5,5);
    }






    /*
    vertices[0] = createB2vec2(startPos);
    vertices[1] = createB2vec2(endPos);
    vertices[2] = createB2vec2(startPos+glm::vec2(0.5,0.5));
    vertices[3] = createB2vec2(endPos+glm::vec2(0.5,0.5));
    bodyShape.Set(vertices,vertexCount); */
}

Missle::~Missle()
{

}

int Missle::move()
{

    // setOrientation(fOrientation);
    //  b2Vec2 vel = body->GetLinearVelocity();
    b2Vec2 vel((float32)(velocity.x),(float32)(velocity.y));
    //  b2Vec2 velShip((float32)(ship->velocity.x),(float32)(ship->velocity.y));
    //  cout<<vel.x<<" "<<vel.y<<endl;
    body->SetLinearVelocity(vel+ship->body->GetLinearVelocity());
    //   cout<<"vel: "<<vel.x<<" "<<vel.y<<endl;
    position = createGlmVec(body->GetPosition());
//   fOrientation = (body->GetAngle());

    positionStart=ship->position;
    positionTarget=ship->position+(ship->orientation2*glm::vec2(20,20));
    ttl-= Renderer::getInstance().ratio;
//   printf("%f \n",ttl);
    if (type == LASER)
    {
        if (ttl < 0.1)
        {
            state=2;
            return 2;
        }
    }
    else if(type == BULLET||type==PLASMA||type==ROCKET)
    {
//        cout<<ttl<<endl;
        if (ttl < -1)
        {
            state=2;
            return 2;
        }
    }

    //  state=1;
    return 1;
}

void Missle::update(float dt)
{
    move();

    if(state==2 )
    {
        ship->world_ship->delFromObjectsContainer(this);
    }
    else if(state==3)
    {
        ttl2 -= Renderer::getInstance().ratio;
        if(ttl2<-0.051)
        {
            ship->world_ship->delFromObjectsContainer(this);
        }
    }
}

void Missle::startContact()
{
    //set time calculating time of collision
    ttl2=0; //= Renderer::getInstance().ratio;
    m_contacting = true;
    state=3;
}

void Missle::endContact()
{
    m_contacting = false;
    // state=2;
}

void Missle::draw()
{
    if(type == LASER)
    {
        float x = position[0];
        float y = position[1];
        glPushMatrix ();
        glLineWidth (10.0f);
        glBegin (GL_LINES);
        glColor3f (1,0,0);
        glVertex3f (positionStart.x,positionStart.y,0);
        glColor3f (0,0,1);
        glVertex3f (x,y,0);
        glEnd();
        glPopMatrix ();

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
    else if(type == BULLET)
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
    else if(type == PLASMA)
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
    else if(type == ROCKET)
    {
        Renderer::getInstance().DrawQuadTexture(2.0f,2.0f,5.0f,5.0f,myID);
    }


}
