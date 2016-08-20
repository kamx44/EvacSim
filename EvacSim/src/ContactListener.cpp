
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
    Object* bodyUserDataA = (Object*)contact->GetFixtureA()->GetBody()->GetUserData();
    Object* bodyUserDataB = (Object*)contact->GetFixtureB()->GetBody()->GetUserData();
   if (bodyUserDataA->getEntityType()==OBJECT_TYPE::CURSOR && bodyUserDataB->getEntityType()==OBJECT_TYPE::ACTOR)
    {
        dynamic_cast<Cursor*>( bodyUserDataA )->startContact();
        dynamic_cast<Actor*>( bodyUserDataB )->startContact();
    }else if(bodyUserDataA->getEntityType()==OBJECT_TYPE::SENSOR && bodyUserDataB->getEntityType()==OBJECT_TYPE::ACTOR)
    {
        dynamic_cast<Sensor*>( bodyUserDataA )->startContact();
        dynamic_cast<Actor*>( bodyUserDataB )->startContact();
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
    }else if(bodyUserDataA->getEntityType()==OBJECT_TYPE::SENSOR && bodyUserDataB->getEntityType()==OBJECT_TYPE::ACTOR)
    {
        dynamic_cast<Sensor*>( bodyUserDataA )->endContact();
        dynamic_cast<Actor*>( bodyUserDataB )->endContact();
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

