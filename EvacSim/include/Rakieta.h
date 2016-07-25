#ifndef RAKIETA_H
#define RAKIETA_H

#include "glm/common.hpp"


class Rakieta
{
    public:
        Rakieta();
        virtual ~Rakieta();
    protected:
    private:
       // Image rakieta_img;
        unsigned int myID;
        glm::vec2 position;
        glm::vec2 velocity;
        glm::vec2 accuracy;
        glm::vec2 rotation;
        float max_speed;
};

#endif // RAKIETA_H
