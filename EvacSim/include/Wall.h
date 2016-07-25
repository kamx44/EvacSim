#ifndef WALL_H
#define WALL_H
#include "Object.h"
#include "World.h"


class Wall : public Object
{
    public:
        Wall(glm::vec2 startPosition,glm::vec2 endPosition,World* world);
        glm::vec2 startPosition;
        glm::vec2 endPosition;
        World* world_wall;
        void update(float dt);
        void draw();
        virtual ~Wall();
    protected:
    private:
};

#endif // WALL_H
