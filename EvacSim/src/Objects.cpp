#include "Objects.h"
#include <Image.h>
#include <Renderer.h>
#include <Object.h>

Objects::Objects()
{

}

Objects::~Objects()
{
    //dtor
}

void Objects::addObject(unsigned int ID,float x,float y,float w,float h)
{

    unsigned int j=ID;
    cout<<"dodawanie"<<ID<<endl;
    pomTab = new float[4];
    pomTab[0]=x;
    pomTab[1]=y;
    pomTab[2]=w;
    pomTab[3]=h;
    objectsMap.emplace(j,pomTab);



}

void Objects::removeObject(unsigned int ID,Image img)
{
//    objectsMap.erase(ID,img);
}

void Objects::drawAll()
{

    unsigned int id;
    for ( auto it = objectsMap.begin(); it != objectsMap.end(); ++it )
    {
        cout<<"kupka"<<endl;
        id = it->first;
        cout<<id<<endl;
        pomTab2 = it->second;
        px=pomTab[0];
        py=pomTab[1];
        pw=pomTab[2];
        ph=pomTab[3];
       // switch()
        Renderer::getInstance().DrawQuadTexture(px, py, pw, ph, id);
    }

}
