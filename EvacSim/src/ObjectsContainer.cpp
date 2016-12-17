#include "ObjectsContainer.h"
#include <iostream>

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
    container.insert(std::make_pair(object->getId(),object));
}


void ObjectsContainer::deleteObject(Object *object)
{
    auto search = container.find(object->getId());
    if(search != container.end()){
        if(object->object_type==OBJECT_TYPE::ACTOR)
        {
           std::cout<<"lama";
        }
        object->delObject();
        //object->destroyBody();
        container.erase(object->getId());
        object->drawable=false;
        //delete object;
        //object = NULL;
    }
}

int ObjectsContainer::getSize(){
    return container.size();
}

Object* ObjectsContainer::getObjectByIndex(int index){
    return container.at(index);
}

 std::unordered_map<unsigned int,Object*> ObjectsContainer::getContainer(){
    return container;
 }
