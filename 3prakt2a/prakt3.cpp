/******************************
* Praktikum 3 - Computer Grafik I 
* HTW-Dresden
* Karl Adler
* s62943
* 
* Auf Ubunutu compilieren mit:
* g++ prakt3.cpp -lglut -lGL -lGLU -lGLEW -o prakt3
* 
*******************************/
#include <iostream>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <math.h>   
#include <GL/glut.h> //added GL prefix for Ubuntu compatibility

#define LOGO 42

using namespace std;

static void myinit(void);
static void RenderScene(void);
static void myReshape(int w, int h);
static void initlogo();
	   
static void myinit(void)
{   
	printf( "OpenGL renderer:                  |%s|\n", (char*)glGetString( GL_RENDERER));
	printf( "OpenGL driver version:            |%s|\n", (char*)glGetString( GL_VERSION));		

    /* Initializations */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();		
    glClearColor (1, 1, 1, 0);	//Hintergrundfarbe
    initlogo();
}

static void initlogo(){
	glNewList(LOGO, GL_COMPILE);
		GLfloat p1[] = {0.0,0.0};
		GLfloat p2[] = {3.0,0.0};
		GLfloat p3[] = {0.0,15.0};
		GLfloat p4[] = {10.0,7.5};
		GLfloat p5[] = {15.0,0.0};
		GLfloat p6[] = {22.0,0.0};
		GLfloat p7[] = {25.0,0.0};
		GLfloat p8[] = {31.0,15.0};
		GLfloat p9[] = {26.0,15.0};
		GLfloat p10[] = {30.0,0.0};
		//additional points:
		GLfloat p11[] = {3.0,15.0}; 	// Erster Balken rechts oben
		
		GLfloat p20[] = {18.0,15.0}; 	//zweiter Balken rechts oben
		GLfloat p21[] = {18.0,0.0};  	//zweiter Balken rechts unten
		GLfloat p22[] = {15.0,15.0}; 	//zweiter Balken links oben
		
		GLfloat p30[] = {34.0,15.0}; 	//dritter Balken rechts oben
		
		GLfloat p41[] = {45.0,15.0}; 	//vierter Balken rechts oben
		GLfloat p40[] = {33.0,0.0};  	//vierter Balken rechts unten
		GLfloat p42[] = {42.0,15.0}; 	//vierter Balken links oben
		
		GLfloat p50[] = {3.0,7.5}; 		// Erster Strich Mitte, links
		GLfloat p51[] = {10.0,0.0}; 	// Erster Strich senkrecht unten
		GLfloat p52[] = {10.0,15.0};	// Erster Strich senkrecht oben
		GLfloat p53[] = {22.0, 15.0};	// Zweiter Strich senkrecht oben
		
		glColor3f(0.0,0.0,0.0);
		//Striche
		glBegin(GL_LINES);    
		 glVertex2fv(p50);
		 glVertex2fv(p4);
		glEnd();
		
		glBegin(GL_LINE_STRIP);
		 glVertex2fv(p51);
		 glVertex2fv(p52);
		 glVertex2fv(p22);
		glEnd();
		
		glBegin(GL_LINE_STRIP);
		 glVertex2fv(p6);
		 glVertex2fv(p53);
		 glVertex2fv(p9);
		glEnd();
		
		//erster Balken
		glBegin(GL_QUADS);    
		 glVertex2fv(p1);
		 glVertex2fv(p2);
		 glVertex2fv(p11);
		 glVertex2fv(p3);
		glEnd();
		//zweiter Balken
		glBegin(GL_QUADS);    
		 glVertex2fv(p5);
		 glVertex2fv(p21);
		 glVertex2fv(p20);
		 glVertex2fv(p22);
		glEnd();
		//dritter Balken
		glBegin(GL_QUADS);    
		 glVertex2fv(p6);
		 glVertex2fv(p7);
		 glVertex2fv(p30);
		 glVertex2fv(p8);
		glEnd();
		
		//vierter Balken
		glColor3f(0.9,0.6,0.1);
		glBegin(GL_QUADS);    
		 glVertex2fv(p10);
		 glVertex2fv(p40);
		 glVertex2fv(p41);
		 glVertex2fv(p42);
		glEnd();
	glEndList();
}

static void RenderScene(void)
{
    /* Creating the Scene */
    glClear(GL_COLOR_BUFFER_BIT);   
    //glColor3f(0,0,0.5);		  /* Farbe festlegen */
    glCallList(LOGO);
    //Modell transformation:
    glMatrixMode( GL_MODELVIEW ); 	// Modell-Matrixstack wählen
    glPushMatrix();					// aktuelel Matrix speichern
	glLoadIdentity();             	// Die Matrix zurücksetzen
	glTranslatef(0,20,0); 	// alles nachfolgende verschieben	
	glScalef(0.2,0.2,0.2);
	glCallList(LOGO);
    glPopMatrix();					// gespeicherte Matrix holen
    //Viewport transformation:
    glMatrixMode( GL_PROJECTION ); 	// Viewport-Matrixstack wählen
	glTranslatef(-5.0,25.0,0.0); 	// alles nachfolgende verschieben	
	glScalef(1.5,1.5,1.5);
	glCallList(LOGO);				// gespeicherte Matrix holen
  
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
	return 0;
}
