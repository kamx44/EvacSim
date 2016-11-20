#ifndef BUILDING_H
#define BUILDING_H

#include "Room.h"

class ObjectsContainer;

typedef pair<glm::vec2,glm::vec2> pairGlm;

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
        std::vector<pairGlm> excRoomsContainer;

};

#endif // BUILDING_H
