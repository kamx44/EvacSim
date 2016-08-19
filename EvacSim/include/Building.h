#ifndef BUILDING_H
#define BUILDING_H

#include "Room.h"

class ObjectsContainer;

class Building
{
    public:
        Building(ObjectsContainer* objectsContainer);
        virtual ~Building();
        void createRooms();
        Sector* getFreeSector();
    protected:
    private:
        ObjectsContainer* objectsContainer;
        std::vector<Room*> roomsContainer;

};

#endif // BUILDING_H
