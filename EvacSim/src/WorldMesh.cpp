#include "WorldMesh.h"



WorldMesh::WorldMesh()
{
    pol = new Polygony*[300];
    for(int i=0; i<300; i++)
    {
        pol[i] = new Polygony[300];
    }
}

WorldMesh::~WorldMesh()
{
    //dtor
}

void WorldMesh::drawPolygon(float x = 1.0f,float y = 1.0f,float z = 1.0f)
{
    dy=0;
    dx=0;
    bool pom = true;
    for(int i=0; i<30; i++)
    {
        for(int j=0; j<30; j++)
        {
            glPushMatrix ();
            glScalef(22,22,1);
            glTranslatef (x+dx, y+dy, -15.0f);
            pol[i][j].draw();
            dx+=sqrt(3);
            glPopMatrix ();
        }
        if(pom)
        {
            dx=sqrt(3)/2;
            pom=false;
        }
        else
        {
            dx=0;
            pom=true;
        }
        dy+=1.5;
    }

}
