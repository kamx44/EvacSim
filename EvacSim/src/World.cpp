#include "World.h"
#include "Object.h"
#include "Renderer.h"
#include "Camera.h"
#include "Events.h"
#include "PolygonGenerator.h"
#include "Asteroide.h"
#include "Boundary.h"
#include "Sector.h"
#include "Building.h"
#include <Box2D/Box2D.h>

using namespace std;

World::World()
{
    objectsContainer = new ObjectsContainer();
}

World::~World()
{
    //dtor
}

b2World World::world = b2World(b2Vec2(0.0f,0.0f));




int World::startGame()
{
    //PolygonGenerator pol;
    initializeWorld();
    playerAlive=true;
    Renderer::getInstance();
    Events::getInstance();
    Renderer::getInstance().world=this;
    Renderer::getInstance().setObjectsContainer(objectsContainer);
    world.SetContactListener(&ContactListenerInstance);

//adding objects

    Ship* statek = new Ship(this);
    // player1->playerShips.push_back(statek);
    // player1->mainShip = statek;
    objectsContainer->addObject(statek);
    Boundary* boundaries = new Boundary();
    objectsContainer->addObject(boundaries);
    //void (World::*addToObjectsContainerPointer)(Object*);
    //addToObjectsContainerPointer = &World::addToObjectsContainer;
    Building* building = new Building(objectsContainer);




    /*wall = new Wall(glm::vec2(-40,0),glm::vec2(40,0),this);
    addToObjectsContainer(wall); */
    createActors(60,building);

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

    for(int i=0; i<objectsContainer->getSize(); i++)
    {
        if(objectsContainer->getObjectByIndex(i)->isAlive==true)
            objectsContainer->getObjectByIndex(i)->update(Renderer::getInstance().ratio);
        else
            objectsContainer->deleteObject(objectsContainer->getObjectByIndex(i));
    }
}

b2Body* World::addToWorld(b2BodyDef& bodyDef){
    return world.CreateBody(&bodyDef); //return pointer to body
}

void World::createActors(int amount,Building* building){
    for(int i=0; i<amount; i++)
    {
        if(Sector* freeSector = building->getFreeSector()){
            Asteroide* ast = new Asteroide(freeSector);
            objectsContainer->addObject(ast);
        }
    }
}
