#ifndef ROOM_H
#define ROOM_H
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <vector>
#include "Sector.h"

using std::vector;
using std::pair;

class ObjectsContainer;

class Room
{
    public:
        Room(ObjectsContainer* objectsContainer,glm::vec2 leftDownCorner,glm::vec2 rightUpCorner,vector<pair<glm::vec2,glm::vec2>> escapePoints);
        virtual ~Room();
        void createBoundaries();
        void createSectors();
        Sector* getFreeSector();
        bool checkIfBetween(glm::vec2 point1, glm::vec2 point2, glm::vec2 escapePoint);
        int checkWhichCloser(glm::vec2 point, glm::vec2 escapePointOne, glm::vec2 escapePointTwo);
        int createWall(glm::vec2 point1, glm::vec2 point2, vector<int>::size_type index);

    protected:
    private:
        glm::vec2 *vertices;
        vector<Sector*> sectors;
        vector<pair<glm::vec2,glm::vec2>> *escapePoints;
        ObjectsContainer* objectsContainer;
};

#endif // ROOM_H
