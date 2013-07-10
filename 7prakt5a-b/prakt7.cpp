/******************************
* Praktikum 5a - Computer Grafik I 
* HTW-Dresden
* Karl Adler
* s62943
* 
* Auf Ubunutu compilieren mit:
* g++ prakt7.cpp -lglut -lGL -lGLU -lGLEW -o prakt7
* 
*******************************/
#include <iostream>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <math.h>
#include <time.h> 
#include <GL/glut.h> //added GL prefix for Ubuntu compatibility

#define MAXP 10

GLint nNumPoints = 0;

GLfloat ctrlpoints[10][3];
GLdouble mouseOgl[3] = {0.0,0.0,0.0};
GLint mouseX, mouseY;
bool mousePressed = false;
void GetOGLPos(int x, int y);

void init(void)
{
   glClearColor(1.0, 1.0, 1.0, 0.0);
   glShadeModel(GL_FLAT);
    // Enable the evaluator
    glEnable(GL_MAP1_VERTEX_3);
    glEnable(GL_DEPTH);
}

void display(void)
{
   int i;
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glColor3f(0.0, 0.0, 1.0);
   glLoadIdentity();   
	if(mousePressed && nNumPoints < MAXP){
		GetOGLPos(mouseX, mouseY);
		std::cout
			<< mouseOgl[0] << " # "
			<< mouseOgl[1] << " # "
			<< mouseOgl[2] << " # "
			<< std::endl;	
		nNumPoints++;
		ctrlpoints[nNumPoints-1][0] = mouseOgl[0];
		ctrlpoints[nNumPoints-1][1] = mouseOgl[1];
		//ctrlpoints[nNumPoints-1][2] = mouseOgl[2];
		ctrlpoints[nNumPoints-1][2] = 0;
	}
	//Kurven
	if( nNumPoints == MAXP ){
		glMap1f(GL_MAP1_VERTEX_3,	// Type of data generated
			0.0f,						// Lower u range
			1.0f,						// Upper u range
			3,							// Distance between points in the data 3: ...Z-X-Y-Z...
			nNumPoints,					// number of control points
			&ctrlpoints[0][0]);			// start point
		glLineWidth(1.0);
		glBegin(GL_LINE_STRIP);
			glColor3f(0.0, 0.0, 1.0);
			float max = pow(MAXP,2);	//accuracy of pint calulation
			for (i = 0; i <= max; i++) 
				glEvalCoord1f((GLfloat) (i/max));	//high value to avoid gaps?!?
		glEnd();
	}
	//Kontrollpunkte:
	glPointSize(5.0);
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_POINTS);
		for (i = 0; i < nNumPoints; i++) 
			glVertex3fv(&ctrlpoints[i][0]);
	glEnd();
	//Linien
	glLineWidth(1.0);
	glColor3f(0.0, 1.0, 0.0);
	glBegin(GL_LINE_STRIP);
		for (i = 0; i < nNumPoints; i++) 
			glVertex3fv(&ctrlpoints[i][0]);
	glEnd();
	if( nNumPoints == MAXP ){nNumPoints = 0;}
   glFlush();
}

void reshape(int w, int h)
{
   glViewport(0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   //keep aspect ratio:
   if (w <= h)
      glOrtho(-2.0, 2.0, -2.0*(GLfloat)h/(GLfloat)w, 
               2.0*(GLfloat)h/(GLfloat)w, -2.0, 2.0);
   else
      glOrtho(-2.0*(GLfloat)w/(GLfloat)h, 
               2.0*(GLfloat)w/(GLfloat)h, -2.0, 2.0, -2.0, 2.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

//handle click events of the mouse
void myMouse(int button, int state, int x, int y)
{
	//mouse coords to gl coords
	
	mouseX = x;
	mouseY = y;
	switch (button)
	{
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_UP){	//on release left mouse button
				std::cout << x << " * "<< y << std::endl;
				mousePressed = true;
				glutPostRedisplay(); //redisplay and calculate gl coords
			} else {	
				mousePressed = false;
			} 
		break;
    }
}
// detailed information: 
// http://nehe.gamedev.net/article/using_gluunproject/16013/
void GetOGLPos(int x, int y)
{
	//init vars:
    GLint viewport[4];			
    GLdouble modelview[16];		
    GLdouble projection[16];	
    GLfloat winX, winY, winZ;	
    GLdouble posX, posY, posZ;
	//get gl specs
    glGetDoublev( GL_MODELVIEW_MATRIX, modelview );		//get Modelmatrix 	
    glGetDoublev( GL_PROJECTION_MATRIX, projection );	//get projection matrix
    glGetIntegerv( GL_VIEWPORT, viewport );				//get viewport values
	//calculate the gl mouseposition
    winX = (float)x;
    winY = (float)viewport[3] - (float)y;
    glReadPixels( x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );
    
    gluUnProject( winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);
	mouseOgl[0] = posX;
	mouseOgl[1] = posY;
	mouseOgl[2] = posZ;
}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
   glutInitWindowSize (600, 600);
   glutInitWindowPosition (100, 100);
   glutCreateWindow (argv[0]);
   
    //  Enable Z-buffer depth test
	glEnable(GL_DEPTH_TEST);
   init ();
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutMouseFunc(myMouse);
   glutMainLoop();
   return 0;
}
