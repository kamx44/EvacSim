#include "Sector.h"
#include "iostream"

using std::cout;
using std::endl;

Sector::Sector(glm::vec2 leftDownCorner,glm::vec2 rightUpCorner)
{
    counter++;
    vertices = new glm::vec2[4];
    this->vertices[1] = leftDownCorner;
    this->vertices[3] = rightUpCorner;
    vertices[0].x = vertices[3].x;
    vertices[0].y = vertices[1].y;
    vertices[2].x = vertices[1].x;
    vertices[2].y = vertices[3].y;
    //cout<<"Sector "<<counter<<" lC: ("<<leftDownCorner.x<<","<<leftDownCorner.y<<") rC: ("<<rightUpCorner.x<<","<<rightUpCorner.y<<")"<<endl;
}

Sector::~Sector()
{
    //dtor
}

int Sector::getLeftDownCornerX(){
    return static_cast<int>(vertices[1].x);
}

int Sector::getRightUpCornerY(){
    return static_cast<int>(vertices[3].y);
}

int Sector::counter = 0;
