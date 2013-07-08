/******************************
* Praktikum 1a - Computer Grafik I 
* HTW-Dresden
* Karl Adler
* s62943
* 
* Auf Ubunutu compilieren mit:
* g++ prakt1.cpp -lglut -lGL -lGLU -lGLEW -o prakt1
* 
*******************************/
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h> //added GL prefix for Ubuntu compatibility


   
static void myinit(void)
{   
	printf( "OpenGL renderer:                  |%s|\n", (char*)glGetString( GL_RENDERER));
	printf( "OpenGL driver version:            |%s|\n", (char*)glGetString( GL_VERSION));	

    /* Initializations */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();		
    glClearColor (1, 1, 1, 0);	//Hintergrundfarbe
}

static void RenderScene(void)
{
	
    /* Creating the Scene */
    glClear(GL_COLOR_BUFFER_BIT);   
    glColor3f(0,0,0.5);		  /* Farbe festlegen */
    
    glBegin(GL_LINE_LOOP);    /* ersten mit letzten Punkt verbinden */  
    glVertex2f(0.25,0.25);	  /* Rectangle created by a group */
	glVertex2f(0.75,0.25);    /* of vertices. Last vertex is */
	glVertex2f(0.75,0.75);    /* connected with the first vertex */
	glVertex2f(0.25,0.75);		  
    glEnd(); 
    
    glMatrixMode( GL_MODELVIEW ); 	// Modell-Matrixstack wählen
    glPushMatrix();					// aktuelel Matrix speichern
	glLoadIdentity();             	// Die Matrix zurücksetzen
	glTranslatef(0.5,0.5,-0.5); 	// alles nachfolgende verschieben	
	glColor3f(0,0,0);		  
	glutWireTeapot(0.3);			// Teapott zeichnen
    glPopMatrix();					// gespeicherte Matrix holen
    
    glFlush ();		  /* Flushes OpenGL command queues and buffers */
}

static void myReshape(int w, int h)
{
    /* Callback function to handle window dimensions and size changes */
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);	/* specifies the current matrix */
    glLoadIdentity();			/* Sets the currant matrix to identity */
    gluOrtho2D(0.0,1.0,0.0,1.0);	/* Sets the clipping rectangle extends */
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
    glutMainLoop();
}
