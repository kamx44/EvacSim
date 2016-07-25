#ifndef POLYGON_H
#define POLYGON_H
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include <math.h>





class Polygon
{
    public:
        Polygon();
        virtual ~Polygon();
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

#endif // POLYGON_H
