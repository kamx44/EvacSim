#ifndef WORLD_H
#define WORLD_H

#include <vector>

#include "ContactListener.h"
#include "PolygonGenerator.h"



class Object;


class World
{
public:
    World();
    virtual ~World();


    std::vector<Object*> objectsContainer;
    bool playerAlive;

    ContactListener ContactListenerInstance;
    PolygonGenerator polygonGenerator;

    void addToObjectsContainer(Object* object);
    void delFromObjectsContainer(Object* object);
    static b2Body* addToWorld(b2BodyDef& bodyDef);
    void moveAll();
    int startGame();

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
