#ifndef POLYGON2_H
#define POLYGON2_H
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include <math.h>


class Polygon2
{
    public:
        Polygon2();
        virtual ~Polygon2();
        void draw();
    protected:
    private:
        float s = 1;
        float h;
        float r;
        float b;
        float a;
        float x;
        float y;
};

#endif // POLYGON2_H
