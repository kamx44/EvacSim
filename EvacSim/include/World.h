#ifndef WORLD_H
#define WORLD_H

#include <vector>

#include "ContactListener.h"
#include "PolygonGenerator.h"
//#include "ObjectsContainer.h"



class Object;
class ObjectsContainer;
class Building;


class World
{
public:
    World();
    virtual ~World();
    ObjectsContainer* objectsContainer;
    ContactListener ContactListenerInstance;
    PolygonGenerator polygonGenerator;
    bool playerAlive;
    static b2Body* addToWorld(b2BodyDef& bodyDef);
    void moveAll();
    int startGame();
    void createActors(int amount,Building* building);

   // b2Vec2 gravity;
    static void initializeWorld(){
        world.SetGravity(b2Vec2(0.0f,0.0f));
    }
    static b2World world;
protected:
private:
    //static b2Vec2 gravity();

};

//

#endif // WORLD_H
