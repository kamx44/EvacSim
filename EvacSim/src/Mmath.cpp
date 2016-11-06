#include "Math.h"

#include <math.h>
#include <ctime>
#include <stdlib.h>
#include <stdio.h>
#include <glm/glm.hpp>
#include <Box2D/Box2D.h>



/********************************************//**
 * \brief srand module which allow to randomize numbers
 *
 * \return void
 * #kamx
 ***********************************************/
void initSrand()
{
    static int srand_lock = 0;
    if(srand_lock != 1)
#ifdef DEBUG_MODE
        srand(0);
#else
        srand (static_cast <unsigned> (time(0)));
#endif // DEBUG_MODE
    srand_lock = 1;
}

/********************************************//**
 * \brief The function will generate a vector of floats from some arbitrary LO to some arbitrary HI
 *
 * \param LO start float
 * \param HI high border?
 * \return Returne random glm::vec2
 * #kamx
 ***********************************************/
glm::vec2 getRandomVec2 (float LO, float HI)
{
    float x = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));
    float y = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));
    return glm::vec2(x,y);
}

/********************************************//**
 * \brief This function create glm vector from vector from box2d library
 *
 * \param b2Vec2 vec (vector from box2d library)
 * \return glm::vec2 (vector from glm library)
 * #kamx
 ***********************************************/
glm::vec2 createGlmVec(b2Vec2 vec)
{
    float x = vec.x;
    float y = vec.y;
    return glm::vec2(x,y);
}

/********************************************//**
 * \brief This function create b2Vec2 box2d library vector from glm vector
 *
 * \param glm::vec2 vec (vector from glm library)
 * \return b2Vec2 vec (vector from box2d library)
 * #kamx
 ***********************************************/
b2Vec2 createB2vec2(glm::vec2 vec)
{
    float x = vec.x;
    float y = vec.y;
    return b2Vec2(x,y);
}

/********************************************//**
 * \brief This function create random float between two variables LO HI (should be template)
 *
 * \param float LO (which is lowest possible number)
 * \param float HI (which is highest possible number)
 * \return random float number
 * #kamx
 ***********************************************/
float getRandomFloat (float LO, float HI)
{
    float x = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));
    return x;
}

/********************************************//**
 * \brief This function create random int between two variables LO HI (should be template)
 *
 * \param int LO (which is lowest possible number)
 * \param int HI (which is highest possible number)
 * \return random int number
 * #kamx
 ***********************************************/
int getRandomInt (int LO, int HI)
{
    initSrand();
    float x = LO + static_cast <int> (rand()) /( static_cast <int> (RAND_MAX/(HI-LO)));
    return x;
}

/********************************************//**
 * \brief This function calculating diagonal from two variables
 *
 * \param float x
 * \param float y
 * \return diagonal from two variables
 * #kamx
 ***********************************************/
float diagonal(float x,float y)
{
    return sqrt((x*x)+(y*y));
}

b2Vec2 normalize(b2Vec2 vec){
    float diag = diagonal(vec.x,vec.y);
    if(diag>0){
        vec.x = vec.x/diag;
        vec.y = vec.y/diag;
    }
    return vec;
}

b2Vec2 multiplyB2Vec2(b2Vec2 vec, float mul){
    vec.x *= mul;
    vec.y *= mul;
    return vec;
}




