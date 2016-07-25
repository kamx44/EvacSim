#ifndef WORLDMESH_H
#define WORLDMESH_H

#include "Polygony.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <math.h>





class WorldMesh
{
public:
    Polygony **pol;
    WorldMesh();
    WorldMesh(int polSize);
    virtual ~WorldMesh();
    void drawPolygon(float,float,float);

protected:
private:
    float dx;
    float dy;

};

#endif // WORLDMESH_H
