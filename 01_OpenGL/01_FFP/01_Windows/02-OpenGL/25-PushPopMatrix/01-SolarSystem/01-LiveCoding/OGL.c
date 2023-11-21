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
int day = 0;
int year = 0;

GLUquadric* quadric = NULL;

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
		case 'F':							// FallThrough
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

		case 'D':
			day = (day + 6) % 360;
			break;
		case 'd':
			day = (day - 6) % 360;
			break;

		case 'Y':
			year = (year + 3) % 360;
			break;
		case 'y':
			year = (year - 3) % 360;
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

	// Initialize Quadric
	quadric = gluNewQuadric();

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
	// Code
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Depth-Buffer Added For 3D(Depth)
		
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// View Transformation
	gluLookAt(0.0f, 0.0f, 5.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f);

	// Push The Matrix 
	glPushMatrix();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glColor3f(1.0f, 0.5f, 0.0f);
	// Draw The Sun
	gluSphere(quadric, 0.75f, 50, 50);
		
	glRotatef((GLfloat)year, 0.0f, 1.0f, 0.0f);
	glTranslatef(1.5f, 0.0f, 0.0f); 
	
	glRotatef((GLfloat)day, 0.0f, 1.0f, 0.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);

	glColor3f(0.4f, 0.9f, 1.0f);
	// Draw The Earth
	gluSphere(quadric, 0.2f, 15, 15);

	//Draw The Moon
	/*glRotatef((GLfloat)day, 0.0f, 1.0f, 0.0f);
	glTranslatef(1.2f, 0.0f, 0.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	gluSphere(quadric, 0.2f, 15, 15);*/
	
	//Pop The Matrix
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

