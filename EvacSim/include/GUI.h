#ifndef GUI_H
#define GUI_H

#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/OpenGL/GLRenderer.h>
#include <iostream>
#include "Events.h"

class GUI
{
    public:
        GUI();
        virtual ~GUI();

        int init();
        int createTest();
        void renderTest();

    protected:
    private:
    CEGUI::OpenGLRenderer* myRenderer;
        CEGUI::DefaultResourceProvider* rp;
        CEGUI::MouseCursor* mouseC;
        CEGUI::FrameWindow* fWnd;

};

#endif // GUI_H
