#ifndef OBJECTSCONTAINER_H
#define OBJECTSCONTAINER_H
#include "Object.h"


class ObjectsContainer
{
    public:
        ObjectsContainer();
        virtual ~ObjectsContainer();
        void addObject(Object* object);
        void deleteObject(Object* object);
        int getSize();
        Object* getObjectByIndex(int index);
    protected:
    private:
        std::vector<Object*> container;
};

#endif // OBJECTSCONTAINER_H
