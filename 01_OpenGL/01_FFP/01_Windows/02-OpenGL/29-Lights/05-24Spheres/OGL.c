// OpenGL starts here...
// Common Windows Header Files
#include <windows.h>		  // same as <stdio.h> library function, // includes around 3,50,000 API's, Win32 API
#include <stdio.h>            // For File I/O
#include <stdlib.h>           // For exit()				

// OpenGl Header Files
#include <gl/GL.h> // '\' also works

#include <gl/glu.h>

#include "OGL.h"		      // Our HeaderFile   

//Macros
#define WIN_WIDTH 800
#define WIN_HEIGHT 600

// link with opengl library
#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "glu32.lib")

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

//Light Global Variables 
BOOL bLight = FALSE;
GLfloat lightAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat lightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat lightPosition[] = { 0.0f, 0.0f, 0.0f, 1.0f };

GLUquadric* quadric = NULL;

GLfloat angleForXRotation = 0.0f;
GLfloat angleForYRotation = 0.0f;
GLfloat angleForZRotation = 0.0f;
GLuint keyPressed = 0;


// Entry Point Function
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow) //Winmain(main function which gives window(Hungarian Notation Used)), lpsz(Long Pointer To Zero-Terminated String ('\0')), hPrevInstance - for backward compatibility
{
	//Function Declarations 
	int initialize(void);
	void uninitialize(void);
	void display(void);
	void update(void);

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

		case 'L':
		case 'l':
			if (bLight == FALSE)
			{
				glEnable(GL_LIGHTING);
				bLight = TRUE;
			}
			else
			{
				glDisable(GL_LIGHTING);
				bLight = FALSE;
			}
			break;

		case 'X':
		case 'x':
			keyPressed = 1;
			angleForXRotation = 0.0f; // Reset
			break;
		case 'Y':
		case 'y':
			keyPressed = 2;
			angleForYRotation = 0.0f; // Reset
			break;
		case 'Z':
		case 'z':
			keyPressed = 3;
			angleForZRotation = 0.0f; // Reset
			break;

		}
		break;

	case WM_CLOSE:
	{
		DestroyWindow(hwnd);
		break;
	}

	case WM_DESTROY :  // GetMessage- False
		if (gpfile)
		{
			fprintf(gpfile, "Program Ended Successfully\n");
			fclose(gpfile);
			gpfile = NULL;
		}
		PostQuitMessage(0);
		break;
	default :
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
	void resize(int, int);
	// Function Declarations

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
	pfd.cDepthBits = 32;

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

	//Enabling Depth...
	glShadeModel(GL_SMOOTH); // For smooth shading while Lighting(optional) 
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // Optional (Nicest Correction Of)

	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
	glEnable(GL_LIGHT0);
	
	//Initialize Quadric
	quadric = gluNewQuadric(); //internally calls malloc()

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Set The Clearcolor of window to blue
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f); // background(grey/dark grey) alpha(transperancy, opaqueness)
			
	resize(WIN_WIDTH, WIN_HEIGHT);  //Warmup resize()

	return(0);
	
}

void resize(int width, int height)
{
	// Code
	if (height <= 0)
		height = 1;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if (width <= height)
	{
		glOrtho(0.0f,   //left
			15.5f,      //right
			0.0f * ((GLfloat)height / (GLfloat)width), //bottom
			15.5f * ((GLfloat)height / (GLfloat)width), //top
			-10.0f, //near  
			10.0f);	 //far						//left, right, bottom, top, far, near
	}
	else
	{
		glOrtho(0.0f * ((GLfloat)width / (GLfloat)height),
			15.5f * ((GLfloat)width / (GLfloat)height),
			0.0f,
			15.0f,
			-10.0f,
			10.0f);                         //left, right, bottom, top, far, near
	}

	glViewport(0, 0, (GLsizei)width, (GLsizei)height); // 0x to 0y, total Width and Height of my window, binoculars
}

void display(void)
{
	// Code

	//Variable Declarations
	GLfloat materialAmbient[4];
	GLfloat materialDiffuse[4];
	GLfloat materialSpecular[4];
	GLfloat materialShininess;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Depth-Buffer Added For 3D(Depth)
		
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//Animation
	if (keyPressed == 1)
	{
		glRotatef(angleForXRotation, 1.0f, 0.0f, 0.0f);
		lightPosition[2] = angleForXRotation;
	}

	else if (keyPressed == 2)
	{
		glRotatef(angleForYRotation, 0.0f, 1.0f, 0.0f);
		lightPosition[0] = angleForYRotation;
	}

	else if (keyPressed == 3)
	{
		glRotatef(angleForZRotation(0.0f, 0.0f, 1.0f);
		lightPosition[1] = angleForZRotation;
	}

	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

	//First Column Of Precious Stones
	 
	// ***** 1st sphere on 1st column, emerald *****
	materialAmbient[0] = 0.0215; // r
	materialAmbient[1] = 0.1745; // g
	materialAmbient[2] = 0.0215; // b
	materialAmbient[3] = 1.0f;   // a
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	materialDiffuse[0] = 0.07568; // r
	materialDiffuse[1] = 0.61424; // g
	materialDiffuse[2] = 0.07568; // b
	materialDiffuse[3] = 1.0f;    // a
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	materialSpecular[0] = 0.633;  // r 
	materialSpecular[1] = 0.727811; // g
	materialSpecular[2] = 0.633;    // b
	materialSpecular[3] = 1.0f;     // a
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	materialShininess = 0.6 * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(1.0f, 14.0f, 0.0f); // -5.0f means towards the screen on z-axis (-)on z-axis
	gluSphere(quadric, 1.0f, 30, 30); //gluSphere call internally creates all Normals for you

	// ***** 2nd sphere on 1st column, jade *****
	// ambient material 
	materialAmbient[0] = 0.135; //r
	materialAmbient[1] = 0.2225; // g
	materialAmbient[2] = 0.1575; // b
	materialAmbient[3] = 1.0f;   // a
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	// diffuse material
	materialDiffuse[0] = 0.54; // r
	materialDiffuse[1] = 0.89; // g
	materialDiffuse[2] = 0.63; // b
	materialDiffuse[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	// specular material
	materialSpecular[0] = 0.316228; // r
	materialSpecular[1] = 0.316228; // g
	materialSpecular[2] = 0.316228; // b
	materialSpecular[3] = 1.0f;     //a
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	//Shininess
	materialShininess = 0.1 * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);
	glLoadIdentity();
	glTranslatef(1.0f, 11.5f, 0.0f);
	gluSphere(1.0f, 30, 30);

	// ***** 3rd sphere on 1st column, obsidian *****
	// ambient material
	materialAmbient[0] = 0.05375; // r
	materialAmbient[1] = 0.05;    // g
	materialAmbient[2] = 0.06625; // b
	materialAmbient[3] = 1.0f;    // a
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	// diffuse material
	materialDiffuse[0] = 0.18275; // r
	materialDiffuse[1] = 0.17;    // g
	materialDiffuse[2] = 0.22525; // b
	materialDiffuse[3] = 1.0f;    // a
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	// specular material
	materialSpecular[0] = 0.332741; // r
	materialSpecular[1] = 0.328634; // g
	materialSpecular[2] = 0.346435; // b
	materialSpecular[3] = 1.0f;     // a
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialSpecular);

	// shininess
	materialShininess = 0.3 * 128;
	glMaterialf(GL_FRONT, GL_DIFFUSE, materialShininess);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(1.5f, 14.0f, 0.0f); // -5.0f means towards the screen on z-axis (-)on z-axis
	gluSphere(quadric, 1.0f, 30, 30); //gluSphere call internally creates all Normals for you

	// ***** 3rd sphere on 1st column, obsidian *****
	// ambient material
	materialAmbient[0] = 0.05375; // r
	materialAmbient[1] = 0.05;    // g
	materialAmbient[2] = 0.06625; // b
	materialAmbient[3] = 1.0f;    // a
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	// diffuse material
	materialDiffuse[0] = 0.18275; // r
	materialDiffuse[1] = 0.17;    // g
	materialDiffuse[2] = 0.22525; // b
	materialDiffuse[3] = 1.0f	  // a
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	// specular material
	materialSpecular[0] = 0.332741; // r
	materialSpecular[1] = 0.328634; // g
	materialSpecular[2] = 0.346435; // b
	materialSpecular[3] = 1.0f;     // a
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShininess = 0.3 * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(1.5f, 12.0f, 0.0f);
	gluSphere(quadric, 1.0f, 30, 30);

	// ***** 4th sphere on 1st column, pearl *****
	// ambient material
	materialAmbient[0] = 0.25;    // r
	materialAmbient[1] = 0.20725; //g
	materialAmbient[2] = 0.20725; //b
	materialAmbient[3] = 1.0f;    //a
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	// diffuse material
	materialDiffuse[0] = 1.0;   // r
	materialDiffuse[1] = 0.829; // g
	materialDiffuse[2] = 0.829; // b
	materialDiffuse[3] = 1.0f;  // a
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	// specular material
	materialSpecular[0] = 0.296648; // r
	materialSpecular[1] = 0.296648; // g
	materialSpecular[2] = 0.296648; // b
	materialSpecular[3] = 1.0f;     // a
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShininess = 0.088 * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	// geometry
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(1.0f, 11.5f, 0.0f);
	gluSphere(quadric, 1.0f, 30, 30);

	// ***** 5th sphere on 1st column, ruby *****
// ambient material
	materialAmbient[0] = 0.1745;  // r
	materialAmbient[1] = 0.01175; // g
	materialAmbient[2] = 0.01175; // b
	materialAmbient[3] = 1.0f;    // a
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	// diffuse material
	materialDiffuse[0] = 0.61424; // r
	materialDiffuse[1] = 0.04136; // g
	materialDiffuse[2] = 0.04136; // b
	materialDiffuse[3] = 1.0f;    // a
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	// specular material
	materialSpecular[0] = 0.727811; // r
	materialSpecular[1] = 0.626959; // g
	materialSpecular[2] = 0.626959; // b
	materialSpecular[3] = 1.0f;     // a
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShininess = 0.6 * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	// geometry
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(1.0f, 11.5f, 0.0f);
	gluSphere(quadric, 1.0f, 30, 30);

	// ***** 6th sphere on 1st column, turquoise *****
	// ambient material
	materialAmbient[0] = 0.1;     // r
	materialAmbient[1] = 0.18725; // g
	materialAmbient[2] = 0.1745;  // b
	materialAmbient[3] = 1.0f;    // a
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	// diffuse material
	materialDiffuse[0] = 0.396;   // r
	materialDiffuse[1] = 0.74151; // g
	materialDiffuse[2] = 0.69102; // b
	materialDiffuse[3] = 1.0f;    // a
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	// specular material
	materialSpecular[0] = 0.297254; // r
	materialSpecular[1] = 0.30829;  // g
	materialSpecular[2] = 0.306678; // b
	materialSpecular[3] = 1.0f;     // a
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShininess = 0.1 * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	// geometry
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(1.0f, 11.5f, 0.0f);
	gluSphere(quadric, 1.0f, 30, 30);

	// ***** 1st sphere on 2nd column, brass *****
// ambient material
	materialAmbient[0] = 0.329412; // r
	materialAmbient[1] = 0.223529; // g
	materialAmbient[2] = 0.027451; // b
	materialAmbient[3] = 1.0f;     // a
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	// diffuse material
	materialDiffuse[0] = 0.780392; // r
	materialDiffuse[1] = 0.568627; // g
	materialDiffuse[2] = 0.113725; // b
	materialDiffuse[3] = 1.0f;     // a
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	// specular material
	materialSpecular[0] = 0.992157; // r
	materialSpecular[1] = 0.941176; // g
	materialSpecular[2] = 0.807843; // b
	materialSpecular[3] = 1.0f;     // a
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShininess = 0.21794872 * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	// geometry
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(..., ..., 0.0f);
	gluSphere(quadric, 1.0f, 30, 30);

	// ***** 2nd sphere on 2nd column, bronze *****
// ambient material
	materialAmbient[0] = 0.2125; // r
	materialAmbient[1] = 0.1275; // g
	materialAmbient[2] = 0.054;  // b
	materialAmbient[3] = 1.0f;   // a
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	// diffuse material
	materialDiffuse[0] = 0.714;   // r
	materialDiffuse[1] = 0.4284;  // g
	materialDiffuse[2] = 0.18144; // b
	materialDiffuse[3] = 1.0f;    // a
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	// specular material
	materialSpecular[0] = 0.393548; // r
	materialSpecular[1] = 0.271906; // g
	materialSpecular[2] = 0.166721; // b
	materialSpecular[3] = 1.0f;     // a
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShininess = 0.2 * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	// geometry
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(..., ..., 0.0f);
	gluSphere(quadric, 1.0f, 30, 30);

	// ***** 3rd sphere on 2nd column, chrome *****
// ambient material
	materialAmbient[0] = 0.25; // r
	materialAmbient[1] = 0.25; // g
	materialAmbient[2] = 0.25; // b
	materialAmbient[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	// diffuse material
	materialDiffuse[0] = 0.4;  // r
	materialDiffuse[1] = 0.4;  // g
	materialDiffuse[2] = 0.4;  // b
	materialDiffuse[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	// specular material
	materialSpecular[0] = 0.774597; // r
	materialSpecular[1] = 0.774597; // g
	materialSpecular[2] = 0.774597; // b
	materialSpecular[3] = 1.0f;     // a
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShininess = 0.6 * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	// geometry
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(..., ..., 0.0f);
	gluSphere(quadric, 1.0f, 30, 30);

	// ***** 4th sphere on 2nd column, copper *****
// ambient material
	materialAmbient[0] = 0.19125; // r
	materialAmbient[1] = 0.0735;  // g
	materialAmbient[2] = 0.0225;  // b
	materialAmbient[3] = 1.0f;    // a
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	// diffuse material
	materialDiffuse[0] = 0.7038;  // r
	materialDiffuse[1] = 0.27048; // g
	materialDiffuse[2] = 0.0828;  // b
	materialDiffuse[3] = 1.0f;    // a
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	// specular material
	materialSpecular[0] = 0.256777; // r
	materialSpecular[1] = 0.137622; // g
	materialSpecular[2] = 0.086014; // b
	materialSpecular[3] = 1.0f;     // a
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShininess = 0.1 * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	// geometry
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(..., ..., 0.0f);
	gluSphere(quadric, 1.0f, 30, 30);

	// ***** 5th sphere on 2nd column, gold *****
// ambient material
	materialAmbient[0] = 0.24725; // r
	materialAmbient[1] = 0.1995;  // g
	materialAmbient[2] = 0.0745;  // b
	materialAmbient[3] = 1.0f;    // a
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	// diffuse material
	materialDiffuse[0] = 0.75164; // r
	materialDiffuse[1] = 0.60648; // g
	materialDiffuse[2] = 0.22648; // b
	materialDiffuse[3] = 1.0f;    // a
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	// specular material
	materialSpecular[0] = 0.628281; // r
	materialSpecular[1] = 0.555802; // g
	materialSpecular[2] = 0.366065; // b
	materialSpecular[3] = 1.0f;     // a
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShininess = 0.4 * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	// geometry
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(..., ..., 0.0f);
	gluSphere(quadric, 1.0f, 30, 30);

	// ***** 6th sphere on 2nd column, silver *****
// ambient material
	materialAmbient[0] = 0.19225; // r
	materialAmbient[1] = 0.19225; // g
	materialAmbient[2] = 0.19225; // b
	materialAmbient[3] = 1.0f;    // a
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	// diffuse material
	materialDiffuse[0] = 0.50754; // r
	materialDiffuse[1] = 0.50754; // g
	materialDiffuse[2] = 0.50754; // b
	materialDiffuse[3] = 1.0f;    // a
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	// specular material
	materialSpecular[0] = 0.508273; // r
	materialSpecular[1] = 0.508273; // g
	materialSpecular[2] = 0.508273; // b
	materialSpecular[3] = 1.0f;     // a
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShininess = 0.4 * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	// geometry
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(..., ..., 0.0f);
	gluSphere(quadric, 1.0f, 30, 30);

	// ***** 1st sphere on 3rd column, black *****
// ambient material
	materialAmbient[0] = 0.0;  // r
	materialAmbient[1] = 0.0;  // g
	materialAmbient[2] = 0.0;  // b
	materialAmbient[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	// diffuse material
	materialDiffuse[0] = 0.01; // r
	materialDiffuse[1] = 0.01; // g
	materialDiffuse[2] = 0.01; // b
	materialDiffuse[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	// specular material
	materialSpecular[0] = 0.50; // r
	materialSpecular[1] = 0.50; // g
	materialSpecular[2] = 0.50; // b
	materialSpecular[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShininess = 0.25 * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	// geometry
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(..., ..., 0.0f);
	gluSphere(quadric, 1.0f, 30, 30);
	// *******************************************************

	// ***** 2nd sphere on 3rd column, cyan *****
	// ambient material
	materialAmbient[0] = 0.0;  // r
	materialAmbient[1] = 0.1;  // g
	materialAmbient[2] = 0.06; // b
	materialAmbient[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	// diffuse material
	materialDiffuse[0] = 0.0;        // r
	materialDiffuse[1] = 0.50980392; // g
	materialDiffuse[2] = 0.50980392; // b
	materialDiffuse[3] = 1.0f;       // a
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	// specular material
	materialSpecular[0] = 0.50196078; // r
	materialSpecular[1] = 0.50196078; // g
	materialSpecular[2] = 0.50196078; // b
	materialSpecular[3] = 1.0f;       // a
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShininess = 0.25 * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	// geometry
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(..., ..., 0.0f);
	gluSphere(quadric, 1.0f, 30, 30);
	// *******************************************************

	// ***** 3rd sphere on 2nd column, green *****
	// ambient material
	materialAmbient[0] = 0.0;  // r
	materialAmbient[1] = 0.0;  // g
	materialAmbient[2] = 0.0;  // b
	materialAmbient[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	// diffuse material
	materialDiffuse[0] = 0.1;  // r
	materialDiffuse[1] = 0.35; // g
	materialDiffuse[2] = 0.1;  // b
	materialDiffuse[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	// specular material
	materialSpecular[0] = 0.45; // r
	materialSpecular[1] = 0.55; // g
	materialSpecular[2] = 0.45; // b
	materialSpecular[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShininess = 0.25 * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	// geometry
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(..., ..., 0.0f);
	gluSphere(quadric, 1.0f, 30, 30);
	// *******************************************************

	// ***** 4th sphere on 3rd column, red *****
	// ambient material
	materialAmbient[0] = 0.0;  // r
	materialAmbient[1] = 0.0;  // g
	materialAmbient[2] = 0.0;  // b
	materialAmbient[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	// diffuse material
	materialDiffuse[0] = 0.5;  // r
	materialDiffuse[1] = 0.0;  // g
	materialDiffuse[2] = 0.0;  // b
	materialDiffuse[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	// specular material
	materialSpecular[0] = 0.7;  // r
	materialSpecular[1] = 0.6;  // g
	materialSpecular[2] = 0.6;  // b
	materialSpecular[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShininess = 0.25 * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	// geometry
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(..., ..., 0.0f);
	gluSphere(quadric, 1.0f, 30, 30);
	// *******************************************************

	// ***** 5th sphere on 3rd column, white *****
	// ambient material
	materialAmbient[0] = 0.0;  // r
	materialAmbient[1] = 0.0;  // g
	materialAmbient[2] = 0.0;  // b
	materialAmbient[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	// diffuse material
	materialDiffuse[0] = 0.55; // r
	materialDiffuse[1] = 0.55; // g
	materialDiffuse[2] = 0.55; // b
	materialDiffuse[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	// specular material
	materialSpecular[0] = 0.70; // r
	materialSpecular[1] = 0.70; // g
	materialSpecular[2] = 0.70; // b
	materialSpecular[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShininess = 0.25 * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	// geometry
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(..., ..., 0.0f);
	gluSphere(quadric, 1.0f, 30, 30);
	// *******************************************************

	// ***** 6th sphere on 3rd column, yellow plastic *****
	// ambient material
	materialAmbient[0] = 0.0;  // r
	materialAmbient[1] = 0.0;  // g
	materialAmbient[2] = 0.0;  // b
	materialAmbient[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	// diffuse material
	materialDiffuse[0] = 0.5;  // r
	materialDiffuse[1] = 0.5;  // g
	materialDiffuse[2] = 0.0;  // b
	materialDiffuse[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	// specular material
	materialSpecular[0] = 0.60; // r
	materialSpecular[1] = 0.60; // g
	materialSpecular[2] = 0.50; // b
	materialSpecular[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShininess = 0.25 * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	// geometry
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(..., ..., 0.0f);
	gluSphere(quadric, 1.0f, 30, 30);

	// ***** 1st sphere on 4th column, black *****
// ambient material
	materialAmbient[0] = 0.02; // r
	materialAmbient[1] = 0.02; // g
	materialAmbient[2] = 0.02; // b
	materialAmbient[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	// diffuse material
	materialDiffuse[0] = 0.01; // r
	materialDiffuse[1] = 0.01; // g
	materialDiffuse[2] = 0.01; // b
	materialDiffuse[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	// specular material
	GL_SPECULAR[0] = 0.4;  // r
	GL_SPECULAR[1] = 0.4;  // g
	GL_SPECULAR[2] = 0.4;  // b
	GL_SPECULAR[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShininess = 0.078125 * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	// geometry
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(..., ..., 0.0f);
	gluSphere(quadric, 1.0f, 30, 30);
	// *******************************************************

	// ***** 2nd sphere on 4th column, cyan *****
	// ambient material
	materialAmbient[0] = 0.0;  // r
	materialAmbient[1] = 0.05; // g
	materialAmbient[2] = 0.05; // b
	materialAmbient[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	// diffuse material
	materialDiffuse[0] = 0.4;  // r
	materialDiffuse[1] = 0.5;  // g
	materialDiffuse[2] = 0.5;  // b
	materialDiffuse[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	// specular material
	materialSpecular[0] = 0.04; // r
	materialSpecular[1] = 0.7;  // g
	materialSpecular[2] = 0.7;  // b
	materialSpecular[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShininess = 0.078125 * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	// geometry
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(..., ..., 0.0f);
	gluSphere(quadric, 1.0f, 30, 30);
	// *******************************************************

	// ***** 3rd sphere on 4th column, green *****
	// ambient material
	materialAmbient[0] = 0.0;  // r
	materialAmbient[1] = 0.05; // g
	materialAmbient[2] = 0.0;  // b
	materialAmbient[3] = 1.0f; // a
	glMaterialfv(..., ..., ...);

	// diffuse material
	materialDiffuse[0] = 0.4;  // r
	materialDiffuse[1] = 0.5;  // g
	materialDiffuse[2] = 0.4;  // b
	materialDiffuse[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	// specular material
	materialSpecular[0] = 0.04; // r
	materialSpecular[1] = 0.7;  // g
	materialSpecular[2] = 0.04; // b
	materialSpecular[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShininess = 0.078125 * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	// geometry
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(..., ..., 0.0f);
	gluSphere(quadric, 1.0f, 30, 30);
	// *******************************************************

	// ***** 4th sphere on 4th column, red *****
	// ambient material
	materialAmbient[0] = 0.05; // r
	materialAmbient[1] = 0.0;  // g
	materialAmbient[2] = 0.0;  // b
	materialAmbient[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	// diffuse material
	materialDiffuse[0] = 0.5;  // r
	materialDiffuse[1] = 0.4;  // g
	materialDiffuse[2] = 0.4;  // b
	materialDiffuse[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	// specular material
	materialSpecular[0] = 0.7;  // r
	materialSpecular[1] = 0.04; // g
	materialSpecular[2] = 0.04; // b
	materialSpecular[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_SPECULAR, ...);

	// shininess
	materialShininess = 0.078125 * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, G);

	// geometry
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(..., ..., 0.0f);
	gluSphere(quadric, 1.0f, 30, 30);
	// *******************************************************

	// ***** 5th sphere on 4th column, white *****
	// ambient material
	materialAmbient[0] = 0.05; // r
	materialAmbient[1] = 0.05; // g
	materialAmbient[2] = 0.05; // b
	materialAmbient[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	// diffuse material
	materialDiffuse[0] = 0.5;  // r
	materialDiffuse[1] = 0.5;  // g
	materialDiffuse[2] = 0.5;  // b
	materialDiffuse[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	// specular material
	materialSpecular[0] = 0.7;  // r
	materialSpecular[1] = 0.7;  // g
	materialSpecular[2] = 0.7;  // b
	materialSpecular[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShininess = 0.078125 * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	// geometry
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(..., ..., 0.0f);
	gluSphere(quadric, 1.0f, 30, 30);
	// *******************************************************

	// ***** 6th sphere on 4th column, yellow rubber *****
	// ambient material
	materialAmbient[0] = 0.05; // r
	materialAmbient[1] = 0.05; // g
	materialAmbient[2] = 0.0;  // b
	materialAmbient[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialShininess);

	// diffuse material
	materialDiffuse[0] = 0.5;  // r
	materialDiffuse[1] = 0.5;  // g
	materialDiffuse[2] = 0.4;  // b
	materialDiffuse[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	// specular material
	materialSpecular[0] = 0.7;  // r
	materialSpecular[1] = 0.7;  // g
	materialSpecular[2] = 0.04; // b
	materialSpecular[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

	// shininess
	materialShininess = 0.078125 * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	// geometry
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(19.5f, 1.5f, 0.0f);
	gluSphere(quadric, 1.0f, 30, 30);

	SwapBuffers(ghdc);
}

void update(void)
{
	// Code
	if (keyPressed == 1)
	{
		angleForXRotation = angleForXRotation + 0.5f;

		if (angleForXRotation >= 360.0f)
		{
			angleForXRotation = angleForXRotation - 360.0f;
		}

	}

	if (keyPressed == 2)
	{
		angleForYRotation = angleForYRotation + 0.5f;

		if (angleForYRotation >= 360.0f)
		{
			angleForYRotation = angleForYRotation - 360.0f;
		}
	}


	if (keyPressed == 3)
	{
		angleForZRotation = angleForYRotation + 0.5f;

		if (angleForZRotation >= 360.0f)
		{
			angleForZRotation = angleForZRotation  - 360.0f;
		}
	}
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

	if (quadric)
	{
		gluDeleteQuadric(quadric);
		quadric = NULL;
	}

	// Close The LogFile
	if (gpfile)
	{
		fprintf(gpfile, "Program Ended Successfully");
		fclose(gpfile);
		gpfile = NULL;
	}
}

