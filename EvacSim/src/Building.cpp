#include <glm/glm.hpp>
#include <glm/detail/type_mat2x2.hpp>
#include "Building.h"
#include "Room.h"
#include <vector>




Building::Building(ObjectsContainer* objectsContainer)
{
    this->objectsContainer=objectsContainer;
    //createRooms();
}

Building::~Building()
{
    //dtor
}

Sector* Building::getFreeSector(){
    Sector* freeSector;
    for(Room* room : roomsContainer){
        if((freeSector = room->getFreeSector())!=nullptr)
            return freeSector;
    }
    return nullptr;
}
