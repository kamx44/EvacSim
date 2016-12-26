#ifndef BUILDING1_H
#define BUILDING1_H
#include "Building.h"

class Building1 : public Building
{
    public:
        Building1(ObjectsContainer* objectsContainer);
        virtual ~Building1();
        void createRooms();

    protected:

    private:
};

#endif // BUILDING1_H
