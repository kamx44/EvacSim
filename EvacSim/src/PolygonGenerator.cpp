#include "PolygonGenerator.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <cstddef>        // std::size_t
#include <stdlib.h>
#include <Box2D/Box2D.h>

using namespace std;

PolygonGenerator::PolygonGenerator()
{

}

PolygonGenerator::~PolygonGenerator()
{
    //dtor
}

vector<std::vector<b2Vec2*>> PolygonGenerator::getPolygonContainer(string fileName)
{
    fstream plik;
    plik.open( "data/pol/"+fileName, std::ios::in | std::ios::out );
    if( plik.good() == true )
    {
        cout << "Uzyskano dostep do pliku!" << endl;
        string napis;
        for(int i=0; i<10; i++)
            getline( plik, napis );
        //density
        getline(plik,napis);
        cout << napis << endl;
        density = findOneNumber(napis);
        cout<<"density: "<<density<<endl;
        //friction
        getline(plik,napis);
        cout << napis << endl;
        friction = findOneNumber(napis);
        cout<<"friction: "<<friction<<endl;
        //restitution
        getline(plik,napis);
        cout << napis << endl;
        restitution = findOneNumber(napis);
        cout<<"restitution: "<<restitution<<endl;
        while( !plik.eof() )
        {
            getline(plik,napis);
            cout << napis << endl;
            size_t f = napis.find("<polygon>",0);
            if(f!=string::npos)
            {
                //int verticesSize = findComma(napis);
                std::vector<b2Vec2*> vertices=findVertices(napis);
                polygon_container.push_back(vertices);
            }
        }
        plik.close();
    }
    else
        cout << "Dostep do pliku zostal zabroniony!" << endl;
    return polygon_container;
}

float PolygonGenerator::findOneNumber(string napis)
{
    std::size_t first = napis.find_first_of("1234567890");
    std::size_t last = napis.find_last_of("1234567890");
    string x;
    if(first!=string::npos&&last!=string::npos)
    {
        if(first==last)
        {
            char s1 = napis[first];
            x = string(1,s1);
            float y=atof(x.c_str());
            return y;
        }
        else if(first<last)
        {
            for(size_t i = first; i<=last; i++)
            {
                char s1 = napis[i];
                if(i==first)
                    x = string(1,s1);
                else
                    x+=s1;
            }
            float y=atof(x.c_str());
            return y;
        }
        else
        {
            throw("cosik nie tak");
        }
    }
    return 0;
}

int PolygonGenerator::findComma(string& napis)
{
    size_t mark=0;
    int counter = 0;
    while(mark!=string::npos)
    {
        size_t temp = mark;
        mark = napis.find(",",temp);
        if(mark!=string::npos)
            napis.erase(mark,1);
        counter++;
    }
    return counter/2;
}

std::vector<b2Vec2*> PolygonGenerator::findVertices(string napis)
{
    std::size_t first = napis.find_first_of("1234567890");
    std::size_t last = napis.find_last_of("1234567890");
    string numb = "";
    bool x=true;
    std::vector<b2Vec2*> vertices;
    b2Vec2* vec;
    for(size_t i = first; i<=last+1; i++)
    {
        char s1 = napis[i];
        char prev=napis[i-1];
        if((s1!=' ')&&(s1!='<'))
        {
            numb +=  s1;
        }
        else if(prev!=' ')
        {
            if(x==true)
            {
                vec = new b2Vec2();
                vec->x=atof(numb.c_str())/10;
                x=false;
            }
            else
            {
                vec->y=atof(numb.c_str())/10;
                x=true;
                vertices.push_back(vec);
            }
            numb = "";
        }
    }
    return vertices;
}
