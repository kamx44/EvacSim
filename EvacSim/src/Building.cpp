#include <glm/glm.hpp>
#include <glm/detail/type_mat2x2.hpp>
#include "Building.h"
#include "Room.h"
#include <vector>

using std::vector;
using std::pair;
typedef pair<glm::vec2,glm::vec2> pairGlm;

Building::Building(ObjectsContainer* objectsContainer)
{
    this->objectsContainer=objectsContainer;
    createRooms();
}

Building::~Building()
{
    //dtor
}

void Building::createRooms(){
    vector<pairGlm> escRooms1{pairGlm(glm::vec2(0.0f,0.0f),glm::vec2(0.0f,10.0f))};
    Room* room = new Room(objectsContainer,glm::vec2(-40.0f,0.0f),glm::vec2(0.0f,40.0f),escRooms1);
    roomsContainer.push_back(room);
    vector<pairGlm> escRooms2{
        pairGlm(glm::vec2(0.0f,0.0f),glm::vec2(0.0f,10.0f)),
        pairGlm(glm::vec2(0.0f,-10.0f),glm::vec2(0.0f,-20.0f)),
        pairGlm(glm::vec2(10.0f,-10.0f),glm::vec2(10.0f,-20.0f))
        };
    room = new Room(objectsContainer,glm::vec2(0.0f,-20.0f),glm::vec2(10.0f,10.0f),escRooms2);
    roomsContainer.push_back(room);
    vector<pairGlm> escRooms3{pairGlm(glm::vec2(0.0f,-10.0f),glm::vec2(0.0f,-20.0f))};
    room = new Room(objectsContainer,glm::vec2(-40.0f,-40.0f),glm::vec2(0.0f,-10.0f),escRooms3);
    roomsContainer.push_back(room);
    vector<pairGlm> escRooms4{pairGlm(glm::vec2(30.0f,20.0f),glm::vec2(40.0f,20.0f))};
    room = new Room(objectsContainer,glm::vec2(10.0f,20.0f),glm::vec2(40.0f,40.0f),escRooms4);
    roomsContainer.push_back(room);
    vector<pairGlm> escRooms5{
        pairGlm(glm::vec2(30.0f,10.0f),glm::vec2(40.0f,10.0f)),
        pairGlm(glm::vec2(40.0f,20.0f),glm::vec2(30.0f,20.0f))
        };
    room = new Room(objectsContainer,glm::vec2(30.0f,10.0f),glm::vec2(40.0f,20.0f),escRooms5);
    roomsContainer.push_back(room);
    vector<pairGlm> escRooms6{
        pairGlm(glm::vec2(30.0f,10.0f),glm::vec2(40.0f,10.0f)),
        pairGlm(glm::vec2(20.0f,-10.0f),glm::vec2(30.0f,-10.0f))
        };
    room = new Room(objectsContainer,glm::vec2(20.0f,-10.0f),glm::vec2(40.0f,10.0f),escRooms6);
    roomsContainer.push_back(room);
    vector<pairGlm> escRooms7{
        pairGlm(glm::vec2(20.0f,-10.0f),glm::vec2(30.0f,-10.0f)),
        pairGlm(glm::vec2(20.0f,-20.0f),glm::vec2(10.0f,-20.0f)),
        pairGlm(glm::vec2(10.0f,-20.0f),glm::vec2(10.0f,-10.0f))
        };
    room = new Room(objectsContainer,glm::vec2(10.0f,-20.0f),glm::vec2(40.0f,-10.0f),escRooms7);
    roomsContainer.push_back(room);
    vector<pairGlm> escRooms8{
        pairGlm(glm::vec2(20.0f,-20.0f),glm::vec2(10.0f,-20.0f)),
        pairGlm(glm::vec2(40.0f,-30.0f),glm::vec2(40.0f,-40.0f))
        };
    room = new Room(objectsContainer,glm::vec2(10.0f,-40.0f),glm::vec2(40.0f,-20.0f),escRooms8);
    roomsContainer.push_back(room);

}

Sector* Building::getFreeSector(){
    Sector* freeSector;
    for(Room* room : roomsContainer){
        if((freeSector = room->getFreeSector())!=nullptr)
            return freeSector;
    }
    return nullptr;
}
