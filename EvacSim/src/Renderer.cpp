#include "Renderer.h"


Renderer::Renderer()
{
    width = 800;
    height = 600;
    (!initSDL())? std::cout << "Initialization SDL2 OK!" << std::endl : std::cout << "Initialization SDL2 ERROR!" << std::endl ;
    initOpenGL();
    ratio = 0;
    last_time = 0;

}

Renderer& Renderer::getInstance()   // singletone pattern
{
    static Renderer instance;
    return instance;
}

Renderer::~Renderer()
{
    SDL_GL_DeleteContext(glcontext);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Renderer::setObjectsContainer(ObjectsContainer *objContainer)
{
    objectsContainer = objContainer;
}

void Renderer::consoleToggleVisibility()
{

}

void Renderer::calcTime()
{
    unsigned int curr_time;

    do
    {
        curr_time = SDL_GetTicks();
        Sleep(0);
    }
    while ((curr_time - last_time)/1000.0f < 1.0f/121);             // fps max = 100 //fix

    ratio = (double)(curr_time - last_time) / 1000.0f;

    last_time = curr_time;
}



void Renderer::drawAll()
{
    calcTime();
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);         // Clear The Screen And The Depth Buffer
    glColor4f (1,1,1,1);
    glLoadIdentity();
    b2Vec2 pos = objectsContainer->getObjectByIndex(1)->body->GetPosition();
    glTranslatef(-pos.x,-pos.y,-30);  //minus variables because bodies position is inverted against to opengl map

    drawAxis(0,0,0);
    glColor3f( 0.0, 1.0, 1.0 );
    for(auto it : objectsContainer->getContainer())
    {
        if(it.second->drawable==true)
            it.second->draw();
    }
    glPopMatrix();
    glColor3f( 0.0, 1.0, 1.0 );
    glLoadIdentity();
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, -15.0f);
    SDL_GL_SwapWindow(window);
}


void Renderer::initOpenGL()
{

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);						// Black Background
    glClearDepth(1.0f);											// Depth Buffer Setup
    glDepthFunc(GL_LESS);										// Type Of Depth Testing
    glEnable(GL_DEPTH_TEST);									// Enable Depth Testing
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);			// Enable Alpha Blending
    glEnable(GL_BLEND);											// Enable Blending
    glAlphaFunc(GL_GREATER,0.1f);								// Set Alpha Testing
    glEnable(GL_ALPHA_TEST);									// Enable Alpha Testing
    //glEnable(GL_TEXTURE_2D);									// Enable Texture Mapping
    //glEnable(GL_CULL_FACE);									    // Remove Back Face
    glShadeModel(GL_SMOOTH);
    glViewport(0, 0, width, height);							// Ustawienie pola rzutowania (wsp. okna = (0,0) do (Width, Height))
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();											// Reset The Projection Matrix
    gluPerspective(90.0f,(GLfloat)width/(GLfloat)height,0.1f,330.0f); // Obliczanie parametróe rzutowanie (ciekawa funcja, zobaczcie do dokumentacji w necie)
    glMatrixMode(GL_MODELVIEW);									// Rysowanie czas zacz¹æ :)

}

bool Renderer::initSDL()
{
    // m0rf: nie bêdê opisywa³ tej funkcji, napisa³em j¹ zgodnie z wytycznymi na wikipedii SDLowskiej
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
        return 1;
    }


    // Create a window. Window mode MUST include SDL_WINDOW_OPENGL for use with OpenGL.
    window = SDL_CreateWindow(
                 "MOKA GAME",
                 SDL_WINDOWPOS_UNDEFINED,
                 SDL_WINDOWPOS_UNDEFINED,
                 width,
                 height,
                 SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE
             );

    if(window==nullptr) 		// In the event that the window could not be made...
    {
        printf("Could not create window: %s\n", SDL_GetError());
        return 1;
    }

    // Create an OpenGL context associated with the window.
    glcontext = SDL_GL_CreateContext(window);
    if(glcontext==nullptr) 		// In the event that the window could not be made...
    {
        printf("Could not create glcontext: %s\n", SDL_GetError());
        return 1;
    }
    // if(SDL_SetRelativeMouseMode(SDL_TRUE)<0){ // przechwycenie kursora przez okno = dostêpne tylko wzglêdne przesuniêcia
    // fprintf(stderr, "Unable to set Relative Mouse Mode: %s\n", SDL_GetError());
    // }
    SDL_ShowCursor(SDL_ENABLE);
    SDL_SetRelativeMouseMode(SDL_FALSE);
    return 0;
}


void Renderer::drawAxis (float x = 1.0f,float y = 1.0f,float z = 1.0f)
{
    glPushMatrix ();

    glTranslatef (x, y, z);					// trochê dalej od pocz¹tku uk³adu...
    glScalef     (2.25, 2.25, 2.25);					// powiêksz
    glLineWidth (1.0f);									// bruboœæ lini na 1px;
    glBegin (GL_LINES);									// rysowanie lini
    glColor3f (1,0,0);
    glVertex3f (0,0,0);
    glVertex3f (1,0,0);    // X axis is red.
    glColor3f (0,1,0);
    glVertex3f (0,0,0);
    glVertex3f (0,1,0);    // Y axis is green.
    glColor3f (0,0,1);
    glVertex3f (0,0,0);
    glVertex3f (0,0,1);    // z axis is blue.
    glEnd();
    glPopMatrix ();

}

void Renderer::printSupportedExtensions()
{

}


void Renderer::DrawQuad(float x, float y, float w, float h)
{
    w /= 2.0f;
    h /= 2.0f;

    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(x - w, y - h,-10);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(x + w, y - h,-10);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(x + w, y + h,-10);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(x - w, y + h,-10);
    glEnd();
}

void Renderer::DrawQuadRGBA(float x, float y, float w, float h, float r, float g, float b, float a)
{
    // Get old color.
    float current_color[4];
    glGetFloatv(GL_CURRENT_COLOR, current_color);

    // Set new color and draw quad.
    glColor4f(r, g, b, a);
    DrawQuad(x, y, w, h);

    // Set old color.
    glColor4fv(current_color);
}

void Renderer::DrawQuadTexture(float x, float y, float w, float h, unsigned int texture_id)
{
    // Enable texturing if needed.
    bool texturing_enabled = glIsEnabled(GL_TEXTURE_2D);
    if(!texturing_enabled)
        glEnable(GL_TEXTURE_2D);

    // Bind texture and draw.
    glBindTexture(GL_TEXTURE_2D, texture_id);
    DrawQuad(x, y, w, h);

    // Disable if was disable.
    if(!texturing_enabled)
        glDisable(GL_TEXTURE_2D);
}
void Renderer::drawPoint(float x, float y)
{

    glTranslatef(x,y,0);
    glBegin(GL_POINT);
    glColor3f (0,1,0);
    glPointSize(3);
    glVertex3f(x,y,-9);
    glEnd();

}
void Renderer::DrawQuadTexture(float x, float y, float w, float h,float angle, unsigned int texture_id)
{
    glPushMatrix ();
    w /= 2.0f;
    h /= 2.0f;
    bool texturing_enabled = glIsEnabled(GL_TEXTURE_2D);
    if(!texturing_enabled)
        glEnable(GL_TEXTURE_2D);
    glColor3f (1,1,1);
    glTranslatef(x,y,0);
    glRotatef(RADTODEG(angle), 0.0f, 0.0f, 1.0f);
    glTranslatef(0,0,0);
    // Bind texture and draw.
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(0 - w, 0 - h,0.1f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(0 + w, 0 - h,0.1f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(0 + w, 0 + h,0.1f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(0 - w, 0 + h,0.1f);

 glEnd();

    // Disable if was disable.
    if(!texturing_enabled)
        glDisable(GL_TEXTURE_2D);

    glPopMatrix ();
}
