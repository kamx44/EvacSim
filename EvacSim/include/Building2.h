#ifndef BUILDING2_H
#define BUILDING2_H
#include "Building.h"

class Building2 : public Building
{
    public:
        Building2(ObjectsContainer* objectsContainer);
        virtual ~Building2();
        void createRooms();

    protected:

    private:
};

#endif // BUILDING1_H
