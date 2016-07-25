
#include "ContactListener.h"
#include "Module.h"
#include "Missle.h"
#include "Asteroide.h"
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
    if ( (bodyUserDataA->getEntityType()==OBJECT_TYPE::ASTEROIDE && bodyUserDataB->getEntityType()==OBJECT_TYPE::MISSLE))
    {
        dynamic_cast<Missle*>( bodyUserDataB )->startContact();
        dynamic_cast<Asteroide*>( bodyUserDataA )->startContact();
    }
    else if ( (bodyUserDataA->getEntityType()==OBJECT_TYPE::MODULE && bodyUserDataB->getEntityType()==OBJECT_TYPE::ASTEROIDE))
    {
        dynamic_cast<Module*>( bodyUserDataA )->startContact();
        dynamic_cast<Asteroide*>( bodyUserDataB )->startContact();

    }
    else if ( (bodyUserDataA->getEntityType()==OBJECT_TYPE::MODULE && bodyUserDataB->getEntityType()==OBJECT_TYPE::MODULE))
    {
        //     std::cout<<"KUIRWAAA"<<std::endl;
    }
}

void ContactListener::EndContact(b2Contact* contact)
{

    //check if fixture A was a Object
    Object* bodyUserDataA = (Object*)contact->GetFixtureA()->GetBody()->GetUserData();
    Object* bodyUserDataB = (Object*)contact->GetFixtureB()->GetBody()->GetUserData();
    // std::cout<<bodyUserData->getEntityType()<<std::endl;
    if ( (bodyUserDataA->getEntityType()==OBJECT_TYPE::ASTEROIDE && bodyUserDataB->getEntityType()==OBJECT_TYPE::MISSLE))
    {
        dynamic_cast<Asteroide*>( bodyUserDataA )->endContact();
        dynamic_cast<Missle*>( bodyUserDataB )->endContact();
    }
    else if ( (bodyUserDataA->getEntityType()==OBJECT_TYPE::MODULE && bodyUserDataB->getEntityType()==OBJECT_TYPE::ASTEROIDE))
    {
        dynamic_cast<Module*>( bodyUserDataA )->endContact();
        dynamic_cast<Asteroide*>( bodyUserDataB )->endContact();
    }
    bContact = false;
}

void ContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{
    Object* bodyUserDataA = (Object*)contact->GetFixtureA()->GetBody()->GetUserData();
    Object* bodyUserDataB = (Object*)contact->GetFixtureB()->GetBody()->GetUserData();
    if ( (bodyUserDataA->getEntityType()==OBJECT_TYPE::MODULE && bodyUserDataB->getEntityType()==OBJECT_TYPE::MODULE))
    {
        if(dynamic_cast<Module*>( bodyUserDataA )->parentId==dynamic_cast<Module*>( bodyUserDataB )->parentId)
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

