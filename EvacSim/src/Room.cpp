#include "Room.h"
#include "Wall.h"
#include "Exit.h"
#include "ObjectsContainer.h"
#include "Sector.h"
#include <iostream>

using namespace std;

Room::Room(ObjectsContainer* objectsContainer,std::vector<pair<glm::vec2,glm::vec2>> excRoomsContainer,glm::vec2 leftDownCorner,glm::vec2 rightUpCorner,vector<pair<glm::vec2,glm::vec2>> escapePoints)
{
    this->objectsContainer = objectsContainer;
    this->excRoomsContainer = excRoomsContainer;
    vertices = new glm::vec2[4];
    this->vertices[1] = leftDownCorner;
    this->vertices[3] = rightUpCorner;

    this->escapePoints = &escapePoints;
    createBoundaries();
    createSectors();
}

Room::~Room()
{
    //dtor
}

void Room::createBoundaries(){
    vertices[0].x = vertices[3].x;
    vertices[0].y = vertices[1].y;
    vertices[2].x = vertices[1].x;
    vertices[2].y = vertices[3].y;
    int i=0;
    int j=0;
    while(i<4){
        if(i==3)
            j=0;
        else
            j=i+1;
            createWall(vertices[i],vertices[j],0);
        i++;
    }
    for(vector<int>::size_type i=0;i<(*escapePoints).size();i++){
        if(checkIfEscapeExist((*escapePoints)[i].first,(*escapePoints)[i].second)){
            Exit* exit = new Exit((*escapePoints)[i].first,(*escapePoints)[i].second);
            b2Vec2 mid = exit->body->GetPosition();//exit->getMiddlePoint();
            cout<<"Exit: "<<mid.x<<" "<<mid.y<<endl;//";;"<<(*escapePoints)[i].second.x<<" "<<(*escapePoints)[i].second.y<<endl;
            objectsContainer->addObject(exit);
            exits.push_back(exit);
        }
    }
}

int Room::createWall(glm::vec2 point1, glm::vec2 point2, vector<int>::size_type index){
    cout<<"weszlo"<<endl;
    bool found=false;
    vector<int>::size_type i = index;
    while(i<(*escapePoints).size() && !found){
        if(checkIfBetween(point1,point2,(*escapePoints)[i].first)&&checkIfBetween(point1,point2,(*escapePoints)[i].second)){
            int escapeNr = checkWhichCloser(point1,(*escapePoints)[i].first,(*escapePoints)[i].second); //wartosc bezwzgledna wyniku
            if(escapeNr==1){
                createWall(point1,(*escapePoints)[i].first,i);
                createWall(point2,(*escapePoints)[i].second,i);
                found=true;
            }
            else if(escapeNr==2){
                createWall(point2,(*escapePoints)[i].first,i);
                createWall(point1,(*escapePoints)[i].second,i);
                found=true;
            }
            else{
                throw "ERROR: wrong escape coordinates";
            }
            //escape(escapePointOne,escapePointTwo)
        }else{
            found=false;
        }
        i++;
    }
    if(!found){

        Wall *wall = new Wall(point1,point2);
        objectsContainer->addObject(wall);
    }
    return 0;
}

bool Room::checkIfBetween(glm::vec2 point1, glm::vec2 point2, glm::vec2 escapePoint){
    if(point1.x==point2.x){
        if(escapePoint.x!=point1.x){
            return false;
        }else{
            if(point1.y<point2.y){
                if(escapePoint.y<=point2.y&&escapePoint.y>=point1.y)
                    return true;
                else
                    return false;
            } else{
                if(escapePoint.y<=point1.y&&escapePoint.y>=point2.y)
                    return true;
                else
                    return false;
            }
        }
    }else{
        if(escapePoint.y!=point1.y){
            return false;
        }else{
            if(point1.x<point2.x){
                if(escapePoint.x<=point2.x&&escapePoint.x>=point1.x)
                    return true;
                else
                    return false;
            } else{
                if(escapePoint.x<=point1.x&&escapePoint.x>=point2.x)
                    return true;
                else
                    return false;
            }
        }
    }
}

int Room::checkWhichCloser(glm::vec2 point, glm::vec2 escapePointOne, glm::vec2 escapePointTwo){
    if(point.x==escapePointOne.x){
        if(abs(point.y-escapePointOne.y)<abs(point.y-escapePointTwo.y))
            return 1;
        else
            return 2;
    }else if(point.y==escapePointOne.y){
        if(abs(point.x-escapePointOne.x)<abs(point.x-escapePointTwo.x))
            return 1;
        else
            return 2;
    }
    return 0;
}

void Room::createSectors(){
    for(int i = static_cast<int>(vertices[0].y);i<static_cast<int>(vertices[3].y);i+=10){
        for(int j = static_cast<int>(vertices[0].x);j>static_cast<int>(vertices[1].x);j-=10){
            sectors.push_back(new Sector(glm::vec2(j-10,i),glm::vec2(j,i+10)));
        }
    }
}

Sector* Room::getFreeSector(){
    if(!sectors.empty()){
        Sector* tmpSector = sectors.at(sectors.size()-1);
        sectors.pop_back();
        return tmpSector;
    }
    else
        return nullptr;
}

bool Room::checkIfEscapeExist(glm::vec2 point1,glm::vec2 point2){
    for(auto exit : excRoomsContainer){
        if(((exit.first == point1) && (exit.second == point2)) || ((exit.first == point2) && (exit.second == point1)) )
            return false;
    }
    return true;
}
