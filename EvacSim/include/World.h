#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include "ContactListener.h"




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
    bool endOfSimulation;
    static b2World world;
    static b2Body* addToWorld(b2BodyDef& bodyDef);
    void moveAll();
    int startGame();
    void createAgents(int amount,Building* building);
    static void initializeWorld(){
        world.SetGravity(b2Vec2(0.0f,0.0f));
    }
protected:
private:


};

//

#endif // WORLD_H
