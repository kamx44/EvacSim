#ifndef BUILDING3_H
#define BUILDING3_H
#include "Building.h"

class Building3 : public Building
{
    public:
        Building3(ObjectsContainer* objectsContainer);
        virtual ~Building3();
        void createRooms();

    protected:

    private:
};

#endif // BUILDING1_H
