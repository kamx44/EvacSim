#include "Events.h"
#include "Renderer.h"
#include "Camera.h"


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




       /* case SDL_MOUSEBUTTONDOWN:
            switch (event.button.button)
            {
            case SDL_BUTTON_LEFT:
                CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(CEGUI::LeftButton);
                break ;

            case SDL_BUTTON_MIDDLE:
                CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(CEGUI::MiddleButton);
                break ;

            case SDL_BUTTON_RIGHT:
                CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(CEGUI::RightButton);
                break ;
            }
            break ;
        case SDL_MOUSEBUTTONUP:
            switch (event.button.button)
            {
            case SDL_BUTTON_LEFT:
                CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonUp(CEGUI::LeftButton);
                break ;

            case SDL_BUTTON_MIDDLE:
                CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonUp(CEGUI::MiddleButton);
                break ;

            case SDL_BUTTON_RIGHT:
                CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonUp(CEGUI::RightButton);
                break ;
            }
            break;*/
        case SDL_KEYDOWN:
            if(event.key.keysym.sym == SDLK_ESCAPE)
                return false;
            // CEGUI::Key::Scan kc = SDLKeyToCEGUIKey(event.key.keysym.sym);

//            CEGUI::System::getSingleton().getDefaultGUIContext().injectChar(event.key.keysym.sym);
                        if(event.key.keysym.sym < 512)
            keypadKeys[event.key.keysym.sym] = true;
            break;

            // It is difficult to resolve how to manage key press event.
            // Currently its made on two manners, store in static buffer keypadKeys and
            // writeing some public variable from singleton-classes. #morf

            // Edit: Now, we decide to refer keypad buffer #morf

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




