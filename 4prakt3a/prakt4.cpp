/******************************
* Praktikum 3a - Computer Grafik I 
* HTW-Dresden
* Karl Adler
* s62943
* 
* Auf Ubunutu compilieren mit:
* g++ prakt4.cpp -lglut -lGL -lGLU -lGLEW -o prakt3
* 
*******************************/
#include <iostream>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <math.h>
#include <GL/glut.h> //added GL prefix for Ubuntu compatibility

#define WHEEL 1

using namespace std;

static void myinit(void);
static void RenderScene(void);
static void myReshape(int w, int h);
static void initwheel();
	   
static void myinit(void)
{   
	printf( "OpenGL renderer:                  |%s|\n", (char*)glGetString( GL_RENDERER));
	printf( "OpenGL driver version:            |%s|\n", (char*)glGetString( GL_VERSION));		

    /* Initializations */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();		
    glClearColor (1, 1, 1, 0);	//Hintergrundfarbe
    initwheel();
}

static void initwheel(){
	glNewList(WHEEL, GL_COMPILE);
		glColor3f(0.0,0.0,0.0);
		float x = 0;
		float y = 0;
		float r = 20;
		int segments = 40;
		//the circle
		glBegin( GL_LINE_LOOP );
		for( int n = 0; n <= segments; ++n ) {
			float const t = 2*M_PI*(float)n/(float)segments;
			glVertex2f(x + sin(t)*r, y + cos(t)*r);
		}
		glEnd();
		//the spokes
		glBegin( GL_LINES );
		for( int n = 0; n <= segments; ++n ) {
			float const t = 2*M_PI*(float)n/(float)segments;
			glVertex2f(x, y);
			glVertex2f(x + sin(t)*r, y + cos(t)*r);
		}
		glEnd();
			
	glEndList();
}

static void RenderScene(void)
{
	float x=0.0;
	for(int i = 2*360; i > 0; i--){	//zwei runden
    /* Creating the Scene */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);   
    glColor3f(0,0,0.5);		  /* Farbe festlegen */
    //Modell transformation:
    glMatrixMode( GL_MODELVIEW ); 	// Modell-Matrixstack wählen
    glPushMatrix();					// aktuelel Matrix speichern
	glLoadIdentity();             	// Die Matrix zurücksetzen
	float ankle = i%360;
		glTranslatef(x,0.0,0.0);
		glRotatef(ankle,0,0,1);
		if(i<180){
			x++;			
		}
		
		glCallList(WHEEL);
		glPopMatrix();			
		glutSwapBuffers();
	}
	
    //glFlush ();		  /* Flushes OpenGL command queues and buffers */
}

static void myReshape(int w, int h)
{
    /* Callback function to handle window dimensions and size changes */
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);	/* specifies the current matrix */
    glLoadIdentity();			/* Sets the currant matrix to identity */
    gluOrtho2D(-20,100.0,-20.0,100.0);	/* Sets the clipping rectangle extends */
}

/*  Main Loop */

int main(int argc, char** argv)
{
    
    /* GLUT functions to set up the window */
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_ALPHA | GLUT_DEPTH);
    glutInitWindowPosition (0, 0);
	glutInitWindowSize(600, 600);
	glutInit(&argc, argv);
    glutCreateWindow (argv[0]);
    
    myinit();			/* Initialization */

    glutReshapeFunc (myReshape); /* Function to call when window changes size */
    glutDisplayFunc(RenderScene); /* Creating the Szene */
    glutMainLoop();
	return 0;
}
