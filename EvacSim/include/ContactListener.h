#ifndef CONTACTLISTENER_H
#define CONTACTLISTENER_H

#include <Box2D/Box2D.h>
#include <typeinfo>
#include <iostream>

class ContactListener : public b2ContactListener
{
private:
    bool bContact;  //checking if there is a contact
public:
    ContactListener();
    ~ContactListener();
    void BeginContact(b2Contact* contact);
    void EndContact(b2Contact* contact);
    void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
    void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);

//    glm::vec2 vertices;
};
//d
#endif // CONTACTLISTENER_H
