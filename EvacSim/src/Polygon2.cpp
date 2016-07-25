#include "Polygon2.h"
#include "Mmath.h"

Polygon2::Polygon2()
{
    //ctor
}
void Polygon2::draw()
{
    float x0=0;
    float y0=0;
    float x1=0;
    float y1=1;
    float r = 1;



						// trochê dalej od pocz¹tku uk³adu...
		//glScalef     (2.25, 2.25, 2.25);					// powiêksz
		glLineWidth (1.0f);									// bruboœæ lini na 1px;
		glBegin (GL_LINES);
		for(int i=0;i<6;i++)
        {
            x = r*sin(DEGTORAD(i*60));
            y = r*cos(DEGTORAD(i*60));
			glColor3f (1,0,0);  glVertex3f (x,y,0);
        }
		glEnd();

}

Polygon2::~Polygon2()
{
    //dtor
}
