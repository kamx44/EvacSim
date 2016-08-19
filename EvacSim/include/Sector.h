#ifndef SECTOR_H
#define SECTOR_H
#include <glm/glm.hpp>

class Sector
{
    public:
        static int counter;
        Sector(glm::vec2 leftDownCorner,glm::vec2 rightUpCorner);
        int getLeftDownCornerX();
        int getRightUpCornerY();
        virtual ~Sector();
    protected:
    private:
        glm::vec2* vertices;
};

#endif // SECTOR_H
