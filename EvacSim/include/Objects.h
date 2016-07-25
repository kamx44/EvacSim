#ifndef OBJECTS_H
#define OBJECTS_H
#include "Image.h"

#include <unordered_map>


class Objects
{
    public:
        Objects();
        virtual ~Objects();
        void addObject(unsigned int,float x,float y,float w,float h);
        void removeObject(unsigned int,Image);
        void drawAll();
    protected:
    private:
        std::unordered_map<unsigned int,float*> objectsMap;
        float* pomTab;
        float* pomTab2;
        float px = 0;
        float py = 0;
        float pw = 0;
        float ph = 0;
};

#endif // OBJECTS_H
