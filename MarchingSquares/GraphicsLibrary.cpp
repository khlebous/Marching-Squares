#include"GraphicsLibrary.h"
#include <algorithm>  
#include "GLFW\glfw3.h"

extern int SCR_WIDTH;
extern int SCR_HEIGHT;

void GL::Draw(vector<Line> lines)
{
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(0.0, SCR_WIDTH, 0.0, SCR_HEIGHT, 0.0, 1.0);

	glColor3f(1.0, 0.0, 0.0);
	glLineWidth(3);
	glBegin(GL_POINTS);
	glBegin(GL_LINES);

	for (Line &l : lines) 
	{
		glVertex2i(l.A.y, l.A.x);
		glVertex2i(l.B.y, l.B.x);
	}
	glEnd();
}
