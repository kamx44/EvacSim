#ifndef OBJECTSCONTAINER_H
#define OBJECTSCONTAINER_H
#include "Object.h"
#include <unordered_map>


class ObjectsContainer
{
    public:
        ObjectsContainer();
        virtual ~ObjectsContainer();
        void addObject(Object* object);
        void deleteObject(Object* object);
        int getSize();
        Object* getObjectByIndex(int index);
        std::unordered_map<unsigned int,Object*> getContainer();
    protected:
    private:
        std::unordered_map<unsigned int,Object*> container;
};

#endif // OBJECTSCONTAINER_H
