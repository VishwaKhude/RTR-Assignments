// Windows Header Files
#include <windows.h>     //sdk win32api
#include <stdio.h>      //file io
#include <stdlib.h>     //exit()
#include <time.h>

#include<mmsystem.h>

//OpenGL Header File
#include <GL/gl.h>

#include <GL/glu.h>

#include "a_aa_aai.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

//macros
#define WIN_WIDTH 800
#define WIN_HEIGHT 600

#pragma comment(lib, "winmm.lib")

//link with opengl library
#pragma comment(lib,"opengl32.lib")

#pragma	comment(lib,"glu32.lib")

// Global Function Declarations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// Global Variable Declarations

// for fileIO
FILE *gpFILE = NULL;   

//for fullscreen
HWND ghwnd = NULL;  
DWORD dwStyle = 0;
WINDOWPLACEMENT wpPrev = { sizeof(WINDOWPLACEMENT) };
BOOL gbFullscreen = FALSE;

//for active/inactive
BOOL gbActiveWindow = FALSE;

//opengl related global variables
HDC ghdc = NULL;
HGLRC ghrc = NULL;

GLfloat cangle = -360.0f;

//texture variable
GLuint texture_baby = 0; //5
GLuint texture_mama = 0;   //3
GLuint texture_Mom_baby = 0;  //1
GLuint texture_kite = 0; //4
GLuint texture_corn = 0;  //2

//scenes
#define A_AA_AAI 1 
#define M_M_MAKA 2
#define MA_MA 3
#define _kite 4
#define _baby 8

float b_x = 0.0;
GLfloat b = 0.0f;

//for time
int Global_time = 0;
GLfloat fTimer = 0.0f;
SYSTEMTIME  timer_in_Min;
int iCurrent_Time;

int Present_Scene = A_AA_AAI;

GLfloat lerp(GLfloat start, GLfloat end, GLfloat t)
{
	return (start + (end - start) * t);
}

//Entry Point Function
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
	//fucntion declaration
	int initialize(void);
	void uninitialize(void);
	void display(void);
	void update(void);

	//Local Variable Declarations
	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG msg;                                                                                                                              
	TCHAR szAppName[] = TEXT(" VMKWindow ");

	int iResult = 0;

	BOOL bDone = FALSE;   //game loop

    //for centering
	int ScreenHeight;      
    int ScreenWidth;
    int WindowHeight = 600;
    int WindowWidth = 800;
    
    ScreenWidth = GetSystemMetrics(SM_CXSCREEN);
    ScreenHeight = GetSystemMetrics(SM_CYSCREEN);

    int X = ScreenWidth / 2 - WindowWidth / 2;
    int Y = ScreenHeight / 2 - WindowHeight / 2;


	//code
	gpFILE = fopen("Log.txt", "w");
	if (gpFILE == NULL)
	{
		MessageBox(NULL, TEXT("Log File Cannot Be Opend"), TEXT("Error"), MB_OK | MB_ICONERROR);
		exit(0);
	}
	fprintf(gpFILE, "Program Started Successfully\n");
	
	//WNDCEX initialization
	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.lpfnWndProc = WndProc;
	wndclass.hInstance = hInstance;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON));
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.lpszClassName = szAppName;
	wndclass.lpszMenuName = NULL;
	wndclass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON));

	//Register WNDCLASSEX
	RegisterClassEx(&wndclass);

	//Create Window
	hwnd = CreateWindow(szAppName,
		TEXT("Vishwa Mahesh Khude"),
	    WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE,
		X,
		Y,
		WIN_WIDTH,
		WIN_HEIGHT,
		NULL,
		NULL,
		hInstance,
		NULL);

	ghwnd = hwnd;

	//Initialization
	iResult = initialize();
	if (iResult != 0)
	{
		MessageBox(hwnd, TEXT("initialize() failed"), TEXT("Error"), MB_OK | MB_ICONERROR);
		DestroyWindow(hwnd);
	}
	
	// Show The Window
	ShowWindow(hwnd, iCmdShow);
	SetForegroundWindow(hwnd);
	SetFocus(hwnd);

	// Paint/Re-Draw The Window
	//UpdateWindow(hwnd);

	// Message-loop
	//game-loop
	while (bDone == FALSE)
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
				bDone = TRUE;
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			if (gbActiveWindow == TRUE)
			{
				//RENDER
				display();

				//UPDATE
				update();
			}
			
		}
	}
	uninitialize();

	return((int)msg.wParam);
}


//CallBack Function
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	// Function Declarations
	void ToggleFullScreen(void);
	void resize(int, int);

	// code
	switch (iMsg)
	{
	case WM_SETFOCUS:    
		gbActiveWindow = TRUE;
		break;

	case WM_KILLFOCUS:
		gbActiveWindow = FALSE;
		break;

	case WM_SIZE:
		resize(LOWORD(lParam), HIWORD(lParam));
		break;

    case WM_ERASEBKGND:
		return(0);
		break;

	case WM_KEYDOWN:
		switch (LOWORD(wParam))
		{
		case VK_ESCAPE:

			DestroyWindow(hwnd);
			break;

		}
		break; 
	
	case WM_CHAR:
		switch(LOWORD(wParam))
		{
	    case 's':
        case 'S':
            //if (b == FALSE)
            {
              //  bStartAnimation = TRUE;
                PlaySound("A Aa Aaai.wav", NULL, SND_ASYNC| SND_FILENAME);
            }
            break;
		
		case 'F':
		case 'f':
			if(gbFullscreen == FALSE)
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
		DestroyWindow(hwnd);
		break;

	case WM_DESTROY :
		if (gpFILE)
		{
			fprintf(gpFILE,"Prgram Ended Successfully...\n");
			fclose(gpFILE);
			gpFILE = NULL;
		}
		
		PostQuitMessage(0);
		break;
	default :
		break;
	}

	return(DefWindowProc(hwnd, iMsg, wParam, lParam));
}

void ToggleFullScreen(void)
{
	// Local Variable Declarations
	MONITORINFO mi = { sizeof(MONITORINFO) };

	// Code
	if (gbFullscreen == FALSE)
	{
		dwStyle = GetWindowLong(ghwnd, GWL_STYLE); 

		if (dwStyle & WS_OVERLAPPEDWINDOW)
		{
			if (GetWindowPlacement(ghwnd, &wpPrev) && GetMonitorInfo(MonitorFromWindow(ghwnd, MONITORINFOF_PRIMARY), &mi)) 
			{
				SetWindowLong(ghwnd, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW);  // ~Remove Contents
				SetWindowPos(ghwnd, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top, mi.rcMonitor.right - mi.rcMonitor.left, mi.rcMonitor.bottom - mi.rcMonitor.top, SWP_NOZORDER | SWP_FRAMECHANGED);
			}
		}
		ShowCursor(FALSE);
	}
	else
	{
		SetWindowPlacement(ghwnd, &wpPrev);

		SetWindowLong(ghwnd, GWL_STYLE, dwStyle |  WS_OVERLAPPEDWINDOW);

		SetWindowPos(ghwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_FRAMECHANGED);

		ShowCursor(TRUE);
	}
}

int initialize(void)
{

	//function declarations
	void resize(int, int);
	BOOL loadGLTexture(GLuint*, TCHAR[]);
	GLuint createTexture2D(const char* filePath);

	//code
	//variable declaration
	PIXELFORMATDESCRIPTOR PFD;
	int iPixelFormatIndex = 0;
	BOOL bResult;

	//initializing PFD struct with 0
	ZeroMemory(&PFD, sizeof(PIXELFORMATDESCRIPTOR));

	//initialization of PixelFormatDescriptor
	PFD.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	PFD.nVersion = 1;
	PFD.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER; 
	PFD.iPixelType = PFD_TYPE_RGBA;
	PFD.cColorBits = 32; 
	PFD.cRedBits = 8;
	PFD.cGreenBits = 8;
	PFD.cBlueBits = 8;
	PFD.cAlphaBits = 8;
	PFD.cDepthBits = 32;

	//get dc
	ghdc = GetDC(ghwnd);
	if (ghdc == NULL)
	{
		fprintf(gpFILE, "GetDC Failed\n");
		return(-1);
	}

	//choosing pixel format which closlely matches to our initialised PFD
	iPixelFormatIndex = ChoosePixelFormat(ghdc, &PFD);

	if (iPixelFormatIndex == 0)
	{
		fprintf(gpFILE, "Choose PixelFormat function() failed");
		return(-2);
	}

	//set obtain PixelFormat
	if (SetPixelFormat(ghdc, iPixelFormatIndex, &PFD) == FALSE)
	{
		fprintf(gpFILE, "SetPixelFormat\n");
		return(-3);
	}
	
	//create opengl context from device context
	ghrc = wglCreateContext(ghdc);
	if (ghrc == NULL)
	{
		fprintf(gpFILE, "wglCreateContext() failed");
		return(-4);
	}
	
	//make rendering context current
	if (wglMakeCurrent(ghdc, ghrc) == FALSE)
	{
		fprintf(gpFILE, "wglMakeCurrent() Fialed\n");
		return(-5);
	}

	// Enabling Depth
	glShadeModel(GL_SMOOTH);						   // Beautification Line (optional)
	glClearDepth(1.0f);								   // Compulsory
	glEnable(GL_DEPTH_TEST);						   // Compulsory
	glDepthFunc(GL_LEQUAL);							   // Compulsory
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // Beuatification (optional)


	GetSystemTime(&timer_in_Min);

	//set the clear color of window to blue
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	//create texture
	texture_Mom_baby = createTexture2D("MomBaby.png");
	texture_corn = createTexture2D("Corn.png");
	texture_mama = createTexture2D("Mama.png");
	texture_kite = createTexture2D("kite_2.png");
	texture_baby = createTexture2D("baby.png");
	
	//tell opengl to enable texture
	glEnable(GL_TEXTURE_2D);

	resize(WIN_WIDTH, WIN_HEIGHT);
	//here opengl starts

	return(0);
}

BOOL loadGLTexture(GLuint* texture, TCHAR imageResourceID[])
{
	//LOCAL VARIABLE declaration
	HBITMAP hBitmap = NULL;
	BITMAP bmp;

	hBitmap = (HBITMAP)LoadImage(GetModuleHandle(NULL), imageResourceID, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);	if (hBitmap == NULL)
	{
		fprintf(gpFILE, "Load image failed\n");
		return(FALSE);
	}
	
	//get image data
	GetObject(hBitmap, sizeof(BITMAP), &bmp);

	//create opengl texture
	glGenTextures(1, texture);

	//bind to 
	glBindTexture(GL_TEXTURE_2D, *texture);

	//unpack & allinment
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	//set texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	//create multiple mipmap images
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, bmp.bmWidth, bmp.bmHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, (void *)bmp.bmBits);

	//unbind
	glBindTexture(GL_TEXTURE_2D, 0);

	DeleteObject(hBitmap);

	hBitmap = NULL;

	return TRUE; 
}

int TimeCal(SYSTEMTIME currentTime, SYSTEMTIME StartTime)
{
	int CurrentSeconds = currentTime.wMinute * 60 + currentTime.wSecond;

	int StartSeconds = StartTime.wMinute * 60 + StartTime.wSecond;

	return CurrentSeconds - StartSeconds;
}

void resize(int width, int height)
{
	//code
	if (height <= 0)
		height = 1;

	//resize
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);

	glViewport(0, 0, (GLsizei)width, (GLsizei)height);

}

void display(void)
{
	void aai();
	void maka();
	void mama();
	void patang();
	void chandr();
	void watch();
	void thava();
	void bal();

	SYSTEMTIME CurrentTime;
	//code
	GetSystemTime(&CurrentTime);

	Global_time = TimeCal(CurrentTime, timer_in_Min);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
  
	glTranslatef(0.0f, 0.0f, -4.0f);
	glScalef(3.0f, 1.75f, 1.75f);

	switch (Present_Scene)
	{
	case A_AA_AAI:
		if (Global_time <= 8)
		{
			aai();
		}
		else
		{
			Present_Scene = M_M_MAKA;
		}
		break;

	case M_M_MAKA:
		if (Global_time <= 10)
		{
			maka();
		}
		else
		{
			Present_Scene = MA_MA;
		}
		break;

	case MA_MA:
		if (Global_time <= 12)
		{
			mama();
		}
		else if(Global_time <= 15)
		{
			aai();
		}
		else if (Global_time <= 17)
		{
			maka();
		}
		else if (Global_time < 18)
		{
			mama();
		}
		else
		{
			Present_Scene = _kite;
		}
		break;

	case _kite:
		if (Global_time <= 25)
		{
			mama();		
		}
		if (Global_time >= 40)
		{
			patang();
		}
		if (Global_time >= 44)
		{
			chandr();
		}
		if (Global_time >=47)
		{
			patang();
		}
		if (Global_time >=51)
		{
			chandr();
		}
		if (Global_time >=54)
		{
			watch();
		}
		if (Global_time >= 57)
		{
			thava();
		}
		if (Global_time >= 59)
		{
			bal();
		}
		if (Global_time >= 66)
		{
			aai();
		}
		
		
		
		
		

		break;


	default:
		break;
	}
	SwapBuffers(ghdc);
}

void update(void)
{
	//code
}

void uninitialize(void)
{
	//fucntion declarations
	void ToggleFullScreen(void);

	//code
	//if application is exiting in fullscreen
	if (gbFullscreen == TRUE)
	{
		ToggleFullScreen();
		gbFullscreen = FALSE;
	}	

	//make the hdc as current context
	if (wglGetCurrentContext() == ghrc )
	{
		wglMakeCurrent(NULL, NULL);
	}

	//Delete rendering context
	if (ghrc)
	{
		wglDeleteContext(ghrc);
		ghrc = NULL;
	}

	//release the hdc
	if (ghdc)
	{
		ReleaseDC(ghwnd,ghdc);
		ghdc = NULL;
	}
	
	//DestroyWindow
	if (ghwnd)
	{
		DestroyWindow(ghwnd);
		ghwnd = NULL;
	}

	if (texture_Mom_baby)
	{
		glDeleteTextures(1, &texture_Mom_baby);
		texture_Mom_baby= 0;
	}

	if (texture_corn)
	{
		glDeleteTextures(1, &texture_corn);
		texture_corn = 0;
	}

	if (texture_mama)
	{
		glDeleteTextures(1, &texture_mama);
		texture_mama = 0;
	}

	if (texture_kite)
	{
		glDeleteTextures(1, &texture_kite);
		texture_kite = 0;
	}

	if (texture_baby)
	{
		glDeleteTextures(1, &texture_baby);
		texture_baby= 0;
	}

	//close the log file
		if (gpFILE)
		{
			fprintf(gpFILE,"Program Ended Successfully...\n");
			fclose(gpFILE);
			gpFILE = NULL;
		}
}

void aai()
{
	glBindTexture(GL_TEXTURE_2D, texture_Mom_baby);

	glBegin(GL_QUADS);

	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.0f,1.0f, 0.0f);

	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, 0.0f);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-1.0f, -1.0f, 0.0f);

	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(1.0f, -1.0f, 0.0f);

	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);
}


void maka()
{
	glBindTexture(GL_TEXTURE_2D, texture_corn);

	glBegin(GL_QUADS);

	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.0f,1.0f, 0.0f);

	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, 0.0f);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-1.0f, -1.0f, 0.0f);

	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(1.0f, -1.0f, 0.0f);

	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);
}

void mama()
{
	glBindTexture(GL_TEXTURE_2D, texture_mama);

	glBegin(GL_QUADS);

	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.0f,1.0f, 0.0f);

	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, 0.0f);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-1.0f, -1.0f, 0.0f);

	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(1.0f, -1.0f, 0.0f);

	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);
}

void patang()
{
	glBindTexture(GL_TEXTURE_2D, texture_kite);

	glBegin(GL_QUADS);

	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.0f,1.0f, 0.0f);

	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, 0.0f);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-1.0f, -1.0f, 0.0f);

	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(1.0f, -1.0f, 0.0f);

	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);
}


void baby()
{
	glBindTexture(GL_TEXTURE_2D, texture_baby);

	glBegin(GL_QUADS);

	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.0f,1.0f, 0.0f);

	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, 0.0f);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-1.0f, -1.0f, 0.0f);

	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(1.0f, -1.0f, 0.0f);

	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);
}
