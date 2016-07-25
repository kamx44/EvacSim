#ifndef EVENTS_H
#define EVENTS_H
#include <SDL2/SDL.h>
#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/OpenGL/GLRenderer.h>

class Events
{
public:
    Events();
    virtual ~Events();
    static Events& getInstance();

    int checkEvents();
    int mouseX, mouseY;
    bool keypadKeys[512];
protected:
private:
};

#endif // EVENTS_H
