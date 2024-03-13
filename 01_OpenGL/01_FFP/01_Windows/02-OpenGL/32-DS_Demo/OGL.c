// OpenGL starts here...
// Common Windows Header Files
#include <windows.h>		  // same as <stdio.h> library function, // includes around 3,50,000 API's, Win32 API
#include <stdio.h>            // For File I/O
#include <stdlib.h>           // For exit()				
#include <stdbool.h>

// OpenGl Header Files
#include <gl/GL.h> // '\' also works

#include <gl/glu.h>

#include "OGL.h"		      // Our HeaderFile   

#include <mmsystem.h>

// Maths library 
#define _USE_MATH_DEFINES
#include <math.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

//texture object
GLuint texture_clouds = 0;
GLuint texture_grass = 0;
GLuint texture_Background = 0;

#define MAX_PARTICLES 1000
#define WCX		640
#define WCY		480
#define RAIN	0
#define SNOW	1
#define	HAIL	2

float slowdown = 2.0;
float velocity = 0.0;
float zoom = -16.0;
float pan = 0.0;
float tilt = 0.0;
float hailsize = 0.1;

int loop;
int fall = RAIN;

//floor colors
float r = 0.0;
float g = 1.0;
float b = 0.0;
float ground_points[21][21][3];
float ground_colors[21][21][4];
float accum = -10.0;

typedef struct {
  // Life
  bool alive;	// is the particle alive?
  float life;	// particle lifespan
  float fade; // decay
  // color
  float red;
  float green;
  float blue;
  // Position/direction
  float xpos;
  float ypos;
  float zpos;
  // Velocity/Direction, only goes down in y dir
  float vel;
  // Gravity
  float gravity;
}particles;

// Paticle System
particles par_sys[MAX_PARTICLES];

// function declarations
void drawRain(void);
void drawSnow(void);
void drawHail(void);
void drawScene(void);
void init(void);
void initParticles(int);
void cube(void);
void grass(void);
void BlueBackground(void);

//Macros
#define WIN_WIDTH 800
#define WIN_HEIGHT 600

// link with opengl library
#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "winmm.lib")

//OpenGl Related Global Variables
HDC ghdc = NULL;
HGLRC ghrc = NULL;

// Global Function Declarations (Global because called by OS, _far, _pascal)      
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM); // (WndProc(Window Procedure) - Hungarian(Charles Simonee) Notation used) (CALLBACK FUNCTIONS), (HWND - Handle Window), (WPARAM - 16 Bit), (LPARAM - 32Bit)

// Global Variable Declarations
FILE* gpfile = NULL;          //Global Pointer

HWND ghwnd = NULL;
BOOL gbActive = FALSE;
DWORD dwStyle = 0;
WINDOWPLACEMENT wpPrev = { sizeof(WINDOWPLACEMENT) };
BOOL gbFullscreen = FALSE;

// Entry Point Function
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow) //Winmain(main function which gives window(Hungarian Notation Used)), lpsz(Long Pointer To Zero-Terminated String ('\0')), hPrevInstance - for backward compatibility
{
	//Function Declarations 
	int initialize(void);
	void uninitialize(void);
	void display(void);
	void update(void);

	//music
	PlaySound("Ye Re Ye Re Pausa.wav", NULL, SND_ASYNC | SND_FILENAME);

	// Local Variable Declarations
	WNDCLASSEX wndclass;  // (wndclass)window, class(type), type or class of window EX(WND(window) Extended)
	HWND hwnd;            //handle window (Unsigned Int)
	MSG msg;             // struct
	TCHAR szAppName[] = TEXT(" VMKWindow "); // szAppName - Zero Terminated String To App-Name ('\0'), TEXT (macro)
											 // TEXT(macro) - similar to 'printf'
	int iResult = 0;
	BOOL bdone = FALSE;

	// Local Variable Declarations For Centering Of Window
	int ScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	int ScreenHeight = GetSystemMetrics(SM_CYSCREEN);
	
	//code
	gpfile = fopen("log.txt", "w");
	if (gpfile == NULL)
	{
		MessageBox(NULL, TEXT("Log File Cannot Be Opened"), TEXT("Error"), MB_OK | MB_ICONERROR);
		exit(0);
	}
	fprintf(gpfile, "Program Started Successfully\n");

	// WNDCEX initialization (Window Class Extended)
	wndclass.cbSize = sizeof(WNDCLASSEX); // cbSize - Count Of Byte-size                                      
	wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC; // CS_HREDRAW - Class Style (Horizontal Re-Draw), CS_VREDRAW - (Vertical Re-Draw) // window should be drawn(modified) vertically and horizontally, CS | OWNDC = ClassStyle |Class Ownership(paint Specialist)
	wndclass.cbClsExtra = 0;  //c
	wndclass.cbWndExtra = 0;
	wndclass.lpfnWndProc = WndProc; // lpfnWndProc - Long Pointer To Function - Window-Procedure
	wndclass.hInstance = hInstance;  //Handle To Instance, (Camel Notation Used)
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH); // hbrBackground - (handleTobrush) Background, BLACK_BRUSH = macro For Getting Brush Having Black Color
	wndclass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON)); // hIcon - handle To Icon, Icon is an - 'RESOURCE', ('NULL' Parameter because we have no icon and have to get one), ID - Identifier I - (Application) - Icon ,  
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);  // NULL Parameter because we don't have a cursor, hCursor(Handle To Cursor)  
	wndclass.lpszClassName = szAppName;  // long-pointer to zero(NULL) terminated ('\0') Class Name
	wndclass.lpszMenuName = NULL;      //Menu name - NULL (We Don't Have A Menu)
	wndclass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON)); 

	// Register WNDCLASSEX
	RegisterClassEx(&wndclass); //Registered Extended Class

	// Create Window
	hwnd = CreateWindowEx(WS_EX_APPWINDOW, szAppName, // string to NULL terminated app name('\0'), //Creates window in memory, WS_EX_APPWINDOW -  
		TEXT("Vishwa Khude"), // TEXT(macro) UNICODE
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE,  // (Window Style - OVERLAPPED(appearing above every application), WS_CLIPCHILDREN - area Clipped / Covered By Child Window is Excluded By Parent Window, WS_CLIPSIBLINGS - Exclude Clipping/Covering By SiblingWindow, WS_VISIBLE - PArent Window Should Be Visible 
		ScreenWidth / 2 - WIN_WIDTH / 2,
		ScreenHeight / 2 - WIN_HEIGHT / 2,
		WIN_WIDTH,
		WIN_HEIGHT,
		NULL,    // Desktop Window(Defalut Handle), HWND_DESKTOP, (Parent Window Handle)      
		NULL,    // NULL Parameter for 'Menu'
		hInstance,    // Instance for Creating Handle For Window(OS)
		NULL);   // used as an vessel (lpvoid(long parameter to void) - void*)
	
	ghwnd = hwnd;

	// initialization
	iResult = initialize();
	if (iResult != 0)
	{
		MessageBox(hwnd, TEXT("initialize() Failed"), TEXT("Error"), MB_OK | MB_ICONERROR);
		DestroyWindow(hwnd);
	}
		
	// Show The Window
	ShowWindow(hwnd, iCmdShow);  // ShowWindow - Displaying Our Window, By Using (handle to window (hwnd)), 

	SetForegroundWindow(hwnd); // Set Window On The Fore-ground(Front Order(Z-order))
	SetFocus(hwnd); // Keep My Window Highlited/Focused
		
	// Game-loop
	while (bdone == FALSE)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				bdone = TRUE;
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			if (gbActive == TRUE)
			{
				// Render
				display();

				// Update
				update();
			}
		}


	}

	// Uninitialization
	uninitialize();

	return((int)msg.wParam);
}

// CallBack Function
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam) // UINT- Unsigned INTeger iMsg(integer Message) 
{
	//Function Declarations
	void ToggleFullScreen(void);
	void resize(int, int);

	// code
	switch (iMsg)  // Destroying The Window
	{
	case WM_SETFOCUS:
		gbActive = TRUE;
		break;

	case WM_KILLFOCUS:
		gbActive = FALSE;
		break;

	case WM_SIZE:     // *** lParam(WM_SIZE) gives - Width & Height ***
		resize(LOWORD(lParam), HIWORD(lParam));
		break;

	case WM_ERASEBKGND: // WM_ERASEBKGND - wParam EraseBackGround(Erase The BackGround Of My Window)
		return(0);

	case WM_KEYDOWN:
		switch (LOWORD(wParam))
		{
		case VK_ESCAPE:

			DestroyWindow(hwnd);
			break;
		}
		break;

	case WM_CHAR:
		switch (LOWORD(wParam))
		{
		case 'F':    // FallThrough
		case 'f':
			if (gbFullscreen == FALSE)
			{
				ToggleFullScreen();
				gbFullscreen = TRUE;
			}
			else
			{
				ToggleFullScreen();
				gbFullscreen = FALSE;
			}
			break;
		
		
		}
		break;

	case WM_CLOSE:
	{
		DestroyWindow(hwnd);
		break;
	}

	case WM_DESTROY:  // GetMessage- False
		if (gpfile)
		{
			fprintf(gpfile, "Program Ended Successfully\n");
			fclose(gpfile);
			gpfile = NULL;
		}
		PostQuitMessage(0);
		break;
	default:
		break;
	}

	return(DefWindowProc(hwnd, iMsg, wParam, lParam)); // Default Window Procedure For returning the messages which are not be used in the program
}


void ToggleFullScreen(void)
{
	// Local Variable Declarations
	MONITORINFO mi = { sizeof(MONITORINFO) };

	// Code
	if (gbFullscreen == FALSE)
	{
		dwStyle = GetWindowLong(ghwnd, GWL_STYLE); // Win32 API(dwStyle)

		if (dwStyle & WS_OVERLAPPEDWINDOW)
		{
			if (GetWindowPlacement(ghwnd, &wpPrev) && GetMonitorInfo(MonitorFromWindow(ghwnd, MONITORINFOF_PRIMARY), &mi))
			{
				SetWindowLong(ghwnd, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW);  // ~(To Remove Contents)
				SetWindowPos(ghwnd, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top, mi.rcMonitor.right - mi.rcMonitor.left, mi.rcMonitor.bottom - mi.rcMonitor.top, SWP_NOZORDER | SWP_FRAMECHANGED);
			}
		}
		ShowCursor(FALSE);
	}
	else
	{
		SetWindowPlacement(ghwnd, &wpPrev);

		SetWindowLong(ghwnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);

		SetWindowPos(ghwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_FRAMECHANGED);

		ShowCursor(TRUE);
	}
}

int initialize(void)
{
	init();

	void resize(int, int);
	// Function Declarations

	BOOL loadGLTexture(GLuint*, TCHAR[]);

	GLuint createTexture2D(const char* filePath);

	// Code
	PIXELFORMATDESCRIPTOR pfd;
	int iPixelFormatIndex = 0;
	ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));

	// 1) Initialization of pixel format descriptor
	pfd.nSize, sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;  // RGBA - red, green, blue, alpha
	pfd.cColorBits = 32;
	pfd.cRedBits = 8;
	pfd.cGreenBits = 8;
	pfd.cBlueBits = 8;
	pfd.cAlphaBits = 8;

	// 2) Get The DC 
	ghdc = GetDC(ghwnd);
	if (ghdc == NULL)
	{
		fprintf(gpfile, "GetDC() failed\n");
		return(-1);
	}

	// 3) Choose Pixel Format
	iPixelFormatIndex = ChoosePixelFormat(ghdc, &pfd);
	if (iPixelFormatIndex == 0)
	{
		fprintf(gpfile, "ChoosePixelFormat() failed");
		return(-2);
	}

	// 4) Use Pixel Format Index(Set Obtained Pixel Format)
	if (SetPixelFormat(ghdc, iPixelFormatIndex, &pfd) == FALSE)
	{
		fprintf(gpfile, "SetPixelFormat() failed");
		return(-3);
	}

	// 5) Tell WindowsGraphicsLibrary(WGL) - [bridging API] to give me OpenGl Compatible dc(device context) from this dc(device context)
	ghrc = wglCreateContext(ghdc);
	if (ghrc == NULL)
	{
		fprintf(gpfile, "wglCreateContext() failed");
		return(-4);
	}

	// make rendering context current
	if (wglMakeCurrent(ghdc, ghrc) == FALSE)
	{
		fprintf(gpfile, "wglMakeCurrent() failed");
		return(-5);
	}

	// Set The Clearcolor of window to blue
	
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // alpha(transperancy, opaqueness)

	// Create Texture
	texture_clouds = createTexture2D("clouds.png");
	texture_grass =  createTexture2D("grass.png");
	texture_Background = createTexture2D("BlueBackground.png");

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	//Tell OpenGl To Enable the Texture
	glEnable(GL_TEXTURE_2D);

	resize(WIN_WIDTH, WIN_HEIGHT);

	return(0);
	
}

void resize(int width, int height)
{
	// Code
	if (height <= 0)
		height = 1;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);

	glViewport(0, 0, (GLsizei)width, (GLsizei)height); // 0x to 0y, total Width and Height of my window, binoculars
}

GLuint createTexture2D(const char* filePath)
{
    stbi_set_flip_vertically_on_load(1);
    int width, height, channel;
    unsigned char* data = stbi_load(filePath,&width, &height, &channel, 0);

    if(!data)
    {
        fprintf(gpfile,"Failed To Load %s Texture\n",filePath);
        return -1;
    }

    GLenum format = GL_RGBA;

    if(channel == STBI_grey)
        format = GL_RED;
    else if(channel == STBI_rgb)
        format = GL_RGB;
    else if(channel == STBI_rgb_alpha)
        format = GL_RGBA;

    GLuint texture;
    glPixelStorei(GL_UNPACK_ALIGNMENT,4);
    glGenTextures(1,&texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glBindTexture(GL_TEXTURE_2D,0);

    stbi_image_free(data);
    return texture;
}

void display(void)
{
	// Code
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	//glTranslatef(0.0f, 0.0f, -2.0f); // -3.0f means towards the screen on z-axis (-)on z-axis

	glPushMatrix();
	glTranslatef(-1.6f, 0.88f, -3.0f); // -3.0f means towards the screen on z-axis (-)on z-axis
	glScalef(0.5, 0.5, 0.5);
	cube();
	glPopMatrix();	

	glPushMatrix();
	glTranslatef(-0.68f, 0.88f, -3.0f); // -3.0f means towards the screen on z-axis (-)on z-axis
	glScalef(0.5, 0.5, 0.5);
	cube();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.22f, 0.88f, -3.0f); // -3.0f means towards the screen on z-axis (-)on z-axis
	glScalef(0.5, 0.5, 0.5);
	cube();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.20f, 0.88f, -3.0f); // -3.0f means towards the screen on z-axis (-)on z-axis
	glScalef(0.5, 0.5, 0.5);
	cube();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.95f, 0.88f, -3.0f); // -3.0f means towards the screen on z-axis (-)on z-axis
	glScalef(0.5, 0.5, 0.5);
	cube();
	glPopMatrix();

	drawScene();

	glPushMatrix();
	glTranslatef(0.0f, 0.0f, -3.0f); // -3.0f means towards the screen on z-axis (-)on z-axis
	grass();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0f, 0.0f, -11.0f); // -3.0f means towards the screen on z-axis (-)on z-axis
	glScalef(6.5, 13.5, 1.0);
	BlueBackground();
	glPopMatrix(); 
	
	SwapBuffers(ghdc);

}

void update(void)
{
	// Code

}

void uninitialize(void)
{
	// Function Declarations
	void ToggleFullScreen(void);

	// Code
	// if application is exitting in FullScreen
	if (gbFullscreen == TRUE)
		ToggleFullScreen();

	// make the hdc as Currentdc
	if (wglGetCurrentContext() == ghrc)
	{
		wglMakeCurrent(NULL, NULL);
	}

	// Destroy Rendering Context
	if (ghrc)
	{
		wglDeleteContext(ghrc);
		ghrc = NULL;
	}

	// release the hdc
	if (ghdc)
	{
		ReleaseDC(ghwnd, ghdc);
		ghdc = NULL;
	}
	
	// Destroy window
	if (ghwnd)
	{
		DestroyWindow(ghwnd);
		ghwnd = NULL;
	}

	// Close The LogFile
	if (gpfile)
	{
		fprintf(gpfile, "Program Ended Successfully");
		fclose(gpfile);
		gpfile = NULL;
	}
}

// Initialize/Reset Particles - give them their attributes
void initParticles(int i) {
    par_sys[i].alive = true;
    par_sys[i].life = 1.0;
    par_sys[i].fade = (float)(rand()%100)/1000.0f+0.003f;

    par_sys[i].xpos = (float) (rand() % 21) - 10;
    par_sys[i].ypos = 10.0;
    par_sys[i].zpos = (float) (rand() % 21) - 10;

    par_sys[i].red = 0.5;
    par_sys[i].green = 0.5;
    par_sys[i].blue = 1.0;

    par_sys[i].vel = velocity;
    par_sys[i].gravity = -0.8;//-0.8;

}

void init( ) {
  int x, z;

    glShadeModel(GL_SMOOTH);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);

  // Ground Verticies
    // Ground Colors
    for (z = 0; z < 21; z++) {
      for (x = 0; x < 21; x++) {
        ground_points[x][z][0] = x - 10.0;
        ground_points[x][z][1] = accum;
        ground_points[x][z][2] = z - 10.0;

        ground_colors[z][x][0] = r; // red value
        ground_colors[z][x][1] = g; // green value
        ground_colors[z][x][2] = b; // blue value
        ground_colors[z][x][3] = 0.0; // acummulation factor
      }
    }

    // Initialize particles
    for (loop = 0; loop < MAX_PARTICLES; loop++) {
        initParticles(loop);
    }
}

// For Rain
void drawRain()
{
  float x, y, z;

  for (loop = 0; loop < MAX_PARTICLES; loop=loop+2) {
    if (par_sys[loop].alive == true) {
      x = par_sys[loop].xpos;
      y = par_sys[loop].ypos;
      z = par_sys[loop].zpos + zoom;

	  //glTranslatef(0.0f, 0.0f, 0.0f);

	  // Draw particles
      glColor3f(0.5, 0.5, 1.0);
      glBegin(GL_LINES);
      glVertex3f(x, y, z);
      glVertex3f(x, y+0.5, z);
      glEnd();

      // Update values
      // Move
      // Adjust slowdown for speed!
      par_sys[loop].ypos += par_sys[loop].vel / (slowdown*1000);
      par_sys[loop].vel += par_sys[loop].gravity;
      // Decay
      par_sys[loop].life -= par_sys[loop].fade;

      if (par_sys[loop].ypos <= -10) 
	  {
        par_sys[loop].life = -1.0;
      }

      //Revive
      if (par_sys[loop].life < 0.0) 
	  {
        initParticles(loop);
      }
    }
  }
}

// For Hail
void drawHail() {
  float x, y, z;

  for (loop = 0; loop < MAX_PARTICLES; loop=loop+2) {
    if (par_sys[loop].alive == true) {
      x = par_sys[loop].xpos;
      y = par_sys[loop].ypos;
      z = par_sys[loop].zpos + zoom;

      // Draw particles
      glColor3f(0.8, 0.8, 0.9);
      glBegin(GL_QUADS);
        // Front
        glVertex3f(x-hailsize, y-hailsize, z+hailsize); // lower left
        glVertex3f(x-hailsize, y+hailsize, z+hailsize); // upper left
        glVertex3f(x+hailsize, y+hailsize, z+hailsize); // upper right
        glVertex3f(x+hailsize, y-hailsize, z+hailsize); // lower left
        //Left
        glVertex3f(x-hailsize, y-hailsize, z+hailsize);
        glVertex3f(x-hailsize, y-hailsize, z-hailsize);
        glVertex3f(x-hailsize, y+hailsize, z-hailsize);
        glVertex3f(x-hailsize, y+hailsize, z+hailsize);
        // Back
        glVertex3f(x-hailsize, y-hailsize, z-hailsize);
        glVertex3f(x-hailsize, y+hailsize, z-hailsize);
        glVertex3f(x+hailsize, y+hailsize, z-hailsize);
        glVertex3f(x+hailsize, y-hailsize, z-hailsize);
        //Right
        glVertex3f(x+hailsize, y+hailsize, z+hailsize);
        glVertex3f(x+hailsize, y+hailsize, z-hailsize);
        glVertex3f(x+hailsize, y-hailsize, z-hailsize);
        glVertex3f(x+hailsize, y-hailsize, z+hailsize);
        //Top
        glVertex3f(x-hailsize, y+hailsize, z+hailsize);
        glVertex3f(x-hailsize, y+hailsize, z-hailsize);
        glVertex3f(x+hailsize, y+hailsize, z-hailsize);
        glVertex3f(x+hailsize, y+hailsize, z+hailsize);
        //Bottom
        glVertex3f(x-hailsize, y-hailsize, z+hailsize);
        glVertex3f(x-hailsize, y-hailsize, z-hailsize);
        glVertex3f(x+hailsize, y-hailsize, z-hailsize);
        glVertex3f(x+hailsize, y-hailsize, z+hailsize);
      glEnd();

      // Update values
      //Move
      if (par_sys[loop].ypos <= -10) {
        par_sys[loop].vel = par_sys[loop].vel * -1.0;
      }
      par_sys[loop].ypos += par_sys[loop].vel / (slowdown*1000); // * 1000
      par_sys[loop].vel += par_sys[loop].gravity;

      // Decay
      par_sys[loop].life -= par_sys[loop].fade;

      //Revive
      if (par_sys[loop].life < 0.0) {
        initParticles(loop);
      }
    }
  }
}

// For Snow
void drawSnow() {
  float x, y, z;
  for (loop = 0; loop < MAX_PARTICLES; loop=loop+2) {
    if (par_sys[loop].alive == true) {
      x = par_sys[loop].xpos;
      y = par_sys[loop].ypos;
      z = par_sys[loop].zpos + zoom;

      // Draw particles
      glColor3f(1.0, 1.0, 1.0);
      glPushMatrix();
      glTranslatef(x, y, z);
     // glutSolidSphere(0.2, 16, 16);
      glPopMatrix();

      // Update values
      //Move
      par_sys[loop].ypos += par_sys[loop].vel / (slowdown*1000);
      par_sys[loop].vel += par_sys[loop].gravity;
      // Decay
      par_sys[loop].life -= par_sys[loop].fade;

      if (par_sys[loop].ypos <= -10) {
        int zi = z - zoom + 10;
        int xi = x + 10;
        ground_colors[zi][xi][0] = 1.0;
        ground_colors[zi][xi][2] = 1.0;
        ground_colors[zi][xi][3] += 1.0;
        if (ground_colors[zi][xi][3] > 1.0) {
          ground_points[xi][zi][1] += 0.1;
        }
        par_sys[loop].life = -1.0;
      }

      //Revive
      if (par_sys[loop].life < 0.0) {
        initParticles(loop);
      }
    }
  }
}

// Draw Particles
void drawScene( ) {
  int i, j;
  float x, y, z;

  //glMatrixMode(GL_MODELVIEW);

  //glLoadIdentity();

  //glRotatef(pan, 0.0, 1.0, 0.0);
  //glRotatef(tilt, 1.0, 0.0, 0.0);

  // GROUND?!
  glColor3f(r, g, b);
  /*glBegin(GL_QUADS);
    // along z - y const
    for (i = -10; i+1 < 11; i++) {
      // along x - y const
      for (j = -10; j+1 < 11; j++) {
        glColor3fv(ground_colors[i+10][j+10]);
        glVertex3f(ground_points[j+10][i+10][0],
              ground_points[j+10][i+10][1],
              ground_points[j+10][i+10][2] + zoom);
        glColor3fv(ground_colors[i+10][j+1+10]);
        glVertex3f(ground_points[j+1+10][i+10][0],
              ground_points[j+1+10][i+10][1],
              ground_points[j+1+10][i+10][2] + zoom);
        glColor3fv(ground_colors[i+1+10][j+1+10]);
        glVertex3f(ground_points[j+1+10][i+1+10][0],
              ground_points[j+1+10][i+1+10][1],
              ground_points[j+1+10][i+1+10][2] + zoom);
        glColor3fv(ground_colors[i+1+10][j+10]);
        glVertex3f(ground_points[j+10][i+1+10][0],
              ground_points[j+10][i+1+10][1],
              ground_points[j+10][i+1+10][2] + zoom);
      }

    }
  glEnd(); */
  // Which Particles
  if (fall == RAIN) {
    drawRain();
  }else if (fall == HAIL) {
    drawHail();
  }else if (fall == SNOW) {
    drawSnow();
  }
 
}

void cube()
{	
	glBindTexture(GL_TEXTURE_2D, texture_clouds);
	glBegin(GL_QUADS);
	
	glColor3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f);		//Right Top
	glVertex3f(-1.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f);		//Left Top
	glVertex3f(1.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f);       //left bottom
	glVertex3f(1.0f, -1.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f);		//Right Bottom
	glVertex3f(-1.0f, -1.0f, 0.0f);

	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);
}

void grass()
{
	glBindTexture(GL_TEXTURE_2D, texture_grass);
	glBegin(GL_QUADS);

	glTexCoord2f(1.0f, 1.0f);		//Right Top
	glVertex3f(-3.0f, 1.2f, 0.0f);
	glTexCoord2f(0.0f, 1.0f);		//Left Top
	glVertex3f(3.0f, 1.2f, 0.0f);
	glTexCoord2f(0.0f, 0.0f);       //left bottom
	glVertex3f(3.0f, -1.3f, 0.0f);
	glTexCoord2f(1.0f, 0.0f);		//Right Bottom
	glVertex3f(-3.0f, -1.3f, 0.0f);

	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);
}

void BlueBackground()
{
	glBindTexture(GL_TEXTURE_2D, texture_Background);
	glBegin(GL_QUADS);

	glTexCoord2f(1.0f, 1.0f);		//Right Top
	glVertex3f(-3.0f, 1.2f, 0.0f);
	glTexCoord2f(0.0f, 1.0f);		//Left Top
	glVertex3f(3.0f, 1.2f, 0.0f);
	glTexCoord2f(0.0f, 0.0f);       //left bottom
	glVertex3f(3.0f, 0.1f, 0.0f);
	glTexCoord2f(1.0f, 0.0f);		//Right Bottom
	glVertex3f(-3.0f, 0.1f, 0.0f);

	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);
}

