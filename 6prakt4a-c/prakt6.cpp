/******************************
* Praktikum 4 - Computer Grafik I 
* HTW-Dresden
* Karl Adler
* s62943
* 
* Auf Ubunutu compilieren mit:
* g++ prakt5.cpp -lglut -lGL -lGLU -lGLEW -o prakt5
* 
*******************************/
#include <iostream>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <math.h>
#include <time.h> 
#include <GL/glut.h> //added GL prefix for Ubuntu compatibility

#define CUBE 1
#define DELTA 0.1
#define SCREENW 600
#define SCREENH 600

//using namespace std;

static void myinit(void);
static void RenderScene(void);
static void myReshape(int w, int h);
static void initCube();
void spinDisplay();
void myKeyboard(unsigned char theKey, int mouseX, int mouseY);
void mySpecial(int specKey, int mouseX, int mouseY);
void rotateCube(int x, int y);
void setPerspective();

//globals:	

GLfloat viewpoint[3];
GLfloat alpha = 0.2;
GLfloat beta = 0.8;
GLfloat distance = 5.0;
GLfloat spin = 0.0;
GLfloat x = 0.0;
time_t tstart;
int mode = 2;

GLint mouseXstart = 0;
GLint mouseYstart = 0;
      
static void myinit(void)
{   
	printf( "OpenGL renderer:                  |%s|\n", (char*)glGetString( GL_RENDERER));
	printf( "OpenGL driver version:            |%s|\n", (char*)glGetString( GL_VERSION));		

    /* Initializations */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();		
    glClearColor (1, 1, 1, 0);	//Hintergrundfarbe
    initCube();
    time(&tstart);	//save start time
}

static void initCube(){
	glNewList(CUBE, GL_COMPILE);
		//colors:
		const GLfloat white[] = {1.0,1.0,1.0};
		const GLfloat grey[] = {0.5,0.5,0.5};
		const GLfloat black[] = {0.0,0.0,0.0};
		const GLfloat red[] = {1.0,0.0,0.0};
		const GLfloat green[] = {0.0,1.0,0.0};
		const GLfloat blue[] = {0.0,0.0,1.0};
		const GLfloat cyan[] = {0.0,1.0,1.0};
		const GLfloat magenta[] = {1.0,0.0,1.0};
		const GLfloat yellow[] = {1.0,1.0,0.0};
		//edge length and zero
		const GLfloat l=1.0;
		const GLfloat o=0.0;
		//corners:
		const GLfloat lbb[] = {o,o,o};	// left  bottom 	back
		const GLfloat lbf[] = {o,o,l};	// left  bottom 	front
		const GLfloat ltb[] = {o,l,o};	// left  top		back
		const GLfloat ltf[] = {o,l,l};	// left  top		front
		const GLfloat rbb[] = {l,o,o};	// right bottom 	back
		const GLfloat rbf[] = {l,o,l};	// right bottom 	front
		const GLfloat rtb[] = {l,l,o};	// right top 		back
		const GLfloat rtf[] = {l,l,l};	// right top 		front
		
		//FRONT
		glBegin(GL_POLYGON);
			glColor3fv( blue );     glVertex3fv( lbf );
			glColor3fv( magenta );	glVertex3fv( rbf );
			glColor3fv( white );    glVertex3fv( rtf );
			glColor3fv( cyan );     glVertex3fv( ltf ); 
		glEnd();
		//RIGHT
		glBegin(GL_POLYGON);
			glColor3fv( magenta );  glVertex3fv( rbf );
			glColor3fv( red );		glVertex3fv( rbb );
			glColor3fv( yellow );   glVertex3fv( rtb );
			glColor3fv( white );    glVertex3fv( rtf ); 
		glEnd();
		//LEFT
		glBegin(GL_POLYGON);
			glColor3fv( blue );     glVertex3fv( lbf );
			glColor3fv( black );	glVertex3fv( lbb );
			glColor3fv( green );    glVertex3fv( ltb );
			glColor3fv( cyan );     glVertex3fv( ltf ); 
		glEnd();
		//BOTTOM
		glBegin(GL_POLYGON);
			glColor3fv( blue );     glVertex3fv( lbf );
			glColor3fv( magenta );	glVertex3fv( rbf );
			glColor3fv( red );	    glVertex3fv( rbb );
			glColor3fv( black );    glVertex3fv( lbb ); 
		glEnd();
		//TOP
		glBegin(GL_POLYGON);
			glColor3fv( white );    glVertex3fv( rtf );
			glColor3fv( yellow );	glVertex3fv( rtb );
			glColor3fv( green );    glVertex3fv( ltb );
			glColor3fv( cyan );     glVertex3fv( ltf ); 
		glEnd();
		//BACK
		glBegin(GL_POLYGON);
			glColor3fv( black );    glVertex3fv( lbb );
			glColor3fv( red );		glVertex3fv( rbb );
			glColor3fv( yellow );   glVertex3fv( rtb );
			glColor3fv( green );    glVertex3fv( ltb ); 
		glEnd();
	glEndList();
}

static void RenderScene(void)
{
    /* Creating the Scene */ 
    //  Clear screen and Z-buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f(0,0,0.5);		  /* Farbe festlegen */
   glMatrixMode( GL_MODELVIEW ); 		// Modell-Matrixstack wählen
		glLoadIdentity();             	// Die Matrix zurücksetzen
		glCallList(CUBE);
	glMatrixMode( GL_PROJECTION );		// World-Matrixstack wählen
		glLoadIdentity();             	// Die Matrix zurücksetzen
	setPerspective();
	glutSwapBuffers();
}

static void myReshape(int w, int h)
{
    /* Callback function to handle window dimensions and size changes */
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);	/* specifies the current matrix */
	glLoadIdentity();			/* Sets the currant matrix to identity */
//	gluPerspective(30,1.0,0.1,20.0);	//ankle, aspect ratio x/y, near, far
//	gluLookAt(0,0,10,0.5,0.5,0.5,0,1,0); //Eye:XYZ, Center XYZ, Up XYZ 
	setPerspective();
}

/****************************
* --- ab hier aufgabe 2 --- *
****************************/
//keyboard functions
void myKeyboard(unsigned char theKey, int mouseX, int mouseY)
{
	GLint x = mouseX;
	GLint y = mouseY;
	switch (theKey)
	{
		case 'a':
			distance -= DELTA;
			break;
		case 'q':
			distance += DELTA;
			break;
		case '1':	//orthogonal
			mode = 1;
			break;
		case '2':	//perspektivisch
			mode = 2;
			break;
		case '3':	//Grundris
			mode = 3;
			break;	
		case '4':	//aufriss
			mode = 4;
			break;	
		case '5':	//Seitenriss
			mode = 5;
			break;	
		case '6':	//Dimetrie
			mode = 6;
			break;		
		case '7':	//Isometrie
			mode = 7;
			break;			
		case 'e':
			exit(-1);
	}
	glutPostRedisplay(); //request display() call ASAP
}


void setPerspective(){
	switch(mode){
		case 1:
			viewpoint[0] = distance*sin(beta)*sin(alpha)+0.5;
			viewpoint[1] = distance*cos(beta)+0.5;
			viewpoint[2] = distance*sin(beta)*cos(alpha)+0.5;
			glOrtho(-1,1,-1,1,-20,20);
			gluLookAt(viewpoint[0], viewpoint[1], viewpoint[2],
			 0.5, 0.5, 0.5, 0.0, cos(beta-3.141593*0.5), 0.0); 
			break;
		case 2:
			viewpoint[0] = distance*sin(beta)*sin(alpha)+0.5;
			viewpoint[1] = distance*cos(beta)+0.5;
			viewpoint[2] = distance*sin(beta)*cos(alpha)+0.5;
			gluPerspective(30.0, 1.0, 0.1, 20.0);
			gluLookAt(viewpoint[0], viewpoint[1], viewpoint[2],
			 0.5, 0.5, 0.5, 0.0, cos(beta-3.141593*0.5), 0.0); 
			break;
		
		case 3:	//dimetral
			viewpoint[0] = distance;
			viewpoint[1] = distance;
			viewpoint[2] = distance*sqrtf(7);
			glOrtho(-1,1,-1,1,-20,20);
			gluLookAt(viewpoint[0], viewpoint[1], viewpoint[2],
			 0, 0, 0, 0.0, 1.0, 0.0); 
			break;
		case 4:	//isometric
			viewpoint[0] = distance;
			viewpoint[1] = distance;
			viewpoint[2] = distance;
			glOrtho(-1,1,-1,1,-20,20);
			gluLookAt(viewpoint[0], viewpoint[1], viewpoint[2],
			 0, 0, 0, 0.0, 1.0, 0.0); 
			break;
		case 5:	//Grundriss
			viewpoint[0] = 0.5;
			viewpoint[1] = -distance;
			viewpoint[2] = 0.5;
			glOrtho(-1,1,-1,1,-20,20);
			gluLookAt(viewpoint[0], viewpoint[1], viewpoint[2],
			 0.5, 0.5, 0.5, 1.0, 0.0, 0.0); 
			break;
		case 6:	//Aufriss
			viewpoint[0] = 0.5;
			viewpoint[1] = distance;
			viewpoint[2] = 0.5;
			glOrtho(-1,1,-1,1,-20,20);
			gluLookAt(viewpoint[0], viewpoint[1], viewpoint[2],
			 0.5, 0.5, 0.5, 1.0, 0.0, 0.0); 
			break;
		case 7:	//Seitenriss
			viewpoint[0] = distance;
			viewpoint[1] = 0.5;
			viewpoint[2] = 0.5;
			glOrtho(-1,1,-1,1,-20,20);
			gluLookAt(viewpoint[0], viewpoint[1], viewpoint[2],
			 0.5, 0.5, 0.5, 0.0, 1.0, 0.0); 
			break;
	}
}

void mySpecial(int specKey, int mouseX, int mouseY)
{
	GLint x = mouseX;
	GLint y = SCREENH - mouseY;
	
	switch (specKey)
	{
		case GLUT_KEY_LEFT:  
			alpha -= DELTA;
			break;
		case GLUT_KEY_RIGHT:  
			alpha += DELTA;
			break;
		case GLUT_KEY_UP:  
			beta -= DELTA;
			break;
		case GLUT_KEY_DOWN:
			RenderScene();
			beta += DELTA;
			break;
	}
	glutPostRedisplay(); //request display() call ASAP
}
//move with the mouse
void myMouse(int button, int state, int x, int y)
{
	std::cout << button << std::endl;
	switch (button)
	{
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_DOWN){
				mouseXstart = x;
				mouseYstart = y;
				glutMotionFunc(rotateCube);
				}
		break;
		case 3:	//scroll up
			if (state == GLUT_UP);
			distance -= DELTA;		
			glutPostRedisplay();	
		break;
		case 4:	//scroll down
			if (state == GLUT_UP);
			distance += DELTA;
			glutPostRedisplay();
		break;
    }
}

void rotateCube(int x, int y){
	//relative mouse movement:
	int deltaX = mouseXstart - x;
	int deltaY = mouseYstart - y;
	std::cout << deltaX << " - " << deltaY << std::endl;
	alpha += deltaX * 0.01;
	beta += deltaY * 0.01;
	mouseXstart = x;
	mouseYstart = y;
	glutPostRedisplay(); 
} 

/*  Main Loop */

int main(int argc, char** argv)
{
    
    /* GLUT functions to set up the window */
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_ALPHA | GLUT_DEPTH);
    glutInitWindowPosition (0, 0);
	glutInitWindowSize(SCREENW, SCREENH);
	glutInit(&argc, argv);
    glutCreateWindow (argv[0]);
    //  Enable Z-buffer depth test
	glEnable(GL_DEPTH_TEST);
    
    myinit();			/* Initialization */
    glutReshapeFunc (myReshape); /* Function to call when window changes size */
    glutDisplayFunc(RenderScene); /* Creating the Szene */
   // glutIdleFunc(spinDisplay);

    glutKeyboardFunc(myKeyboard);
    glutSpecialFunc(mySpecial);
    glutMouseFunc(myMouse);
    glutMainLoop();
	return 0;
}
