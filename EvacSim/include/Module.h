#ifndef MODULE_H
#define MODULE_H

#include "World.h"
#include "Object.h"

class World;

class Module : public Object
{
    public:
        Module(Object* parent_,glm::vec2 position_,b2Vec2* vertices_,int32 size_);
        ~Module();
        Object* parent;
      int32 size;
      int id;
      void calcSpeed();
      int parentId;
         void draw();
      virtual void setDamage(float damage);
      void update(float dt);

    protected:
    private:
};

#endif // MODULE_H
