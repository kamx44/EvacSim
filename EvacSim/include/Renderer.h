
#ifndef RENDERER_H
#define RENDERER_H

#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>


//#include <SDL2/SDL_image.h>       // not used yet

#include <vector>
#include <stdio.h>
#include <math.h>

#include <string.h>
#include <iostream>
#include <assert.h>


#define check_gl_error() Renderer::getInstance()._check_gl_error(__FILE__,__LINE__)
#include "Ship.h"

#include "Object.h"
#include "Image.h"
#include "World.h"
#include "ObjectsContainer.h"








// POTRZEBNE MAKRA //
#define DEGTORAD(degree) ((degree) * (3.141592654f / 180.0f))
#define RADTODEG(radian) ((radian) * (180.0f / 3.141592654f))



class Renderer
{
public:

    Renderer();
    ~Renderer();

    static Renderer& getInstance();
    void drawAll();
    void drawAxis (float,float,float);

    double ratio, last_time;

    void printSupportedExtensions();
    void _check_gl_error(const char *file, int line);
    int width, height;

    void drawShip ();
    void drawMissles ();
    void drawAsteroids();
    void drawModules();
    void consoleToggleVisibility();

    void DrawQuad(float x, float y, float w, float h);
    void DrawQuadRGBA(float x, float y, float w, float h, float r, float g, float b, float a);
    void DrawQuadTexture(float x, float y, float w, float h, unsigned int texture_id);
    void DrawQuadTexture(float x, float y, float w, float h,float angle, unsigned int texture_id);
    void drawPoint(float x, float y);
    void setObjectsContainer(ObjectsContainer *objContainer);
    Image statek;


    //      int addObject(Object* aa );
    World* world;


private:

    //WSKAèNIK DO GRY!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    void calcTime();            // obliczanie czasu rysowania klatki + ogranicznik fps
    //bool initGLEW();            // inicjalizacja GLEW - dodatkowe api GL - shadery
    bool initSDL();             // inicjalizacja SDL 2.0.3
    void initOpenGL();          // GL

    SDL_Window *window;
    SDL_GLContext glcontext;


    ObjectsContainer *objectsContainer;


};

#endif // RENDERER_H
