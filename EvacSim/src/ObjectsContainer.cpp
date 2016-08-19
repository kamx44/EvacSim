#include "ObjectsContainer.h"

ObjectsContainer::ObjectsContainer()
{
    //ctor
}

ObjectsContainer::~ObjectsContainer()
{
    //dtor
}

void ObjectsContainer::addObject(Object *object)
{
    container.push_back(object);
}


void ObjectsContainer::deleteObject(Object *object)
{
    for(unsigned int i=0; i<container.size(); i++)
    {
        if(object==container[i])
        {
            if(object->isPlayer==true)
            {
                object->destroyBody();
                container.clear();
                object->drawable=false;
                delete object;
               // playerAlive=false;
            }
            else
            {
                object->destroyBody();
                container.erase(container.begin()+i);
                object->drawable=false;
                delete object;
            }
        }
    }
}

int ObjectsContainer::getSize(){
    return container.size();
}

Object* ObjectsContainer::getObjectByIndex(int index){
    return container.at(index);
}
