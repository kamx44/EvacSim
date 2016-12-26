#include "Object.h"
#include "Mmath.h"
#include "Renderer.h"
#include "Events.h"


#include <Box2D/Box2D.h>

/********************************************//**
 * \brief This function transform glm::vec2 to glm::vec3 (z variable is zero)
 *
 * \param glm::vec2 vector from glm library
 * \return glm::vec3 vector from glm library
 * #kamx
 ***********************************************/
glm::vec3 tran2to3(glm::vec2 wek)
{
    glm::vec3 pom;
    pom.x = wek.x;
    pom.y = wek.y;
    pom.z = 0;
    return pom;
}

/********************************************//**
 * \brief This function transform glm::vec3 to glm::vec2  (z variable is deleted)
 *
 * \param glm::vec3 vector from glm library
 * \return glm::vec2 vector from glm library
 * #kamx
 ***********************************************/
glm::vec2 tran3to2(glm::vec3 wek)
{
    glm::vec2 pom;
    pom.x = wek.x;
    pom.y = wek.y;

    return pom;
}

/********************************************//**
 * \brief Object constructor
 *
 * \param no param
 * \return
 * #kamx
 ***********************************************/
Object::Object()
{

    m_contacting = false;
    isAlive = true;
    drawable = true;
}

/********************************************//**
 * \brief Object destructor
 *
 * \return
 * #kamx
 ***********************************************/
Object::~Object()
{
}

/********************************************//**
 * \brief This function is setting body to world and setting default velocity
 *
 * \param b2World &world reference to world object
 * \return void
 * #kamx
 ***********************************************/
void Object::setToWorld()
{
    b2Vec2 vel((float32)(velocity.x),(float32)(velocity.y));
    body->SetLinearVelocity(vel);
    body->SetUserData( this );
    body->SetAngularDamping(0.0f);
}

/********************************************//**
 * \brief This function is setting fixture to body
 *
 * \param no param
 * \return void
 * #kamx
 ***********************************************/
void Object::setFixtureToBody()
{
    fixture = body->CreateFixture(&fixtureDef);
}


/********************************************//**
 * \brief This function is deleting body from world
 *
 * \param b2World &world reference to world object
 * \return void
 * #kamx
 ***********************************************/
void Object::destroyBody()
{
    World::world.DestroyBody(body);
    body=nullptr;
}

/********************************************//**
 * \brief This function is setting the rotation of object
 *
 * \param float angle
 * \return void
 * #kamx
 ***********************************************/
void Object::setOrientation(float angle)
{
    float c = cos( angle );
    float s = sin( angle );

    orientation[0][0] = c;
    orientation[0][1] = -s;
    orientation[1][0] = s;
    orientation[1][0] =  c;

    orientation2[0] = c;
    orientation2[1] = s;
}

/********************************************//**
 * \brief This function is creating polygon from vertices
 *
 * \param int32 numVertices number of vertices
 * \param float radiusX
 * \param float radiusY
 * \return int (it must be changed)
 * #kamx
 ***********************************************/
int Object::buildBlob(int32 numVertices, float radiusX, float radiusY)
{
    if (numVertices == 0)
        return 1;

    if (numVertices == 1)
    {
//		vertices[0] = glm::vec2(0);
        return 0;
    }
    if (numVertices > 32)
    {
        // morf - wyj¹tek dodac
        return 1;
    }

    float a = M_PI / numVertices;
    float da = 2.0f * M_PI / numVertices;

    for(int i = 0; i < numVertices; i ++)
    {
        a += da;
        vertices[i].Set(cos(a) * radiusX, sin(a) * radiusY);
    }
    return 0;
}

/********************************************//**
 * \brief This function is moving object in world setting orientation and position
 *
 * \return void
 * #kamx
 ***********************************************/
void Object::setParameters(float dt)
{
    //-------------------------------------------------------
    // Integrate position (verlet integration)
    //-------------------------------------------------------
    setOrientation(fOrientation);
    position = createGlmVec(body->GetPosition());
    fOrientation = (body->GetAngle());
    //-------------------------------------------------------
    // Integrate velocity (implicit linear velocity)
    //-------------------------------------------------------
//    velocity		+= force  * (inverseMass    * dt);
    //angularVelocity	+= torque * (inverseInertia * dt);

    //-------------------------------------------------------
    // clear forces
    //-------------------------------------------------------
    force		 = glm::vec2(0.0f, 0.0f);
    torque	 = 0.0f;
}



/********************************************//**
 * \brief This function randomize id of the object
 *
 * \return id
 *
 ***********************************************/
int Object::getRandomId()
{
    /*int currentSize = idContainer.size();
    int i = 0;
    while (idContainer.size()<=currentSize)
    {
        i = (rand() % 100000)+2;
        idContainer.insert(i);
    }
    return i;*/
    unsigned int number = 0;
    if (!idContainer.empty()){
        number = idContainer.at(idContainer.size()-1);
        idContainer.pop_back();
    }
    if(number == 0)
        throw "Container full";
    else{
        //std::cout<<number<<std::endl;
        return number;
    }
}

void Object::returnId(unsigned int id){
    idContainer.push_back(id);
}
/*
int i = (rand() % 100000)+2;
    std::pair<std::set<int>::iterator,bool> ret;
    ret = idContainer.insert(i);
    while (ret.second==false)
    {
        i = (rand() % 100000)+2;
        ret = idContainer.insert(i);
    }
*/

/********************************************//**
 * \brief Initialization of static container idContainer
 *
 * \return Container set
 *
 ***********************************************/
std::vector<int> init()
{
    std::vector<int> tmp;
    for(unsigned int i = 1;i<65000;i++){
        tmp.push_back(i);
    }
    //tmp.insert(0);
    return tmp;
}
std::vector<int> Object::idContainer(init());

void Object::delObject()
{
    std::cout<<"delete object"<<std::endl;
}



OBJECT_TYPE Object::getEntityType()
{
    return object_type;
}

unsigned int Object::getId(){
    if(idObject){
        return idObject;
    }else{
        throw "ERROR: get the null id object";
    }
}
