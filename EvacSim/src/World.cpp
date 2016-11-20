#include "World.h"
#include "Object.h"
#include "Renderer.h"
#include "Camera.h"
#include "Events.h"
#include "PolygonGenerator.h"
#include "Actor.h"
#include "Boundary.h"
#include "Sector.h"
#include "Building.h"
#include "CommunicationBridge.h"
#include <Box2D/Box2D.h>
#include <thread>

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

    Cursor* cursor = new Cursor();
    // player1->playerCursors.push_back(cursor);
    // player1->mainCursor = cursor;
    objectsContainer->addObject(cursor);

    Boundary* boundaries = new Boundary();
    objectsContainer->addObject(boundaries);
    //void (World::*addToObjectsContainerPointer)(Object*);
    //addToObjectsContainerPointer = &World::addToObjectsContainer;
    Building* building = new Building(objectsContainer);
    building->createRooms();
    //createActors(3,building);

    CommunicationBridge* communicationBridge = new CommunicationBridge();
    for(int i=0; i<10; i++)
    {
        if(Sector* freeSector = building->getFreeSector()){
            Actor* actor = new Actor(freeSector,objectsContainer,communicationBridge,cursor);
            objectsContainer->addObject(actor);
            //std::thread functorTest(&Actor::run,*ast);
            //std::thread tescik(Actor::testThread,44);
            actor->Start();
        }
    }
    //world.GetBodyList()


    /*wall = new Wall(glm::vec2(-40,0),glm::vec2(40,0),this);
    addToObjectsContainer(wall); */


    int32 velocityIterations = 6;
    int32 positionIterations = 2;

    try
    {
        //int i=0;
        while(playerAlive)
        {
            //cout<<"STEP: "<<i<<endl;
            world.Step(Renderer::getInstance().ratio, velocityIterations, positionIterations);
            moveAll();
            Renderer::getInstance().drawAll();
            if(!Events::getInstance().checkEvents()) return 0;
            //   Camera::getInstance().calculateCameraMovement();
            //   Camera::getInstance().moveCamera();

          //  if (functorTest.joinable())
          //      functorTest.join();
          //i++;
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
    b2Body* bod = world.CreateBody(&bodyDef);
    return bod; //return pointer to body
}

void World::createActors(int amount,Building* building){
 /*   for(int i=0; i<amount; i++)
    {
        if(Sector* freeSector = building->getFreeSector()){
            Actor* ast = new Actor(freeSector,objectsContainer);
            objectsContainer->addObject(ast);
            thread functorTest;
            thread functorTest(&Actor::run,*ast);

        }
    } */
}
