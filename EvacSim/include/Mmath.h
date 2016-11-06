#ifndef SUPER_MATH_LIB
#define SUPER_MATH_LIB

#include <glm/glm.hpp>
#include <Box2D/Box2D.h>

#define DEGTORAD(degree) ((degree) * (3.141592654f / 180.0f))
#define RADTODEG(radian) ((radian) * (180.0f / 3.141592654f))

void initSrand();
glm::vec2 getRandomVec2 (float LO, float HI);
glm::vec2 createGlmVec(b2Vec2 vec);
b2Vec2 createB2vec2(glm::vec2 vec);
float getRandomFloat (float LO, float HI);
int getRandomInt (int LO, int HI);
float diagonal(float x,float y);
int getRandomId();
b2Vec2 normalize(b2Vec2 vec);
b2Vec2 multiplyB2Vec2(b2Vec2 vec, float mul);
inline void swapf(float& a, float& b)
{
    float c = a;
    a = b;
    b = c;
}

inline float clampf(float x, float min, float max)
{
    return (x < min)? min : (x > max)? max : x;
}
inline float cross(const glm::vec2& a, const glm::vec2& b)
{
    return ((a[0] * b[1]) - (a[1] * b[0]));
}
inline float wrapf(float x, float min, float max)
{
    return (x < min)? (x - min) + max : (x > max)? (x - max) + min : x;

}

#endif // SUPER_MATH_LIB
