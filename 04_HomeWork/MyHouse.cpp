#include <GL/freeglut.h>

// Global variable declarations
bool bIsFullScreen = false;

//Entry-Point Function
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
	glutCreateWindow("My House");

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
    glClearColor(1.0f, 0.89f, 0.88f, 0.0f);
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
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	// Chimney
	glBegin(GL_POLYGON);
	
	glColor3f(0.50f, 0.50f, 0.50f); //RGB values(Brown)
    glVertex3f(0.3f, 0.72f, 0.0f); // (x & y (-,+))
    glVertex3f(0.5f, 0.72f, 0.0f); // (x & y (+, +))
	glVertex3f(0.5f, 0.6f, 0.0f); // (x & y (+, -)) 
    glVertex3f(0.3f, 0.6f, 0.0f); // (x & y (-, -))
	glEnd();

	glBegin(GL_LINE_LOOP);
	
	glColor3f(0.0f, 0.0f, 0.0f); //RGB values(Brown)
    glVertex3f(0.3f, 0.72f, 0.0f); // (x & y (-,+))
    glVertex3f(0.5f, 0.72f, 0.0f); // (x & y (+, +))
	glVertex3f(0.5f, 0.6f, 0.0f); // (x & y (+, -)) 
    glVertex3f(0.3f, 0.6f, 0.0f); // (x & y (-, -))
	glEnd();	

	glBegin(GL_POLYGON);
	
	glColor3f(0.41f, 0.41f, 0.41f);
	glVertex3f(0.28f, 0.74f, 0.0f); // (x & y (-,+))
    glVertex3f(0.52f, 0.74f, 0.0f); // (x & y (+, +))
	glVertex3f(0.52f, 0.72f, 0.0f); // (x & y (+, -)) 
    glVertex3f(0.28f, 0.72f, 0.0f); // (x & y (-, -))
    glEnd(); 

	glBegin(GL_LINE_LOOP);
	
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.28f, 0.74f, 0.0f); // (x & y (-,+))
    glVertex3f(0.52f, 0.74f, 0.0f); // (x & y (+, +))
	glVertex3f(0.52f, 0.72f, 0.0f); // (x & y (+, -)) 
    glVertex3f(0.28f, 0.72f, 0.0f); // (x & y (-, -))
    glEnd(); 

	// Roof
	glBegin(GL_POLYGON);
	
	glColor3f(0.64f, 0.16f, 0.16f); //RGB values(Brown)
    glVertex3f(-0.75f, 0.6f, 0.0f); // (x & y (-,+))
    glVertex3f(0.75f, 0.6f, 0.0f); // (x & y (+, +))
	glVertex3f(0.95f, 0.2f, 0.0f); // (x & y (+, -)) 
    glVertex3f(-0.95f, 0.2f, 0.0f); // (x & y (-, -))
    glEnd(); 
    
	glBegin(GL_LINE_LOOP);
	
	glColor3f(0.0f, 0.0f, 0.0f); //RGB values(Brown)
    glVertex3f(-0.75f, 0.6f, 0.0f); // (x & y (-,+))
    glVertex3f(0.75f, 0.6f, 0.0f); // (x & y (+, +))
	glVertex3f(0.95f, 0.2f, 0.0f); // (x & y (+, -)) 
    glVertex3f(-0.95f, 0.2f, 0.0f); // (x & y (-, -))
    glEnd(); 
	
	// Right Part
    glBegin(GL_QUADS);

    glColor3f(0.48f, 0.40f, 0.93f); //RGB values()
    glVertex3f(-0.7f, 0.2f, 0.0f); // (x & y (-,+))
    glVertex3f(0.6f, 0.2f, 0.0f); // (x & y (+, +))
	glVertex3f(0.6f, -0.9f, 0.0f); // (x & y (+, -)) 
    glVertex3f(-0.7f, -0.9f, 0.0f); // (x & y (-, -))
    glEnd(); 

	glBegin(GL_LINE_LOOP);

    glColor3f(0.0f, 0.0f, 0.0f); //RGB values()
    glVertex3f(-0.7f, 0.2f, 0.0f); // (x & y (-,+))
    glVertex3f(0.6f, 0.2f, 0.0f); // (x & y (+, +))
	glVertex3f(0.6f, -0.9f, 0.0f); // (x & y (+, -)) 
    glVertex3f(-0.7f, -0.9f, 0.0f); // (x & y (-, -))
    glEnd(); 

	glBegin(GL_POLYGON);

    glColor3f(0.48f, 0.40f, 0.93f); //RGB values()
    glVertex3f(0.6f, 0.2f, 0.0f); // (x & y (-,+))
    glVertex3f(0.85f, 0.2f, 0.0f); // (x & y (+, +))
	glVertex3f(0.85f, -0.8f, 0.0f); // (x & y (+, -)) 
    glVertex3f(0.6f, -0.9f, 0.0f); // (x & y (-, -))
    glEnd(); 

	glBegin(GL_LINE_LOOP);

    glColor3f(0.0f, 0.0f, 0.0f); //RGB values()
    glVertex3f(0.6f, 0.2f, 0.0f); // (x & y (-,+))
    glVertex3f(0.85f, 0.2f, 0.0f); // (x & y (+, +))
	glVertex3f(0.85f, -0.8f, 0.0f); // (x & y (+, -)) 
    glVertex3f(0.6f, -0.9f, 0.0f); // (x & y (-, -))
    glEnd(); 
	
	// Left Part
	glBegin(GL_POLYGON);

	glColor3f(0.48f, 0.40f, 0.93f); //RGB values()
    glVertex3f(-0.7f, 0.2f, 0.0f); // Apex (x & y axis)
    glVertex3f(-0.87f, 0.2f, 0.0f); // Middle Portion()
    glVertex3f(-0.87f, -0.9f, 0.0f); // 
    glVertex3f(-0.7f, -0.9f, 0.0f);
    glEnd(); 

	glBegin(GL_LINE_LOOP);

	glColor3f(0.0f, 0.0f, 0.0f); //RGB values()
    glVertex3f(-0.87f, 0.2f, 0.0f); // Apex (x & y axis)
    glVertex3f(-0.41f, 0.2f, 0.0f); // Middle Portion()
    glVertex3f(-0.41f, -0.9f, 0.0f); // 
    glVertex3f(-0.87f, -0.9f, 0.0f);
    glEnd(); 
		
	// Big Window
	glBegin(GL_QUADS);

    glColor3f(0.0f, 0.0f, 0.0f); //RGB values()
    glVertex3f(-0.84f, 0.1f, 0.0f); // (x & y (-,+))
    glVertex3f(-0.43f, 0.1f, 0.0f); // (x & y (+, +))
	glVertex3f(-0.43f, -0.15f, 0.0f); // (x & y (+, -)) 
    glVertex3f(-0.84f, -0.15f, 0.0f); // (x & y (-, -))
    glEnd(); 

	glBegin(GL_LINE_LOOP);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.84f, 0.1f, 0.0f);
	glVertex3f(-0.43f, 0.1f, 0.0f);
	glVertex3f(-0.43f, -0.15f, 0.0f);
	glVertex3f(-0.84f, -0.15f, 0.0f);
	glEnd();

	// Window Border
	glBegin(GL_QUADS);

    glColor3f(0.52f, 0.80f, 0.98f); //RGB values()
    glVertex3f(-0.83f, 0.09f, 0.0f); // (x & y (-,+))
    glVertex3f(-0.44f, 0.09f, 0.0f); // (x & y (+, +))
	glVertex3f(-0.44f, -0.14f, 0.0f); // (x & y (+, -)) 
    glVertex3f(-0.83f, -0.14f, 0.0f); // (x & y (-, -))
    glEnd(); 

	glBegin(GL_LINE_LOOP);

    glColor3f(0.0f, 0.0f, 0.f); //RGB values()
    glVertex3f(-0.83f, 0.09f, 0.0f); // (x & y (-,+))
    glVertex3f(-0.44f, 0.09f, 0.0f); // (x & y (+, +))
	glVertex3f(-0.44f, -0.14f, 0.0f); // (x & y (+, -)) 
    glVertex3f(-0.83f, -0.14f, 0.0f); // (x & y (-, -))
    glEnd(); 

	//Division(Horizontal)
	/*glBegin(GL_LINE_LOOP);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.65f, 0.0f, 0.0f);
	glVertex3f(-0.24f, 0.0f, 0.0f);
	glVertex3f(-0.24f, 0.0f, 0.0f);
	glVertex3f(-0.65f, 0.0f, 0.0f);
	glEnd(); */
	
	// Left Window
	glBegin(GL_QUADS);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.28f, 0.1f, 0.0f);
	glVertex3f(-0.001f, 0.1f, 0.0f);
	glVertex3f(-0.001f, -0.15f, 0.0f);
	glVertex3f(-0.28f, -0.15f, 0.0f);
	glEnd();

	glBegin(GL_LINE_LOOP);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.28f, 0.1f, 0.0f);
	glVertex3f(-0.001f, 0.1f, 0.0f);
	glVertex3f(-0.001f, -0.15f, 0.0f);
	glVertex3f(-0.28f, -0.15f, 0.0f);
	glEnd();

	// Left Window Border
	glBegin(GL_QUADS);

	glColor3f(0.52f, 0.80f, 0.98f);
	glVertex3f(-0.27f, 0.09f, 0.0f);
	glVertex3f(-0.012f, 0.09f, 0.0f);
	glVertex3f(-0.012f, -0.14f, 0.0f);
	glVertex3f(-0.27f, -0.14f, 0.0f);
	glEnd();

	glBegin(GL_LINE_LOOP);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-0.27f, 0.09f, 0.0f);
	glVertex3f(-0.012f, 0.09f, 0.0f);
	glVertex3f(-0.012f, -0.14f, 0.0f);
	glVertex3f(-0.27f, -0.14f, 0.0f);
	glEnd(); 

	// Right Window
	glBegin(GL_QUADS);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.22f, 0.1f, 0.0f);
	glVertex3f(0.5f, 0.1f, 0.0f);
	glVertex3f(0.5f, -0.15f, 0.0f);
	glVertex3f(0.22f, -0.15f, 0.0f);
	glEnd();

	glBegin(GL_LINE_LOOP);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.22f, 0.1f, 0.0f);
	glVertex3f(0.5f, 0.1f, 0.0f);
	glVertex3f(0.5f, -0.15f, 0.0f);
	glVertex3f(0.22f, -0.15f, 0.0f);
	glEnd();

	// Right Window Border
	glBegin(GL_QUADS);

	glColor3f(0.52f, 0.80f, 0.98f);
	glVertex3f(0.23f, 0.09f, 0.0f);
	glVertex3f(0.49f, 0.09f, 0.0f);
	glVertex3f(0.49f, -0.14f, 0.0f);
	glVertex3f(0.23f, -0.14f, 0.0f);
	glEnd();

	glBegin(GL_LINE_LOOP);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.23f, 0.09f, 0.0f);
	glVertex3f(0.49f, 0.09f, 0.0f);
	glVertex3f(0.49f, -0.14f, 0.0f);
	glVertex3f(0.23f, -0.14f, 0.0f);
	glEnd(); 

	glBegin(GL_POLYGON);

	//door
	glColor3f(0.62f, 0.32f, 0.17f); //RGB values(Saddle Brown)
    glVertex3f(0.2f, -0.3f, 0.0f); // Apex (x & y axis)
    glVertex3f(0.5f, -0.3f, 0.0f); // Middle Portion()
    glVertex3f(0.5f, -0.9f, 0.0f); // 
    glVertex3f(0.2f, -0.9f, 0.0f);
    glEnd(); 

	glBegin(GL_LINE_LOOP);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.2f, -0.3f, 0.0f);
	glVertex3f(0.5f, -0.3f, 0.0f);
	glVertex3f(0.5f, -0.9f, 0.0f);
	glVertex3f(0.2f, -0.9f, 0.0f);
	glEnd();

	glBegin(GL_QUADS);

	//door knob
	glColor3f(0.0f, 0.0f, 0.0f); //RGB values(Saddle Brown)
    glVertex3f(0.22f, -0.6f, 0.0f); // Apex (x & y axis)
    glVertex3f(0.24f, -0.6f, 0.0f); // Middle Portion()
    glVertex3f(0.24f, -0.62f, 0.0f); // 
    glVertex3f(0.22f, -0.62f, 0.0f);
    glEnd(); 

	// Door Base
	glBegin(GL_QUADS);

	glColor3f(0.62f, 0.32f, 0.17f); //RGB values(Saddle Brown)
    glVertex3f(0.18f, -0.87f, 0.0f); // Apex (x & y axis)
    glVertex3f(0.52f, -0.87f, 0.0f); // Middle Portion()
    glVertex3f(0.52f, -0.9f, 0.0f); // 
    glVertex3f(0.18f, -0.9f, 0.0f);
    glEnd(); 

	glBegin(GL_LINE_LOOP);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.18f, -0.87f, 0.0f);
	glVertex3f(0.52f, -0.87f, 0.0f);
	glVertex3f(0.52f, -0.9f, 0.0f);
	glVertex3f(0.18f, -0.9f, 0.0f);
	glEnd();

	// Ground
    /*glColor3f(0.0f, 1.0f, 0.0f); //RGB values(Green)
    glVertex3f(-1.0f, 0.15f, 0.0f); // Apex (x & y axis)
    glVertex3f(1.0f, 0.15f, 0.0f); // Middle Portion()
    glVertex3f(1.0f, -1.0f, 0.0f); // 
    glVertex3f(-1.0f, -1.0f, 0.0f);
    glEnd(); */ 

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
