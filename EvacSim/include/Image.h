#ifndef IMAGELOADER_H
#define IMAGELOADER_H

//#include "common_libs.h"

#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_image.h>
#include <GL/glu.h>
#include <stdio.h>
#include <iostream>



//using namespace std;
class Image
{
public:
    Image();
    Image(const char *filename);
    Image(const std::string& posXFilename,
          const std::string& negXFilename,
          const std::string& posYFilename,
          const std::string& negYFilename,
          const std::string& posZFilename,
          const std::string& negZFilename);
    virtual ~Image();

    unsigned int imgToTexture(const char *filename);
    SDL_Surface& imgToSurface(const std::string& filename, GLenum& format);
    unsigned int width, height;
    unsigned int imageID;
    unsigned int sampler;

};

#endif // IMAGELOADER_H
