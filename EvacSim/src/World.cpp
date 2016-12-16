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
#include <typeinfo>

using namespace std;
extern std::mutex m1;

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
    endOfSimulation=false;
    Renderer::getInstance();
    Events::getInstance();
    Renderer::getInstance().world=this;
    Renderer::getInstance().setObjectsContainer(objectsContainer);
    world.SetContactListener(&ContactListenerInstance);

//adding objects

    Cursor* cursor = new Cursor();
    objectsContainer->addObject(cursor);
    Boundary* boundaries = new Boundary();
    objectsContainer->addObject(boundaries);
    Building* building = new Building(objectsContainer);
    building->createRooms();
    CommunicationBridge* communicationBridge = new CommunicationBridge();
    for(int i=0; i<4; i++)
    {
        if(Sector* freeSector = building->getFreeSector()){
            Actor* actor = new Actor(freeSector,objectsContainer,communicationBridge,cursor);
            objectsContainer->addObject(actor);
            actor->Start();
        }
    }


    int32 velocityIterations = 6;
    int32 positionIterations = 2;
    try
    {
        while(!endOfSimulation)
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
    bool found = false;
    //m1.lock();
    for(auto it : objectsContainer->getContainer()){
        //cout<<"TYPE: "<<typeid(*it.second).name()<<"  ";
        if(it.second->object_type == OBJECT_TYPE::ACTOR)
            found = true;
        if(it.second->isAlive==true)
            it.second->update(Renderer::getInstance().ratio);
        else{

            objectsContainer->deleteObject(it.second);
        }
    }
   // m1.unlock();
    if(!found)
        endOfSimulation = true;
}

b2Body* World::addToWorld(b2BodyDef& bodyDef){
    b2Body* bod = world.CreateBody(&bodyDef);
    return bod; //return pointer to body
}

void World::createActors(int amount,Building* building){

}
