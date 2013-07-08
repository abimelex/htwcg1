/******************************
* Praktikum 3b - Computer Grafik I 
* HTW-Dresden
* Karl Adler
* s62943
* 
* Auf Ubunutu compilieren mit:
* g++ prakt5.cpp -lglut -lGL -lGLU -lGLEW -o prakt3
* 
*******************************/
#include <iostream>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <math.h>
#include <time.h> 
#include <GL/glut.h> //added GL prefix for Ubuntu compatibility

#define WHEEL 1

using namespace std;

static void myinit(void);
static void RenderScene(void);
static void myReshape(int w, int h);
static void initwheel();
void spinDisplay();
//globals:	
static GLfloat spin = 0.0;
static GLfloat x = 0.0;
time_t tstart;
    
static void myinit(void)
{   
	printf( "OpenGL renderer:                  |%s|\n", (char*)glGetString( GL_RENDERER));
	printf( "OpenGL driver version:            |%s|\n", (char*)glGetString( GL_VERSION));		

    /* Initializations */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();		
    glClearColor (1, 1, 1, 0);	//Hintergrundfarbe
    initwheel();
    time(&tstart);	//save start time
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
    /* Creating the Scene */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);   
    glColor3f(0,0,0.5);		  /* Farbe festlegen */
    //Modell transformation:
    glMatrixMode( GL_MODELVIEW ); 	// Modell-Matrixstack wählen
    glPushMatrix();					// aktuelel Matrix speichern
	glLoadIdentity();             	// Die Matrix zurücksetzen
	glTranslatef(x,0.0,0.0);
	glRotatef(spin,0,0,1);
	glCallList(WHEEL);
	glPopMatrix();	
    glFlush ();		  /* Flushes OpenGL command queues and buffers */
}

static void myReshape(int w, int h)
{
    /* Callback function to handle window dimensions and size changes */
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);	/* specifies the current matrix */
    glLoadIdentity();			/* Sets the currant matrix to identity */
    gluOrtho2D(-20,100.0,-20.0,100.0);	/* Sets the clipping rectangle extends */
}

void spinDisplay(void)
{
	time_t tcurrent;
	time(&tcurrent);
	double seconds = difftime(tcurrent,tstart);
	//roll out the wheel after 5 seconds
	if(seconds > 5){
		x += 0.1;
	}
	spin = spin - 0.2;
	spin = fmod(spin,360.0);//if spin > 360 set to spin-360
   
   cout << seconds << endl;
   
   glutPostRedisplay();
}

/*  Main Loop */

int main(int argc, char** argv)
{
    
    /* GLUT functions to set up the window */
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB | GLUT_ALPHA | GLUT_DEPTH);
    glutInitWindowPosition (0, 0);
	glutInitWindowSize(600, 600);
	glutInit(&argc, argv);
    glutCreateWindow (argv[0]);
    
    myinit();			/* Initialization */

    glutReshapeFunc (myReshape); /* Function to call when window changes size */
    glutDisplayFunc(RenderScene); /* Creating the Szene */
    glutIdleFunc(spinDisplay);
    glutMainLoop();
	return 0;
}
