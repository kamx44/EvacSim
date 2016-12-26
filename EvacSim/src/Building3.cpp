#include "Building3.h"

using std::vector;
using std::pair;

Building3::Building3(ObjectsContainer* objectsContainer) :
    Building(objectsContainer)
{
    //ctor
}

Building3::~Building3()
{
    //dtor
}

void Building3::createRooms(){
    //ROOM1
    vector<pairGlm> escRooms1{pairGlm(glm::vec2(-10.0f,-20.0f),glm::vec2(-10.0f,-10.0f)),
                              pairGlm(glm::vec2(-10.0f,30.0f),glm::vec2(-10.0f,40.0f))};
    Room* room = new Room(objectsContainer,excRoomsContainer,glm::vec2(-40.0f,-30.0f),glm::vec2(-10.0f,50.0f),escRooms1);
    roomsContainer.push_back(room);
    excRoomsContainer.insert(std::end(excRoomsContainer),std::begin(escRooms1),std::end(escRooms1));

    //ROOM2
    vector<pairGlm> escRooms2{
        pairGlm(glm::vec2(20.0f,0.0f),glm::vec2(20.0f,10.0f)),
        pairGlm(glm::vec2(20.0f,20.0f),glm::vec2(10.0f,20.0f))
        };
    room = new Room(objectsContainer,excRoomsContainer,glm::vec2(0.0f,0.0f),glm::vec2(20.0f,20.0f),escRooms2);
    roomsContainer.push_back(room);
    excRoomsContainer.insert(std::end(excRoomsContainer),std::begin(escRooms2),std::end(escRooms2));

    //ROOM3
    vector<pairGlm> escRooms3{
        pairGlm(glm::vec2(0.0f,0.0f),glm::vec2(0.0f,-10.0f)),
        pairGlm(glm::vec2(20.0f,-20.0f),glm::vec2(20.0f,-30.0f))
        };
    room = new Room(objectsContainer,excRoomsContainer,glm::vec2(0.0f,-30.0f),glm::vec2(20.0f,0.0f),escRooms3);
    roomsContainer.push_back(room);
    excRoomsContainer.insert(std::end(excRoomsContainer),std::begin(escRooms3),std::end(escRooms3));

    //ROOM4
    vector<pairGlm> escRooms4{
        pairGlm(glm::vec2(20.0f,-20.0f),glm::vec2(20.0f,-30.0f)),
        pairGlm(glm::vec2(40.0f,0.0f),glm::vec2(30.0f,0.0f))
        };
    room = new Room(objectsContainer,excRoomsContainer,glm::vec2(20.0f,-30.0f),glm::vec2(40.0f,0.0f),escRooms4);
    roomsContainer.push_back(room);
    excRoomsContainer.insert(std::end(excRoomsContainer),std::begin(escRooms4),std::end(escRooms4));

    //HALL1
    vector<pairGlm> escRooms5{
        pairGlm(glm::vec2(0.0f,40.0f),glm::vec2(0.0f,50.0f)),
        pairGlm(glm::vec2(-10.0f,40.0f),glm::vec2(-10.0f,30.0f)),
        pairGlm(glm::vec2(-10.0f,-10.0f),glm::vec2(-10.0f,-20.0f)),
        pairGlm(glm::vec2(0.0f,-10.0f),glm::vec2(0.0f,0.0f))
        };
    room = new Room(objectsContainer,excRoomsContainer,glm::vec2(-10.0f,-30.0f),glm::vec2(0.0f,50.0f),escRooms5);
    //roomsContainer.push_back(room);
    excRoomsContainer.insert(std::end(excRoomsContainer),std::begin(escRooms5),std::end(escRooms5));

    //HALL2
    vector<pairGlm> escRooms6{
        pairGlm(glm::vec2(20.0f,40.0f),glm::vec2(10.0f,40.0f)),
        pairGlm(glm::vec2(10.0f,20.0f),glm::vec2(20.0f,20.0f))
        };
    room = new Room(objectsContainer,excRoomsContainer,glm::vec2(10.0f,20.0f),glm::vec2(20.0f,40.0f),escRooms6);
    //roomsContainer.push_back(room);
    excRoomsContainer.insert(std::end(excRoomsContainer),std::begin(escRooms6),std::end(escRooms6));

    //HALL3
    vector<pairGlm> escRooms7{
        pairGlm(glm::vec2(0.0f,50.0f),glm::vec2(0.0f,40.0f)),
        pairGlm(glm::vec2(10.0f,40.0f),glm::vec2(20.0f,40.0f))
        };
    room = new Room(objectsContainer,excRoomsContainer,glm::vec2(0.0f,40.0f),glm::vec2(20.0f,50.0f),escRooms7);
    //roomsContainer.push_back(room);
    excRoomsContainer.insert(std::end(excRoomsContainer),std::begin(escRooms7),std::end(escRooms7));

    //HALL4
    vector<pairGlm> escRooms8{
        pairGlm(glm::vec2(20.0f,10.0f),glm::vec2(20.0f,0.0f)),
        pairGlm(glm::vec2(30.0f,0.0f),glm::vec2(40.0f,0.0f)),
        pairGlm(glm::vec2(40.0f,10.0f),glm::vec2(30.0f,10.0f))
        };
    room = new Room(objectsContainer,excRoomsContainer,glm::vec2(20.0f,0.0f),glm::vec2(40.0f,10.0f),escRooms8);
    //roomsContainer.push_back(room);
    excRoomsContainer.insert(std::end(excRoomsContainer),std::begin(escRooms8),std::end(escRooms8));

    //HALL5
    vector<pairGlm> escRooms9{
        pairGlm(glm::vec2(30.0f,10.0f),glm::vec2(40.0f,10.0f)),
        pairGlm(glm::vec2(40.0f,40.0f),glm::vec2(40.0f,50.0f))
        };
    room = new Room(objectsContainer,excRoomsContainer,glm::vec2(30.0f,10.0f),glm::vec2(40.0f,50.0f),escRooms9);
    room->exits[0]->mainExit = true;
    //roomsContainer.push_back(room);
    excRoomsContainer.insert(std::end(excRoomsContainer),std::begin(escRooms9),std::end(escRooms9));

}
