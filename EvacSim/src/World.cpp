#include "World.h"
#include "Object.h"
#include "Renderer.h"
#include "Camera.h"
#include "Events.h"
#include "PolygonGenerator.h"
#include "Asteroide.h"
#include "Boundary.h"
#include "Wall.h"
#include <Box2D/Box2D.h>

using namespace std;

World::World()
{

}

World::~World()
{
    //dtor
}

b2World World::world = b2World(b2Vec2(0.0f,0.0f));

void World::addToObjectsContainer(Object *object)
{
    objectsContainer.push_back(object);
}


void World::delFromObjectsContainer(Object *object)
{
    for(unsigned int i=0; i<objectsContainer.size(); i++)
    {
        if(object==objectsContainer[i])
        {
            if(object->isPlayer==true)
            {
                object->destroyBody();
                objectsContainer.clear();
                object->drawable=false;
                delete object;
                playerAlive=false;
            }
            else
            {
                object->destroyBody();
                objectsContainer.erase(objectsContainer.begin()+i);
                object->drawable=false;
                delete object;
            }
        }
    }
}


int World::startGame()
{
    //PolygonGenerator pol;
    initializeWorld();
    playerAlive=true;
    Renderer::getInstance();
    Events::getInstance();
    Renderer::getInstance().world=this;
    Renderer::getInstance().setObjectsContainer(&objectsContainer);
    world.SetContactListener(&ContactListenerInstance);

//adding objects

    Ship* statek = new Ship(this);
    // player1->playerShips.push_back(statek);
    // player1->mainShip = statek;
    addToObjectsContainer(statek);
    Boundary* boundaries = new Boundary(this);
  //  addToObjectsContainer(boundaries);
    Wall* wall = new Wall(glm::vec2(-40,0),glm::vec2(40,0),this);
    addToObjectsContainer(wall);
    for(int i=0; i<20; i++)
    {
        Asteroide* ast = new Asteroide(this);
        addToObjectsContainer(ast);
    }
    int32 velocityIterations = 6;
    int32 positionIterations = 2;

    try
    {
        while(playerAlive)
        {
            world.Step(Renderer::getInstance().ratio, velocityIterations, positionIterations);
            moveAll();
            Renderer::getInstance().drawAll();
            if(!Events::getInstance().checkEvents()) return 0;
            //   Camera::getInstance().calculateCameraMovement();
            //   Camera::getInstance().moveCamera();
        }
        cout<<"KONIEC GRY"<<endl;
    }
    catch(string& e)
    {
        std::cout<<e<<std::endl;
    }
    return 1;
}

void World::moveAll()
{

    for(int i=0; i<objectsContainer.size(); i++)
    {
        if(objectsContainer.at(i)->isAlive==true)
            objectsContainer.at(i)->update(Renderer::getInstance().ratio);
        else
            delFromObjectsContainer(objectsContainer.at(i));
    }
}

b2Body* World::addToWorld(b2BodyDef& bodyDef){
    return world.CreateBody(&bodyDef); //return pointer to body
}
