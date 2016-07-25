#ifndef MISSLE_H
#define MISSLE_H


#include <glm/common.hpp>
#include "Object.h"

class Ship;

enum KIND
{
    BULLET,
    ROCKET,
    LASER,
    PLASMA
};

class Missle : public Object
{
public:

    Missle(Ship* ship);
    Missle(Ship* ship,KIND, glm::vec2, glm::vec2);

    Ship* ship;
    virtual ~Missle();
    KIND type;
    unsigned int myID;
    glm::vec2 positionStart;
    glm::vec2 positionTarget;
    glm::vec2 position;
    glm::vec2 velocity;
    float32 radius;
    float ttl;              // time to die!
    float ttl2;             // time of collision
    int move();
    void draw();
    void update(float dt);
    void startContact();
    void endContact();
    int state;
    virtual void delObject() {}

protected:
private:
};

#endif // MISSLE_H
