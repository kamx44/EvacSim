#include "Polygon.h"
#include "Mate.h"


Polygon::Polygon()
{


}

void Polygon::draw()
{
    float x0=0;
    float y0=0;
    float x1=0;
    float y1=1;
    float r = 1;

    glPushMatrix ();

						// trochê dalej od pocz¹tku uk³adu...
		glScalef     (2.25, 2.25, 2.25);					// powiêksz
		glLineWidth (1.0f);
		/*								// bruboœæ lini na 1px;
		glBegin (GL_POLYGON_OFFSET_FILL);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		for(int i=0;i<6;i++)
        {
            x = r*sin(DEGTORAD(i*60));
            y = r*cos(DEGTORAD(i*60));
			glColor3f (1,0,0);  glVertex3f (x,y,0);
        }

		glEnd();*/
		glClear(GL_COLOR_BUFFER_BIT);
		glBegin(GL_LINES);
		glVertex2i(100,100);
		glVertex2i(100,300);
		 glVertex2i(400,300);
		  glVertex2i(600,150);
		  glVertex2i(400,100);
		   glEnd();
		   glFlush();

	glPopMatrix ();
}


Polygon::~Polygon()
{
    //dtor
}
