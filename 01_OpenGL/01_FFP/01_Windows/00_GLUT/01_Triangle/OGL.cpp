// Header Files
#include <GL/freeglut.h>

// Global variable declarations
bool bIsFullScreen = false;

// Entry-point function
int main(int argc, char* argv[])
{
	// Function declarations
	void initialize(void);
	void resize(int, int);
	void display(void);
	void keyboard(unsigned char, int, int);
	void mouse(int, int, int, int);
	void uninitialize(void);

	// code
	glutInit(&argc, argv);  //Init(initialization)
  
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);


	glutInitWindowSize(800, 600); //(800x600), 800-width, 600-height
	glutInitWindowPosition(100, 100); //
	glutCreateWindow("My First RTR5 Program : Vishwa Mahesh Khude");

	initialize();

	glutReshapeFunc(resize);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutCloseFunc(uninitialize);

	glutMainLoop();

	return 0;
}

void initialize(void)
{
	//code
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void resize(int width, int height)
{
	//code
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
}

void display(void)
{
	//code
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glBegin(GL_TRIANGLES);

	//Triangle 1(right) vertex and colors

	glColor3f(1.0f, 0.0f, 0.0f); // colors (RGB) 
	glVertex3f(0.0f, 1.0f, 0.0f); //apex vertex(upper)

	glColor3f(0.0f, 1.0f, 0.0f); // colors (RGB) 
	glVertex3f(-1.0f, 0.0f, 1.0f); //left vertex
	
	glColor3f(0.0f, 0.0f, 1.0f); // colors (RGB) 
	glVertex3f(1.0f, 0.0f, 1.0f); //right vertex
	

	/*//triangle 2 vertex and color change
	glColor3f(1.0f, 0.0f, 0.0f); 
	glVertex3f(0.0f, -0.5f, 0.0f); //apex vertex(bottom)

	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.5f, 0.0f, -0.5f); //left vertex

	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(-0.5f, 0.0f, 0.5f); //right vertex */


	glEnd();

	glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y)
{
	//code
	switch (key)
	{
	case 27:
		glutLeaveMainLoop();
		break;
	case 'F':
	case 'f':
		if (bIsFullScreen == false)
		{
			glutFullScreen();
			bIsFullScreen = true;
		}
		else
		{
			glutLeaveFullScreen();
			bIsFullScreen = false;
		}
		break;

	default:
		break;
	}

}

void mouse(int button, int state, int x, int y)
{
	//code
	switch (button)
	{

	case GLUT_RIGHT_BUTTON:

	 glutLeaveMainLoop();

	break;

	default:
		break;
	}
}


void uninitialize(void)
{
	//code

}
