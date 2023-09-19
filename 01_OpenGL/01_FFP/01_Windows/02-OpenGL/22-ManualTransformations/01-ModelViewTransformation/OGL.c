// OpenGL starts here...
// Common Windows Header Files
#include <windows.h>		  // same as <stdio.h> library function, // includes around 3,50,000 API's, Win32 API
#include <stdio.h>            // For File I/O
#include <stdlib.h>           // For exit()				
#define _USE_MATH_DEFINES 1  // Enable Mathematical Operations
#include <math.h>

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
GLfloat cangle = 0.0f;

GLfloat identityMatrix[16];
GLfloat translationMatrix[16];
GLfloat scaleMatrix[16];
GLfloat rotationMatrix_X[16];
GLfloat rotationMatrix_Y[16];
GLfloat rotationMatrix_Z[16];


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
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // Optional (Nicest Correction Of 

	// Set The Clearcolor of window to blue
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // alpha(transperancy, opaqueness)

	//Initialization Of Matrices
	//Identity Matrix
	identityMatrix[0] = 1.0f;
	identityMatrix[1] = 0.0f;
	identityMatrix[2] = 0.0f;
	identityMatrix[3] = 0.0f;

	identityMatrix[4] = 0.0f;
	identityMatrix[5] = 1.0f;
	identityMatrix[6] = 0.0f;
	identityMatrix[7] = 0.0f;

	identityMatrix[8] = 0.0f;
	identityMatrix[9] = 0.0f;
	identityMatrix[10] = 1.0f;
	identityMatrix[11] = 0.0f;

	identityMatrix[12] = 0.0f;
	identityMatrix[13] = 0.0f;
	identityMatrix[14] = 0.0f;
	identityMatrix[15] = 1.0f;

	//Translation Matrix
	translationMatrix[0] = 1.0f;
	translationMatrix[1] = 0.0f;
	translationMatrix[2] = 0.0f;
	translationMatrix[3] = 0.0f;

	translationMatrix[4] = 0.0f;
	translationMatrix[5] = 1.0f;
	translationMatrix[6] = 0.0f;
	translationMatrix[7] = 0.0f;

	translationMatrix[8] = 0.0f;
	translationMatrix[9] = 0.0f;
	translationMatrix[10] = 1.0f;
	translationMatrix[11] = 0.0f;

	translationMatrix[12] = 0.0f;
	translationMatrix[13] = 0.0f;
	translationMatrix[14] = -5.0f;
	translationMatrix[15] = 1.0f;

	//Scale Matrix
	scaleMatrix[0] = 0.75f;
	scaleMatrix[1] = 0.0f;
	scaleMatrix[2] = 0.0f;
	scaleMatrix[3] = 0.0f;

	scaleMatrix[4] = 0.0f;
	scaleMatrix[5] = 0.75f;
	scaleMatrix[6] = 0.0f;
	scaleMatrix[7] = 0.0f;

	scaleMatrix[8] = 0.0f;
	scaleMatrix[9] = 0.0f;
	scaleMatrix[10] = 0.75f;
	scaleMatrix[11] = 0.0f;

	scaleMatrix[12] = 0.0f;
	scaleMatrix[13] = 0.0f;
	scaleMatrix[14] = 0.0f;
	scaleMatrix[15] = 1.0f;

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

void display(void)
{
	static GLfloat angle = 0.0f;

	// Code
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Depth-Buffer Added For 3D(Depth)
		
	glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
	glLoadMatrixf(identityMatrix);
	
	//glTranslatef(0.0f, 0.0f, -6.0f); // -5.0f means towards the screen on z-axis (-)on z-axis
	glMultMatrixf(translationMatrix);
	//glScalef(0.75f, 0.75f, 0.75f);
	glMultMatrixf(scaleMatrix);

	/*glRotatef(cangle, 1.0f, 0.0f, 0.0f);
	glRotatef(cangle, 1.0f, 1.0f, 0.0f);
	glRotatef(cangle, 1.0f, 0.0f, 1.0f); */

	angle = cangle * (M_PI / 180.0f);

	rotationMatrix_X[0] = 1.0f;
	rotationMatrix_X[1] = 0.0f;
	rotationMatrix_X[2] = 0.0f;
	rotationMatrix_X[3] = 0.0f;
	
	rotationMatrix_X[4] = 0.0f;
	rotationMatrix_X[5] = cos(angle);
	rotationMatrix_X[6] = sin(angle);
	rotationMatrix_X[7] = 0.0f;
	
	rotationMatrix_X[8] = 0.0f;
	rotationMatrix_X[9] = -sin(angle);
	rotationMatrix_X[10] = cos(angle);
	rotationMatrix_X[11] = 0.0f;
	
	rotationMatrix_X[12] = 0.0f;
	rotationMatrix_X[13] = 0.0f;
	rotationMatrix_X[14] = 0.0f;
	rotationMatrix_X[15] = 1.0f;

	//Y-rotation Matrix
	rotationMatrix_Y[0] = cos(angle);
	rotationMatrix_Y[1] = 0.0f;
	rotationMatrix_Y[2] = -sin(angle);
	rotationMatrix_Y[3] = 0.0f;
	
	rotationMatrix_Y[4] = 0.0f;
	rotationMatrix_Y[5] = 1.0f;
	rotationMatrix_Y[6] = 0.0f;
	rotationMatrix_Y[7] = 0.0f;
	
	rotationMatrix_Y[8] = sin(angle);
	rotationMatrix_Y[9] = 0.0f;
	rotationMatrix_Y[10] = cos(angle);
	rotationMatrix_Y[11] = 0.0f;

	rotationMatrix_Y[12] = 0.0f;
	rotationMatrix_Y[13] = 0.0f;
	rotationMatrix_Y[14] = 0.0f;
	rotationMatrix_Y[15] = 1.0f;

	rotationMatrix_Z[0] = cos(angle);
	rotationMatrix_Z[1] = sin(angle);
	rotationMatrix_Z[2] = 0.0f;
	rotationMatrix_Z[3] = 0.0f;

	rotationMatrix_Z[4] = -sin(angle);
	rotationMatrix_Z[5] = cos(angle);
	rotationMatrix_Z[6] = 0.0f;
	rotationMatrix_Z[7] = 0.0f;

	rotationMatrix_Z[8] = 0.0f;
	rotationMatrix_Z[9] = 0.0f;
	rotationMatrix_Z[10] = 1.0f;
	rotationMatrix_Z[11] = 0.0f;

	rotationMatrix_Z[12] = 0.0f;
	rotationMatrix_Z[13] = 0.0f;
	rotationMatrix_Z[14] = 0.0f;
	rotationMatrix_Z[15] = 1.0f;

	glMultMatrixf(rotationMatrix_X);
	glMultMatrixf(rotationMatrix_Y);
	glMultMatrixf(rotationMatrix_Z);

	glBegin(GL_QUADS);

	//Front Face
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 1.0f, 1.0f); // Left Top
	glVertex3f(-1.0f, 1.0f, 1.0f); // Right Top
	glVertex3f(-1.0f, -1.0f, 1.0f); // Left Bottom
	glVertex3f(1.0f, -1.0f, 1.0f); // Right Bottom

	//Right Face
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(1.0f, 1.0f, -1.0f); // Left Top
	glVertex3f(1.0f, 1.0f, 1.0f);  // Right Top
	glVertex3f(1.0f, -1.0f, 1.0f);  // Left Bottom
	glVertex3f(1.0f, -1.0f, -1.0f);  // Right Bottom

	//Back Face
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f); // Left Top
	glVertex3f(1.0f, 1.0f, -1.0f); // Right Top
	glVertex3f(1.0f, -1.0f, -1.0f); // Left Bottom
	glVertex3f(-1.0f, -1.0f, -1.0f); //Right Bottom

	//Left Face
	glColor3f(0.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f); //Left Top
	glVertex3f(-1.0f, 1.0f, -1.0f); // Right Top
	glVertex3f(-1.0f, -1.0f, -1.0f); //Left Bottom
	glVertex3f(-1.0f, -1.0f, 1.0f); // Right Bottom

	//Top Face
	glColor3f(1.0f, 0.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);

	//Bottom Face
	glColor3f(1.0f, 1.0f, 0.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);

	glEnd();

	SwapBuffers(ghdc);
}

void update(void)
{
	// Code
	cangle = cangle - 0.1f;
	if (cangle >= 360.0f);
	cangle = cangle + 360.0f;
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

