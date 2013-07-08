/******************************
* Praktikum 1b - Computer Grafik I 
* HTW-Dresden
* Karl Adler
* s62943
* 
* Auf Ubunutu compilieren mit:
* g++ prakt2.cpp -lglut -lGL -lGLU -lGLEW -o prakt2
* 
*******************************/
#include <iostream>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <math.h>   
#include <GL/glut.h> //added GL prefix for Ubuntu compatibility

using namespace std;

static void myinit(void);
static void RenderScene(void);
static void myReshape(int w, int h);
int drawfunction(const char* filename, const GLfloat * color);
int drawCartesian(const GLfloat * color);
int drawScale(const GLfloat* startV, const GLfloat* endV, const GLfloat* color);
   
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
    //glColor3f(0,0,0.5);		  /* Farbe festlegen */
   
    const GLfloat grey[] = {0.5,0.5,0.5};
    drawCartesian(grey);
    
    //Quadrat zeichnen:
    glColor3f(0.9,0.9,0.9);
    glBegin(GL_LINE_LOOP);    // ersten mit letzten Punkt verbinden
     glVertex2f(0,0);	  // Rectangle created by a group
	 glVertex2f(1,0);    // of vertices. Last vertex is
	 glVertex2f(1,1);    // connected with the first vertex
	 glVertex2f(0,1);		  
    glEnd(); 
	
	const GLfloat red[] = {0.9,0.3,0.3};
	drawfunction("daten1", red);
	const GLfloat green[] = {0.3,0.8,0.3};
    drawfunction("daten2", green);
    
    glMatrixMode( GL_MODELVIEW ); 	// Modell-Matrixstack wählen
    glPushMatrix();					// aktuelel Matrix speichern
	glLoadIdentity();   
	          	// Die Matrix zurücksetzen
	glTranslatef(-0.1,-0.1,0.5); 	// alles nachfolgende verschieben	
	glColor3f(0.6,0.7,0.8);		  
	glScalef(0.2,0.2,0.2);
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
    gluOrtho2D(-0.2,1.2,-0.2,1.2);	/* Sets the clipping rectangle extends */
}

int drawfunction(const char* filename, const GLfloat * color)
{
	string s;
	float x, y;
	ifstream f;  				// Datei-Handle
	f.open(filename, ios::in); 	// Öffne Datei aus Parameter
	
	glBegin( GL_LINE_STRIP );	// begin drawing lines
	glColor3fv(color);			// set to given color
	while (!f.eof())        // Solange noch Daten vorliegen
		{
			f >> x;				// lese x
			f >> y;				// lese y
			glVertex2f(x,y);	// erstelle nächsten Punkt
		}
	glEnd();					// linien beenden
	f.close();                	// Datei wieder schließen
	return 1;
}
// Ein Koordinatensystem zeichnen:
int drawCartesian(const GLfloat * color)
{
	cout << color[0] << color[1] << color[2]<< endl;
	//Achsen und Einteilung
	GLfloat startV[] = {0, 0.5};
	GLfloat endV[] = {1, 0.5};
	 drawScale(startV, endV, color);
	GLfloat startV2[] = {0.5, 0};
	GLfloat endV2[] = {0.5, 1};
	 drawScale(startV2, endV2, color);
	
	return 1;
}

int drawScale(const GLfloat* startV, const GLfloat* endV, const GLfloat* color)
{
	//draw the main line
	glColor3fv(color);
	glBegin( GL_LINES );
	 glVertex2fv(startV);
	 glVertex2fv(endV);
	glEnd(); 
	
	//draw scales
	float step = 0.01;
	glBegin( GL_LINES );
		if(startV[1] == endV[1]) // horizontale
		{
			for (float i=startV[0]; i<=endV[0]; i=i+step )
			{
				//every tenth scale line bigger:
				float scaleSize = (fmod((i+step)*100,10) <= 1.0f) ? 0.01: 0.005;
				 glVertex2f(i,startV[1]+scaleSize);
				 glVertex2f(i,endV[1]-scaleSize);
			}
		} else {  //verticale
			for (float i=startV[1]; i<=endV[1]; i=i+step )
			{
				float scaleSize = (fmod((i+step)*100,10) <= 1.0f) ? 0.01: 0.005;
				 glVertex2f(startV[0]+scaleSize, i);
				 glVertex2f(endV[0]-scaleSize, i);
			}			
		}
	glEnd();
	
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
