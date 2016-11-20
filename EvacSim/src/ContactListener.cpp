
#include "Mmath.h"
#include "ContactListener.h"
#include "Cursor.h"
#include "Actor.h"
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
        Actor* actor = dynamic_cast<Actor*>( bodyUserDataA );
        Sensor* sensor = dynamic_cast<Sensor*>( bodyUserDataB );
        sensor->addObjectToCheckActorContainer(actor->getId(),actor->body->GetPosition());
    }
    else if(bodyUserDataA->getEntityType()==OBJECT_TYPE::WALL_EXIT && bodyUserDataB->getEntityType()==OBJECT_TYPE::SENSOR_SIGHT)
    {
        Exit* exit = dynamic_cast<Exit*>( bodyUserDataA );
        Sensor* sensor = dynamic_cast<Sensor*>( bodyUserDataB );
        sensor->addObjectToCheckExitContainer(exit->getId(),exit->getMiddlePoint());
    }
    else if(bodyUserDataA->getEntityType()==OBJECT_TYPE::WALL && bodyUserDataB->getEntityType()==OBJECT_TYPE::SENSOR_MOVE)
    {
        //b2Vec2 vel1 = bodyUserDataA->body->GetLinearVelocityFromWorldPoint( worldManifold.points[0] );
        //b2Vec2 vel2 = bodyUserDataB->body->GetLinearVelocityFromWorldPoint( worldManifold.points[0] );
        b2Vec2 pos1 = worldManifold.points[0];
        b2Vec2 pos2 = dynamic_cast<Sensor*>(bodyUserDataB)->getParentSensor()->body->GetPosition();
        b2Vec2 impactVelocity = pos1 - pos2;
        //std::cout<<"VELOCITY: x:"<<impactVelocity.x<<" y:"<<impactVelocity.y<<std::endl;
        impactVelocity.x *= 100;//(std::rand()%100);
        impactVelocity.y  *= 100;//(std::rand()%100);
        Sensor* sensor = dynamic_cast<Sensor*>( bodyUserDataB );
        //sensor->rotateBody=false;
        sensor->setParentVelocity(impactVelocity);
    }
    else if(bodyUserDataA->getEntityType()==OBJECT_TYPE::WALL && bodyUserDataB->getEntityType()==OBJECT_TYPE::SENSOR_COLLISION){
        //b2Vec2 vel1 = bodyUserDataA->body->GetLinearVelocityFromWorldPoint( worldManifold.points[0] );
        //b2Vec2 vel2 = bodyUserDataB->body->GetLinearVelocityFromWorldPoint( worldManifold.points[0] );
        b2Vec2 pos1 = worldManifold.points[0];
        b2Vec2 pos2 = dynamic_cast<Sensor*>(bodyUserDataB)->getParentSensor()->body->GetPosition();
        b2Vec2 impactVelocity = pos1 - pos2;
        //b2Vec2 impactVelocity = vel1 - vel2;
        //std::cout<<"VELOCITY: x:"<<impactVelocity.x<<" y:"<<impactVelocity.y<<std::endl;
        impactVelocity.x *= 100;//(std::rand()%100);
        impactVelocity.y  *= 100;//(std::rand()%100);
        Sensor* sensor = dynamic_cast<Sensor*>( bodyUserDataB );
        sensor->setParentVelocity(impactVelocity);
    }else if(bodyUserDataA->getEntityType()==OBJECT_TYPE::WALL && bodyUserDataB->getEntityType()==OBJECT_TYPE::SENSOR_OBSTACLE){
        Wall* wall = dynamic_cast<Exit*>( bodyUserDataA );
        Sensor* sensor = dynamic_cast<Sensor*>( bodyUserDataB );
        Sensor* parent = dynamic_cast<Sensor*>(sensor->getParentSensor());
        if(parent->removeObjectFromCheckExitContainer(sensor->getWhereIWasSend(),sensor->getId())){
           sensor->isAlive=false;
        }
    }/*else if(bodyUserDataA->getEntityType()==OBJECT_TYPE::WALL_EXIT && bodyUserDataB->getEntityType()==OBJECT_TYPE::ACTOR)
    {
        Actor* actor = dynamic_cast<Actor*>( bodyUserDataB );
        Exit* exit = dynamic_cast<Exit*>( bodyUserDataA );
        actor->startContact();
        exit->startContact();

    }*/else if(bodyUserDataA->getEntityType()==OBJECT_TYPE::ACTOR && bodyUserDataB->getEntityType()==OBJECT_TYPE::SENSOR_OBSTACLE)
    {
        contact->SetEnabled(false);
        Actor* actor = dynamic_cast<Actor*>( bodyUserDataA );
        Sensor* sensor = dynamic_cast<Sensor*>( bodyUserDataB );
        if(sensor->type==sensorType::OBSTACLE_ACTOR){
            if(sensor->getWhereIWasSend()==actor->getId()){
                Sensor* parent = dynamic_cast<Sensor*>(sensor->getParentSensor());
                parent->addObjectId(actor->getId());
                if(parent->removeObjectFromCheckActorContainer(actor->getId(),sensor->getId())){
                    sensor->isAlive=false;
                }
            }
        }
    }else if(bodyUserDataA->getEntityType()==OBJECT_TYPE::WALL_EXIT && bodyUserDataB->getEntityType()==OBJECT_TYPE::SENSOR_OBSTACLE)
    {
        Exit* exit = dynamic_cast<Exit*>( bodyUserDataA );
        Sensor* sensor = dynamic_cast<Sensor*>( bodyUserDataB );
        if(sensor->type==sensorType::OBSTACLE_EXIT){
            if(sensor->getWhereIWasSend()==exit->getId()){
                Sensor* parent = dynamic_cast<Sensor*>(sensor->getParentSensor());
                if(exit->mainExit){
                    //Actor* actor = dynamic_cast<Actor*>( parent );
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

    //check if fixture A was a Object
    Object* bodyUserDataA = (Object*)contact->GetFixtureA()->GetBody()->GetUserData();
    Object* bodyUserDataB = (Object*)contact->GetFixtureB()->GetBody()->GetUserData();
    // std::cout<<bodyUserData->getEntityType()<<std::endl;
    if ( (bodyUserDataA->getEntityType()==OBJECT_TYPE::CURSOR && bodyUserDataB->getEntityType()==OBJECT_TYPE::ACTOR))
    {
        dynamic_cast<Cursor*>( bodyUserDataA )->endContact();
        dynamic_cast<Actor*>( bodyUserDataB )->endContact();
    }else if(bodyUserDataA->getEntityType()==OBJECT_TYPE::ACTOR && bodyUserDataB->getEntityType()==OBJECT_TYPE::SENSOR_COMMUNICATION)
    {
        Actor* actor = dynamic_cast<Actor*>( bodyUserDataA );
        Sensor* sensor = dynamic_cast<Sensor*>( bodyUserDataB );
        sensor->endContact();
        actor->endContact();
        //sensor->removeObjectId(actor->getId());
    }else if(bodyUserDataA->getEntityType()==OBJECT_TYPE::WALL_EXIT && bodyUserDataB->getEntityType()==OBJECT_TYPE::SENSOR_SIGHT)
    {
        Exit* exit = dynamic_cast<Exit*>( bodyUserDataA );
        Sensor* sensor = dynamic_cast<Sensor*>( bodyUserDataB );
        sensor->endContact();
        exit->endContact();
        // sensor->removeObjectIdPos(exit->getId());
        //sensor1->printData();
        //sensor2->printData();
    }else if(bodyUserDataA->getEntityType()==OBJECT_TYPE::WALL && bodyUserDataB->getEntityType()==OBJECT_TYPE::SENSOR_MOVE){
            Sensor* sensor = dynamic_cast<Sensor*>( bodyUserDataB );
            sensor->rotateBody=true;
    }else if(bodyUserDataA->getEntityType()==OBJECT_TYPE::WALL_EXIT && bodyUserDataB->getEntityType()==OBJECT_TYPE::ACTOR)
    {
        Actor* actor = dynamic_cast<Actor*>( bodyUserDataB );
        Exit* exit = dynamic_cast<Exit*>( bodyUserDataA );
        actor->startContact();
        exit->startContact();
        b2Vec2 pos = exit->body->GetPosition();
        //std::cout<<"EXIT ID:"<<exit->getId()<<" POS x:"<<pos.x<<" y: "<<pos.y<<std::endl;
        actor->setPassedExit(exit->getId());
        //if(exit->mainExit){
        //    actor->isAlive=false;
        //}
    }
    bContact = false;
}

void ContactListener::PreSolve(b2Contact* contact, const b2Manifold* )
{
    Object* bodyUserDataA = (Object*)contact->GetFixtureA()->GetBody()->GetUserData();
    Object* bodyUserDataB = (Object*)contact->GetFixtureB()->GetBody()->GetUserData();
    /*if ( (bodyUserDataA->getEntityType()==OBJECT_TYPE::CURSOR && bodyUserDataB->getEntityType()==OBJECT_TYPE::CURSOR))
    {
        if(dynamic_cast<Cursor*>( bodyUserDataA )->parentId==dynamic_cast<Cursor*>( bodyUserDataB )->parentId)
            contact->SetEnabled(false);
    } contact->SetEnabled(false)*/
    if(bodyUserDataA->getEntityType()==OBJECT_TYPE::ACTOR && bodyUserDataB->getEntityType()==OBJECT_TYPE::SENSOR_OBSTACLE)
    {
        Sensor* sensor = dynamic_cast<Sensor*>( bodyUserDataB );
        if(sensor->type==sensorType::OBSTACLE_ACTOR){
            contact->SetEnabled(false);
        }
        //Actor* actor = dynamic_cast<Actor*>( bodyUserDataA );
        //Sensor* sensor = dynamic_cast<Sensor*>( bodyUserDataB );
        /*if(sensor->type==sensorType::OBSTACLE_ACTOR){
            if(sensor->getWhereIWasSend()!=actor->getId()){
                contact->SetEnabled(false);
            }
        }*/
    }else if(bodyUserDataA->getEntityType()==OBJECT_TYPE::WALL_EXIT && bodyUserDataB->getEntityType()==OBJECT_TYPE::ACTOR)
    {
        contact->SetEnabled(false);
        Actor* actor = dynamic_cast<Actor*>( bodyUserDataB );
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
    //if(bContact){
    Object* bodyUserDataA = (Object*)contact->GetFixtureA()->GetBody()->GetUserData();
    Object* bodyUserDataB = (Object*)contact->GetFixtureB()->GetBody()->GetUserData();
    // if ( (bodyUserDataA && bodyUserDataB)){
    //   std::cout<<"impulse->count: "<<impulse->count<<std::endl;
    for(int i=0; i<impulse->count; i++)
    {
        //    std::cout<<"impulse->normalImpulses "<<i<<" :"<<impulse->normalImpulses[i]<<std::endl;
        bodyUserDataA->setDamage(impulse->normalImpulses[i]);
        bodyUserDataB->setDamage(impulse->normalImpulses[i]);
    }

    //}
    // }
}

