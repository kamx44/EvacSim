#include "Building2.h"

using std::vector;
using std::pair;

Building2::Building2(ObjectsContainer* objectsContainer) :
    Building(objectsContainer)
{
    //ctor
}

Building2::~Building2()
{
    //dtor
}

void Building2::createRooms(){
    //ROOM1
    vector<pairGlm> escRooms1{pairGlm(glm::vec2(-30.0f,0.0f),glm::vec2(-20.0f,0.0f))};
    Room* room = new Room(objectsContainer,excRoomsContainer,glm::vec2(-30.0f,0.0f),glm::vec2(0.0f,30.0f),escRooms1);
    roomsContainer.push_back(room);
    excRoomsContainer.insert(std::end(excRoomsContainer),std::begin(escRooms1),std::end(escRooms1));

    //ROOM2
    vector<pairGlm> escRooms2{pairGlm(glm::vec2(0.0f,30.0f),glm::vec2(0.0f,40.0f))};
    room = new Room(objectsContainer,excRoomsContainer,glm::vec2(-30.0f,30.0f),glm::vec2(0.0f,60.0f),escRooms2);
    roomsContainer.push_back(room);
    excRoomsContainer.insert(std::end(excRoomsContainer),std::begin(escRooms2),std::end(escRooms2));

    //ROOM3
    vector<pairGlm> escRooms3{pairGlm(glm::vec2(-10.0f,-10.0f),glm::vec2(0.0f,-10.0f))};
    room = new Room(objectsContainer,excRoomsContainer,glm::vec2(-30.0f,-40.0f),glm::vec2(0.0f,-10.0f),escRooms3);
    roomsContainer.push_back(room);
    excRoomsContainer.insert(std::end(excRoomsContainer),std::begin(escRooms3),std::end(escRooms3));

    //ROOM4
    vector<pairGlm> escRooms4{pairGlm(glm::vec2(10.0f,0.0f),glm::vec2(20.0f,0.0f))};
    room = new Room(objectsContainer,excRoomsContainer,glm::vec2(10.0f,0.0f),glm::vec2(40.0f,30.0f),escRooms4);
    roomsContainer.push_back(room);
    excRoomsContainer.insert(std::end(excRoomsContainer),std::begin(escRooms4),std::end(escRooms4));

    //ROOM5
    vector<pairGlm> escRooms5{
        pairGlm(glm::vec2(40.0f,-40.0f),glm::vec2(40.0f,-30.0f)),
        pairGlm(glm::vec2(30.0f,-10.0f),glm::vec2(40.0f,-10.0f))
        };
    room = new Room(objectsContainer,excRoomsContainer,glm::vec2(10.0f,-40.0f),glm::vec2(40.0f,-10.0f),escRooms5);
    roomsContainer.push_back(room);
    excRoomsContainer.insert(std::end(excRoomsContainer),std::begin(escRooms5),std::end(escRooms5));

    //HALL1
    vector<pairGlm> escRooms6{
        pairGlm(glm::vec2(-10.0f,-10.0f),glm::vec2(0.0f,-10.0f)),
        pairGlm(glm::vec2(0.0f,-10.0f),glm::vec2(0.0f,0.0f)),
        pairGlm(glm::vec2(-20.0f,0.0f),glm::vec2(-30.0f,0.0f))
        };
    room = new Room(objectsContainer,excRoomsContainer,glm::vec2(-30.0f,-10.0f),glm::vec2(0.0f,0.0f),escRooms6);
    //roomsContainer.push_back(room);
    excRoomsContainer.insert(std::end(excRoomsContainer),std::begin(escRooms6),std::end(escRooms6));

    //HALL2
    vector<pairGlm> escRooms7{
        pairGlm(glm::vec2(0.0f,40.0f),glm::vec2(0.0f,30.0f)),
        pairGlm(glm::vec2(0.0f,0.0f),glm::vec2(0.0f,-10.0f)),
        pairGlm(glm::vec2(10.0f,-10.0f),glm::vec2(10.0f,0.0f)),
        pairGlm(glm::vec2(10.0f,30.0f),glm::vec2(10.0f,40.0f))
        };
    room = new Room(objectsContainer,excRoomsContainer,glm::vec2(0.0f,-40.0f),glm::vec2(10.0f,60.0f),escRooms7);
    //std::cout<<"x: "<<room->exits[1]->startPosition.x<<"y: "<<room->exits[1]->startPosition.y<<std::endl;
    room->exits[1]->mainExit = true;
    //roomsContainer.push_back(room);
    excRoomsContainer.insert(std::end(excRoomsContainer),std::begin(escRooms7),std::end(escRooms7));

    //HALL3
    vector<pairGlm> escRooms8{
        pairGlm(glm::vec2(40.0f,-30.0f),glm::vec2(40.0f,-40.0f)),
        pairGlm(glm::vec2(50.0f,10.0f),glm::vec2(50.0f,20.0f))
        };
    room = new Room(objectsContainer,excRoomsContainer,glm::vec2(40.0f,-40.0f),glm::vec2(50.0f,20.0f),escRooms8);
    room->exits[0]->mainExit = true;
    //std::cout<<"x: "<<room->exits[0]->startPosition.x<<"y: "<<room->exits[0]->startPosition.y<<std::endl;
    //roomsContainer.push_back(room);
    excRoomsContainer.insert(std::end(excRoomsContainer),std::begin(escRooms8),std::end(escRooms8));

}
