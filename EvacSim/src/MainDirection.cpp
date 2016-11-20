#include "MainDirection.h"

MainDirection::MainDirection()
{
    id = 0;
    position = b2Vec2(0,0);
    passed = false;
    isMainExit = false;
}

void MainDirection::setParameters(unsigned int id, b2Vec2 position, bool passed){
    this->id = id;
    this->position = position;
    this->passed = passed;
}

bool MainDirection::isPassed(std::set<unsigned int> passedExits){
    for(auto passedExit : passedExits){
        if(passedExit == id){
            passed = true;
            return passed;
        }
    }
    return false;
}

MainDirection::~MainDirection()
{
    //dtor
}
