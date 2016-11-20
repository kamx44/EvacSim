#ifndef EXIT_H
#define EXIT_H
#include "Object.h"
#include "World.h"
#include "Wall.h"

class Exit : public Wall
{
    public:
        Exit(glm::vec2 startPosition,glm::vec2 endPosition);
        virtual ~Exit();
        virtual void defineFixture(b2EdgeShape&);
        b2Vec2 getMiddlePoint();
        bool mainExit;

    protected:

    private:
};

#endif // EXIT_H
