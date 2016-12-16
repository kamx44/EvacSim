#include "Events.h"
#include "Renderer.h"



Events::Events() {}

Events& Events::getInstance()   // singletone pattern
{
    static Events instance;
    return instance;
}

Events::~Events() {}

/********************************************//**
 * \brief This function check all event of the pool, and changes states of the class.
 *
 * \return Returns false when the application has been closed, true in otherwise.
 *
 ***********************************************/

int Events::checkEvents()
{
    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
        case SDL_MOUSEMOTION:
            mouseX = event.motion.x-(Renderer::getInstance().width/2);
            mouseY = event.motion.y-(Renderer::getInstance().height/2);
            break;
        case SDL_KEYDOWN:
            if(event.key.keysym.sym == SDLK_ESCAPE)
                return false;
                        if(event.key.keysym.sym < 512)
            keypadKeys[event.key.keysym.sym] = true;
            break;
        case SDL_KEYUP:
            if(event.key.keysym.sym < 512)
                keypadKeys[event.key.keysym.sym] = 0;

            if(event.key.keysym.sym==SDLK_F1)
                Renderer::getInstance().consoleToggleVisibility();
            break;
        case SDL_QUIT:
            // window [X] press detection
            return false;
        }
    }
    return true;   // all events checked
}




