
#include "Mmath.h"
#include "ContactListener.h"
#include "Cursor.h"
#include "Agent.h"
#include "Exit.h"
#include "Object.h"



ContactListener::ContactListener()
{
    bContact = false;
}

ContactListener::~ContactListener()
{
    //dtor
}

void ContactListener::BeginContact(b2Contact* contact)
{
    bContact = true;
    b2WorldManifold worldManifold;
    contact->GetWorldManifold( &worldManifold );
    Object* bodyUserDataA = (Object*)contact->GetFixtureA()->GetBody()->GetUserData();
    Object* bodyUserDataB = (Object*)contact->GetFixtureB()->GetBody()->GetUserData();
   if(bodyUserDataA->getEntityType()==OBJECT_TYPE::ACTOR && bodyUserDataB->getEntityType()==OBJECT_TYPE::SENSOR_COMMUNICATION)
    {
        Agent* actor = dynamic_cast<Agent*>( bodyUserDataA );
        Sensor* sensor = dynamic_cast<Sensor*>( bodyUserDataB );
        sensor->addObjectToCheckAgentContainer(actor->getId(),actor->body->GetPosition());
    }
    else if(bodyUserDataA->getEntityType()==OBJECT_TYPE::WALL_EXIT && bodyUserDataB->getEntityType()==OBJECT_TYPE::SENSOR_SIGHT)
    {
        Exit* exit = dynamic_cast<Exit*>( bodyUserDataA );
        Sensor* sensor = dynamic_cast<Sensor*>( bodyUserDataB );
        sensor->addObjectToCheckExitContainer(exit->getId(),exit->getMiddlePoint());
    }
    else if(bodyUserDataA->getEntityType()==OBJECT_TYPE::WALL && bodyUserDataB->getEntityType()==OBJECT_TYPE::SENSOR_MOVE)
    {
        b2Vec2 pos1 = worldManifold.points[0];
        b2Vec2 pos2 = dynamic_cast<Sensor*>(bodyUserDataB)->getParentSensor()->body->GetPosition();
        b2Vec2 impactVelocity = pos1 - pos2;
        impactVelocity.x *= 100;//(std::rand()%100);
        impactVelocity.y  *= 100;
        Sensor* sensor = dynamic_cast<Sensor*>( bodyUserDataB );
        sensor->setParentVelocity(impactVelocity);
    }
    else if(bodyUserDataA->getEntityType()==OBJECT_TYPE::WALL && bodyUserDataB->getEntityType()==OBJECT_TYPE::SENSOR_COLLISION){
        b2Vec2 pos1 = worldManifold.points[0];
        b2Vec2 pos2 = dynamic_cast<Sensor*>(bodyUserDataB)->getParentSensor()->body->GetPosition();
        b2Vec2 impactVelocity = pos1 - pos2;
        impactVelocity.x *= 100;//(std::rand()%100);
        impactVelocity.y  *= 100;
        Sensor* sensor = dynamic_cast<Sensor*>( bodyUserDataB );
        sensor->setParentVelocity(impactVelocity);
    }else if(bodyUserDataA->getEntityType()==OBJECT_TYPE::WALL && bodyUserDataB->getEntityType()==OBJECT_TYPE::SENSOR_OBSTACLE){
        Wall* wall = dynamic_cast<Exit*>( bodyUserDataA );
        Sensor* sensor = dynamic_cast<Sensor*>( bodyUserDataB );
        Sensor* parent = dynamic_cast<Sensor*>(sensor->getParentSensor());
        if(parent->removeObjectFromCheckExitContainer(sensor->getWhereIWasSend(),sensor->getId())){
           sensor->isAlive=false;
        }else if(parent->removeObjectFromCheckAgentContainer(sensor->getWhereIWasSend(),sensor->getId())){
           sensor->isAlive=false;
        }
    }else if(bodyUserDataA->getEntityType()==OBJECT_TYPE::ACTOR && bodyUserDataB->getEntityType()==OBJECT_TYPE::SENSOR_OBSTACLE)
    {
        contact->SetEnabled(false);
        Agent* actor = dynamic_cast<Agent*>( bodyUserDataA );
        Sensor* sensor = dynamic_cast<Sensor*>( bodyUserDataB );
        if(sensor->type==sensorType::OBSTACLE_ACTOR){
            if(sensor->getWhereIWasSend()==actor->getId()){
                Sensor* parent = dynamic_cast<Sensor*>(sensor->getParentSensor());
                parent->addObjectId(actor->getId());
                if(parent->removeObjectFromCheckAgentContainer(actor->getId(),sensor->getId())){
                    sensor->isAlive=false;
                }
            }
        }
    }else if(bodyUserDataA->getEntityType()==OBJECT_TYPE::WALL_EXIT && bodyUserDataB->getEntityType()==OBJECT_TYPE::ACTOR)
    {
        Agent* actor = dynamic_cast<Agent*>( bodyUserDataB );
        Exit* exit = dynamic_cast<Exit*>( bodyUserDataA );
        actor->setPassedExit(exit->getId());
    }
    else if(bodyUserDataA->getEntityType()==OBJECT_TYPE::WALL_EXIT && bodyUserDataB->getEntityType()==OBJECT_TYPE::SENSOR_OBSTACLE)
    {
        Exit* exit = dynamic_cast<Exit*>( bodyUserDataA );
        Sensor* sensor = dynamic_cast<Sensor*>( bodyUserDataB );
        if(sensor->type==sensorType::OBSTACLE_EXIT){
            if(sensor->getWhereIWasSend()==exit->getId()){
                Sensor* parent = dynamic_cast<Sensor*>(sensor->getParentSensor());
                if(exit->mainExit){
                    parent->setMainExitDirection(exit->getId(),exit->getMiddlePoint());
                }else{
                    parent->addObjectIdPos(exit->getId(),exit->getMiddlePoint());
                }
                if(parent->removeObjectFromCheckExitContainer(exit->getId(),sensor->getId())){
                    sensor->isAlive=false;
                }
            }
        }
    }

}

void ContactListener::EndContact(b2Contact* contact)
{
    Object* bodyUserDataA = (Object*)contact->GetFixtureA()->GetBody()->GetUserData();
    Object* bodyUserDataB = (Object*)contact->GetFixtureB()->GetBody()->GetUserData();
    if ( (bodyUserDataA->getEntityType()==OBJECT_TYPE::CURSOR && bodyUserDataB->getEntityType()==OBJECT_TYPE::ACTOR))
    {
        dynamic_cast<Cursor*>( bodyUserDataA )->endContact();
        dynamic_cast<Agent*>( bodyUserDataB )->endContact();
    }else if(bodyUserDataA->getEntityType()==OBJECT_TYPE::ACTOR && bodyUserDataB->getEntityType()==OBJECT_TYPE::SENSOR_COMMUNICATION)
    {
        Agent* actor = dynamic_cast<Agent*>( bodyUserDataA );
        Sensor* sensor = dynamic_cast<Sensor*>( bodyUserDataB );
        sensor->endContact();
        actor->endContact();
    }else if(bodyUserDataA->getEntityType()==OBJECT_TYPE::WALL_EXIT && bodyUserDataB->getEntityType()==OBJECT_TYPE::SENSOR_SIGHT)
    {
        Exit* exit = dynamic_cast<Exit*>( bodyUserDataA );
        Sensor* sensor = dynamic_cast<Sensor*>( bodyUserDataB );
        sensor->endContact();
        exit->endContact();
    }else if(bodyUserDataA->getEntityType()==OBJECT_TYPE::WALL && bodyUserDataB->getEntityType()==OBJECT_TYPE::SENSOR_MOVE){
            Sensor* sensor = dynamic_cast<Sensor*>( bodyUserDataB );
    }else if(bodyUserDataA->getEntityType()==OBJECT_TYPE::WALL_EXIT && bodyUserDataB->getEntityType()==OBJECT_TYPE::ACTOR)
    {
        Agent* actor = dynamic_cast<Agent*>( bodyUserDataB );
        Exit* exit = dynamic_cast<Exit*>( bodyUserDataA );
        actor->startContact();
        exit->startContact();
        b2Vec2 pos = exit->body->GetPosition();
        //actor->setPassedExit(exit->getId());
        if(exit->mainExit){
            actor->isAlive=false;
        }
    }
    bContact = false;
}

void ContactListener::PreSolve(b2Contact* contact, const b2Manifold* )
{
    Object* bodyUserDataA = (Object*)contact->GetFixtureA()->GetBody()->GetUserData();
    Object* bodyUserDataB = (Object*)contact->GetFixtureB()->GetBody()->GetUserData();
    if(bodyUserDataA->getEntityType()==OBJECT_TYPE::ACTOR && bodyUserDataB->getEntityType()==OBJECT_TYPE::SENSOR_OBSTACLE)
    {
        Sensor* sensor = dynamic_cast<Sensor*>( bodyUserDataB );
        if(sensor->type==sensorType::OBSTACLE_ACTOR){
            contact->SetEnabled(false);
        }
    }else if(bodyUserDataA->getEntityType()==OBJECT_TYPE::WALL_EXIT && bodyUserDataB->getEntityType()==OBJECT_TYPE::ACTOR)
    {
        contact->SetEnabled(false);
        Agent* actor = dynamic_cast<Agent*>( bodyUserDataB );
        Exit* exit = dynamic_cast<Exit*>( bodyUserDataA );
        actor->startContact();
        exit->startContact();

    }else if(bodyUserDataA->getEntityType()==OBJECT_TYPE::WALL_EXIT && bodyUserDataB->getEntityType()==OBJECT_TYPE::SENSOR_OBSTACLE)
    {
        Exit* exit = dynamic_cast<Exit*>( bodyUserDataA );
        Sensor* sensor = dynamic_cast<Sensor*>( bodyUserDataB );
        if(sensor->type==sensorType::OBSTACLE_EXIT){
            if(sensor->getWhereIWasSend()!=exit->getId()){
                contact->SetEnabled(false);
            }
        }else{
            contact->SetEnabled(false);
        }
    }else if(bodyUserDataA->getEntityType()==OBJECT_TYPE::WALL_EXIT && bodyUserDataB->getEntityType()==OBJECT_TYPE::SENSOR_COLLISION){
        contact->SetEnabled(false);
    }
}


void ContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
{

}

