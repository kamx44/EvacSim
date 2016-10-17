#ifndef WALL_H
#define WALL_H
#include "Object.h"
#include "World.h"


class Wall : public Object
{
    public:
        Wall(glm::vec2 startPosition,glm::vec2 endPosition);
        glm::vec2 startPosition;
        glm::vec2 endPosition;
        virtual void defineFixture(b2EdgeShape&);
        void update(float dt);
        void draw();
        glm::vec2 getMiddlePoint();
        virtual ~Wall();
    protected:
    private:
};

#endif // WALL_H
