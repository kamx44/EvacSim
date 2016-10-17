
#include "ContactListener.h"
#include "Cursor.h"
#include "Actor.h"
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
   if (bodyUserDataA->getEntityType()==OBJECT_TYPE::CURSOR && bodyUserDataB->getEntityType()==OBJECT_TYPE::ACTOR)
    {
        dynamic_cast<Cursor*>( bodyUserDataA )->startContact();
        dynamic_cast<Actor*>( bodyUserDataB )->startContact();
    }else if(bodyUserDataA->getEntityType()==OBJECT_TYPE::SENSOR_SIGHT && bodyUserDataB->getEntityType()==OBJECT_TYPE::ACTOR)
    {
        Sensor* sensor = dynamic_cast<Sensor*>( bodyUserDataA );
        Actor* actor = dynamic_cast<Actor*>( bodyUserDataB );
        sensor->startContact();
        actor->startContact();
        sensor->addVisibleObject(actor->getId());
    }else if(bodyUserDataA->getEntityType()==OBJECT_TYPE::SENSOR_SIGHT && bodyUserDataB->getEntityType()==OBJECT_TYPE::SENSOR_SIGHT)
    {
        Sensor* sensor1 = dynamic_cast<Sensor*>( bodyUserDataA );
        Sensor* sensor2 = dynamic_cast<Sensor*>( bodyUserDataB );
       // std::cout<<"sensor catched"<<std::endl;
      /*  sensor1->startContact();
        sensor2->startContact();
        sensor1->addVisibleObject(sensor2->getParentId());
        sensor2->addVisibleObject(sensor1->getParentId()); */
        //sensor1->printData();
        //sensor2->printData();
    }else if(bodyUserDataA->getEntityType()==OBJECT_TYPE::WALL && bodyUserDataB->getEntityType()==OBJECT_TYPE::SENSOR_MOVE){
        b2Vec2 vel1 = bodyUserDataA->body->GetLinearVelocityFromWorldPoint( worldManifold.points[0] );
        b2Vec2 vel2 = bodyUserDataB->body->GetLinearVelocityFromWorldPoint( worldManifold.points[0] );
        b2Vec2 impactVelocity = vel1 - vel2;
        //std::cout<<"VELOCITY: x:"<<impactVelocity.x<<" y:"<<impactVelocity.y<<std::endl;
        impactVelocity.x *= (std::rand()%100);
        impactVelocity.y  *= (std::rand()%100);
        Sensor* sensor = dynamic_cast<Sensor*>( bodyUserDataB );
        sensor->rotateBody=false;
        sensor->setParentVelocity(impactVelocity);
    }else if(bodyUserDataA->getEntityType()==OBJECT_TYPE::WALL && bodyUserDataB->getEntityType()==OBJECT_TYPE::SENSOR_COLLISION){
        b2Vec2 vel1 = bodyUserDataA->body->GetLinearVelocityFromWorldPoint( worldManifold.points[0] );
        b2Vec2 vel2 = bodyUserDataB->body->GetLinearVelocityFromWorldPoint( worldManifold.points[0] );
        b2Vec2 impactVelocity = vel1 - vel2;
        //std::cout<<"VELOCITY: x:"<<impactVelocity.x<<" y:"<<impactVelocity.y<<std::endl;
        impactVelocity.x *= (std::rand()%100);
        impactVelocity.y  *= (std::rand()%100);
        Sensor* sensor = dynamic_cast<Sensor*>( bodyUserDataB );
        sensor->setParentVelocity(impactVelocity);
    }
   /* else if ( (bodyUserDataA->getEntityType()==OBJECT_TYPE::CURSOR && bodyUserDataB->getEntityType()==OBJECT_TYPE::CURSOR))
    {
        //     std::cout<<"KUIRWAAA"<<std::endl;
    } */
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
    }else if(bodyUserDataA->getEntityType()==OBJECT_TYPE::SENSOR_SIGHT && bodyUserDataB->getEntityType()==OBJECT_TYPE::ACTOR)
    {
        Sensor* sensor = dynamic_cast<Sensor*>( bodyUserDataA );
        Actor* actor = dynamic_cast<Actor*>( bodyUserDataB );
        sensor->endContact();
        actor->endContact();
        sensor->removeInvisibleObject(actor->getId());
    }else if(bodyUserDataA->getEntityType()==OBJECT_TYPE::SENSOR_SIGHT && bodyUserDataB->getEntityType()==OBJECT_TYPE::SENSOR_SIGHT)
    {
       // std::cout<<"sensor missed"<<std::endl;
        /*Sensor* sensor1 = dynamic_cast<Sensor*>( bodyUserDataA );
        Sensor* sensor2 = dynamic_cast<Sensor*>( bodyUserDataB );
        sensor1->endContact();
        sensor2->endContact();
        sensor1->removeInvisibleObject(sensor2->getParentId());
        sensor2->removeInvisibleObject(sensor1->getParentId()); */
        //sensor1->printData();
        //sensor2->printData();
    }else if(bodyUserDataA->getEntityType()==OBJECT_TYPE::WALL && bodyUserDataB->getEntityType()==OBJECT_TYPE::SENSOR_MOVE){
            Sensor* sensor = dynamic_cast<Sensor*>( bodyUserDataB );
            sensor->rotateBody=true;
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
    } */
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

