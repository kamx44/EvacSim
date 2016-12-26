#ifndef OBJECT_H
#define OBJECT_H

#include <math.h>
#include <set>
#include <glm/glm.hpp>
#include <glm/detail/type_mat2x2.hpp>
#include <Box2D/Box2D.h>



enum OBJECT_TYPE
{
    ACTOR,
    BOUNDARY,
    CURSOR,
    MODULE,
    SENSOR_COLLISION,
    SENSOR_SIGHT,
    SENSOR_COMMUNICATION,
    SENSOR_MOVE,
    SENSOR_OBSTACLE,
    WALL,
    WALL_EXIT
};



class Object
{
public:
    Object();
    virtual ~Object();

    static std::vector<int> idContainer;
    OBJECT_TYPE object_type;  //typ obiektu
    glm::vec2 position;             // position of the object
    glm::vec2 velocity;             // linear velocity

    b2CircleShape circle;           // circle shape
    b2BodyDef bodyDef;              // box2d body definition
    b2Body* body;                   // pointer to body in the world object
    b2PolygonShape bodyShape;       // shape of body
    b2FixtureDef fixtureDef;        // fixture definition
    b2Fixture* fixture;             // fixture of the body
    bool m_contacting;              // cheking if thers any contact
    float angularVelocity;          // predkosc katowa
    float torque;                   // moment obrotowy
    glm::vec2 force;                // force which moves object
    glm::mat2 orientation;          // orientacja w przestrzeni - wektory OBB
    glm::vec2 orientation2;
    float fOrientation;             // moze sie przydac  KAT
    int32 vertexCount;
    b2Vec2 *vertices;
    glm::vec2 normals[32];


    // Set by shape?
    float mass;                     // mass
    unsigned int   idObject;                 // id of the object
    bool  isAlive;
    bool  drawable;                  //is ready to draw

    virtual void startContact()
    {
        m_contacting = true;
    }
    virtual void endContact()
    {
        m_contacting = false;
    }
    OBJECT_TYPE getEntityType();
    virtual void draw() = 0;
    int getRandomId();
    void returnId(unsigned int id);
    unsigned int getId();
    void setOrientation(float angle);                             // z kata do wektora 2x2 ...
    int buildBlob(int numVertices, float radiusX, float radiusY);
    void setParameters(float dt);                       //set basic parameters on update
    virtual void update(float dt) = 0;          //every object should have itself a update function
    void setToWorld();
    void setFixtureToBody();
    void destroyBody();
    void ApplyImpulse(  glm::vec2& impulse,  glm::vec2& contactVector );  //dostanie impulsa
    virtual void delObject();


protected:
private:
};



#endif // OBJECT_H
