#ifndef MAINDIRECTION_H
#define MAINDIRECTION_H
#include "World.h"
#include "iostream"
#include "set"

class MainDirection
{
    public:
        MainDirection();
        virtual ~MainDirection();
        unsigned int id;
        b2Vec2 position;
        bool passed;
        bool isMainExit;
        void setParameters(unsigned int, b2Vec2, bool);
        bool isPassed(std::set<unsigned int> passedExits);
    protected:

    private:
};

#endif // MAINDIRECTION_H
