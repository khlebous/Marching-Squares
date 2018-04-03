#include"GraphicsLibrary.h"
#include <algorithm>  
#include "GLFW\glfw3.h"


void GL::Draw()
{
	glViewport(0, 0, 512, 512);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(0.0, 512, 0.0, 512, 0.0, 1.0);

	glColor3f(1.0, 0.0, 0.0);
	glLineWidth(1);
	glBegin(GL_POINTS);
	glBegin(GL_LINES);
	glVertex2i(0,0);
	glVertex2i(150, 150);
	glVertex2i(10, 20);
	glVertex2i(10, 100);
	glEnd();
}
