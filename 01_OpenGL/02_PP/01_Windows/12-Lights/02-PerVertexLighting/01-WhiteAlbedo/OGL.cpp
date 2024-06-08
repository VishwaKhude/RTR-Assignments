// OpenGL starts here...
// Common Windows Header Files
#include <windows.h> // same as <stdio.h> library function, // includes around 3,50,000 API's, Win32 API
#include <stdio.h>	 // For File I/O
#include <stdlib.h>	 // For exit()

// OpenGl Header Files
#include <gl/glew.h> // This must be before gl/GL.h
#include <gl/GL.h>	 // '\' also works

// header file for programmable pipeline
#include "vmath.h"
using namespace vmath;

#include "OGL.h" // Our HeaderFile
#include "Sphere.h"

// Macros
#define WIN_WIDTH 800
#define WIN_HEIGHT 600

// link with opengl library
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "Sphere.lib")

// OpenGl Related Global Variables
HDC ghdc = NULL;
HGLRC ghrc = NULL;

GLuint shaderProgramObject = 0;
enum
{
	AMC_ATTRIBUTE_POSITION = 0,
	AMC_ATTRIBUTE_COLOR,
	AMC_ATTRIBUTE_NORMAL
};

// for square
GLuint vbo_position = 0;
GLuint vbo_color_cube = 0;
GLuint vbo_normal = 0;
GLuint vao = 0;
GLuint vbo_texcoord = 0;
GLuint vbo_elements = 0;
GLuint angle_sphere = 0;
GLuint numSphereElements = 0;
GLuint gNumVertices = 0;

// Uniform

GLuint viewMatrixUniform = 0;
GLuint modelMatrixUniform = 0;
GLuint projectionViewMatrixUniform = 0;
GLuint lAmbientUniform = 0;
GLuint lSpecularUniform = 0;
GLuint lDiffuseUniform = 0;
GLuint materialDiffuseUniform = 0;
GLuint materialAmbientUniform = 0;
GLuint materialSpecularUniform = 0;
GLuint materialShininessUniform = 0;
GLuint lightPositionUniform = 0;
GLuint keyPressUniform = 0;

BOOL bLightingEnabled = FALSE;
BOOL bAnimationEnabled = FALSE;

// For lighting

GLfloat lightAmbient[] = {0.1f, 0.1f, 0.1f, 1.0f};	// Grey ambient light
GLfloat lightDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};	// White diffuse light
GLfloat lightSpecular[] = {1.0f, 1.0f, 1.0f, 1.0f}; // White specular light
GLfloat lightPosition[] = {100.0f, 100.0f, 100.0f, 1.0f};

GLfloat materialAmbient[] = {0.0f, 0.0f, 0.0f, 1.0f}; // Ambient material black
GLfloat materialDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat materialSpecular[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat materialShininess = 50.0f;

GLUquadric *quadric = NULL;

mat4 perspectiveProjectionMatrix; // mat4 is in vmath.h

// Global Function Declarations (Global because called by OS, _far, _pascal)
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM); // (WndProc(Window Procedure) - Hungarian(Charles Simonee) Notation used) (CALLBACK FUNCTIONS), (HWND - Handle Window), (WPARAM - 16 Bit), (LPARAM - 32Bit)

// Global Variable Declarations
FILE *gpfile = NULL; // Global Pointer

HWND ghwnd = NULL;
BOOL gbActive = FALSE;
DWORD dwStyle = 0;
WINDOWPLACEMENT wpPrev = {sizeof(WINDOWPLACEMENT)};
BOOL gbFullscreen = FALSE;

// Entry Point Function
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow) // Winmain(main function which gives window(Hungarian Notation Used)), lpsz(Long Pointer To Zero-Terminated String ('\0')), hPrevInstance - for backward compatibility
{
	// Function Declarations
	int initialize(void);
	void uninitialize(void);
	void display(void);
	void update(void);

	// Local Variable Declarations
	WNDCLASSEX wndclass;					 // (wndclass)window, class(type), type or class of window EX(WND(window) Extended)
	HWND hwnd;								 // handle window (Unsigned Int)
	MSG msg;								 // struct
	TCHAR szAppName[] = TEXT(" VMKWindow "); // szAppName - Zero Terminated String To App-Name ('\0'), TEXT (macro)
											 // TEXT(macro) - similar to 'printf'
	int iResult = 0;
	BOOL bdone = FALSE;

	// Local Variable Declarations For Centering Of Window
	int ScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	int ScreenHeight = GetSystemMetrics(SM_CYSCREEN);

	// code
	gpfile = fopen("log.txt", "w");
	if (gpfile == NULL)
	{
		MessageBox(NULL, TEXT("Log File Cannot Be Opened"), TEXT("Error"), MB_OK | MB_ICONERROR);
		exit(0);
	}
	fprintf(gpfile, "Program Started Successfully\n");
	fprintf(gpfile, "***********************************************\n\n");

	// WNDCEX initialization (Window Class Extended)
	wndclass.cbSize = sizeof(WNDCLASSEX);				 // cbSize - Count Of Byte-size
	wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC; // CS_HREDRAW - Class Style (Horizontal Re-Draw), CS_VREDRAW - (Vertical Re-Draw) // window should be drawn(modified) vertically and horizontally, CS | OWNDC = ClassStyle |Class Ownership(paint Specialist)
	wndclass.cbClsExtra = 0;							 // c
	wndclass.cbWndExtra = 0;
	wndclass.lpfnWndProc = WndProc;								   // lpfnWndProc - Long Pointer To Function - Window-Procedure
	wndclass.hInstance = hInstance;								   // Handle To Instance, (Camel Notation Used)
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);  // hbrBackground - (handleTobrush) Background, BLACK_BRUSH = macro For Getting Brush Having Black Color
	wndclass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON)); // hIcon - handle To Icon, Icon is an - 'RESOURCE', ('NULL' Parameter because we have no icon and have to get one), ID - Identifier I - (Application) - Icon ,
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);				   // NULL Parameter because we don't have a cursor, hCursor(Handle To Cursor)
	wndclass.lpszClassName = szAppName;							   // long-pointer to zero(NULL) terminated ('\0') Class Name
	wndclass.lpszMenuName = NULL;								   // Menu name - NULL (We Don't Have A Menu)
	wndclass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON));

	// Register WNDCLASSEX
	RegisterClassEx(&wndclass); // Registered Extended Class

	// Create Window
	hwnd = CreateWindowEx(WS_EX_APPWINDOW, szAppName,											// string to NULL terminated app name('\0'), //Creates window in memory, WS_EX_APPWINDOW -
						  TEXT("Vishwa Khude"),													// TEXT(macro) UNICODE
						  WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE, // (Window Style - OVERLAPPED(appearing above every application), WS_CLIPCHILDREN - area Clipped / Covered By Child Window is Excluded By Parent Window, WS_CLIPSIBLINGS - Exclude Clipping/Covering By SiblingWindow, WS_VISIBLE - PArent Window Should Be Visible
						  ScreenWidth / 2 - WIN_WIDTH / 2,
						  ScreenHeight / 2 - WIN_HEIGHT / 2,
						  WIN_WIDTH,
						  WIN_HEIGHT,
						  NULL,		 // Desktop Window(Defalut Handle), HWND_DESKTOP, (Parent Window Handle)
						  NULL,		 // NULL Parameter for 'Menu'
						  hInstance, // Instance for Creating Handle For Window(OS)
						  NULL);	 // used as an vessel (lpvoid(long parameter to void) - void*)

	ghwnd = hwnd;

	// initialization
	iResult = initialize();
	if (iResult != 0)
	{
		MessageBox(hwnd, TEXT("initialize() Failed"), TEXT("Error"), MB_OK | MB_ICONERROR);
		DestroyWindow(hwnd);
	}

	// Show The Window
	ShowWindow(hwnd, iCmdShow); // ShowWindow - Displaying Our Window, By Using (handle to window (hwnd)),

	SetForegroundWindow(hwnd); // Set Window On The Fore-ground(Front Order(Z-order))
	SetFocus(hwnd);			   // Keep My Window Highlited/Focused

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

	return ((int)msg.wParam);
}

// CallBack Function
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam) // UINT- Unsigned INTeger iMsg(integer Message)
{
	// Function Declarations
	void ToggleFullScreen(void);
	void resize(int, int);

	// code
	switch (iMsg) // Destroying The Window
	{
	case WM_SETFOCUS:
		gbActive = TRUE;
		break;

	case WM_KILLFOCUS:
		gbActive = FALSE;
		break;

	case WM_SIZE: // *** lParam(WM_SIZE) gives - Width & Height ***
		resize(LOWORD(lParam), HIWORD(lParam));
		break;

	case WM_ERASEBKGND: // WM_ERASEBKGND - wParam EraseBackGround(Erase The BackGround Of My Window)
		return (0);

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
		case 'F': // FallThrough
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
			if (bLightingEnabled == FALSE)
			{
				bLightingEnabled = TRUE;
			}
			else
			{
				bLightingEnabled = FALSE;
			}
		}
		break;

	case WM_CLOSE:
	{
		DestroyWindow(hwnd);
		break;
	}

	case WM_DESTROY: // GetMessage- False
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

	return (DefWindowProc(hwnd, iMsg, wParam, lParam)); // Default Window Procedure For returning the messages which are not be used in the program
}

void ToggleFullScreen(void)
{
	// Local Variable Declarations
	MONITORINFO mi = {sizeof(MONITORINFO)};

	// Code
	if (gbFullscreen == FALSE)
	{
		dwStyle = GetWindowLong(ghwnd, GWL_STYLE); // Win32 API(dwStyle)

		if (dwStyle & WS_OVERLAPPEDWINDOW)
		{
			if (GetWindowPlacement(ghwnd, &wpPrev) && GetMonitorInfo(MonitorFromWindow(ghwnd, MONITORINFOF_PRIMARY), &mi))
			{
				SetWindowLong(ghwnd, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW); // ~(To Remove Contents)
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

	float sphere_positions[1146];
	float sphere_normals[1146];
	float sphere_texcoords[764];
	unsigned short sphere_elements[2280];

	// function declarations
	void uninitialize(void);
	void printGLInfo(void);

	numSphereElements = getNumberOfSphereElements();

	// Code
	PIXELFORMATDESCRIPTOR pfd;
	int iPixelFormatIndex = 0;
	ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));

	// 1) Initialization of pixel format descriptor
	pfd.nSize, sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA; // RGBA - red, green, blue, alpha
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
		return (-1);
	}

	// 3) Choose Pixel Format
	iPixelFormatIndex = ChoosePixelFormat(ghdc, &pfd);
	if (iPixelFormatIndex == 0)
	{
		fprintf(gpfile, "ChoosePixelFormat() failed");
		return (-2);
	}

	// 4) Use Pixel Format Index(Set Obtained Pixel Format)
	if (SetPixelFormat(ghdc, iPixelFormatIndex, &pfd) == FALSE)
	{
		fprintf(gpfile, "SetPixelFormat() failed");
		return (-3);
	}

	// 5) Tell WindowsGraphicsLibrary(WGL) - [bridging API] to give me OpenGl Compatible dc(device context) from this dc(device context)
	ghrc = wglCreateContext(ghdc);
	if (ghrc == NULL)
	{
		fprintf(gpfile, "wglCreateContext() failed");
		return (-4);
	}

	// make rendering context current
	if (wglMakeCurrent(ghdc, ghrc) == FALSE)
	{
		fprintf(gpfile, "wglMakeCurrent() failed");
		return (-5);
	}

	// initialize GLEW
	if (glewInit() != GLEW_OK)
	{
		fprintf(gpfile, "glewInit failed to initialize GLEW");
		return (-6);
	}

	// print GLINFO
	printGLInfo();

	// Vertex Shader
	const GLchar *vertexShaderSourceCode =
		"#version 460 core"
		"\n"
		"in vec4 aPosition;"
		"in vec3 aNormal;"
		"uniform mat4 uModelMatrix;"
		"uniform mat4 uViewMatrix;"
		"uniform mat4 uProjectionMatrix;"
		"uniform vec3 uLightAmbient;"
		"uniform vec3 uLightDefuse;"
		"uniform vec3 uLightSpecular;"
		"uniform vec4 uLightPosition;"
		"uniform vec3 uMaterialAmbient;"
		"uniform vec3 uMaterialDefuse;"
		"uniform vec3 uMaterialSpecular;"
		"uniform float uMaterialShininess;"
		"uniform int uKeyPress;"
		"out vec3 oPhong_ADS_Light;"
		"void main(void)"
		"{"
		"if(uKeyPress == 1)"
		"{"
		"vec4 iCoordinates = uViewMatrix * uModelMatrix * aPosition;"
		"vec3 transformedNormals = normalize(mat3(uViewMatrix * uModelMatrix)*aNormal);"
		"vec3 lightDirection = normalize(vec3(uLightPosition - iCoordinates));"
		"vec3 reflectionVector = reflect(-lightDirection, transformedNormals);"
		"vec3 viewerVector = normalize(-iCoordinates.xyz);"
		"vec3 ambientLight = uLightAmbient * uMaterialAmbient;"
		"vec3 defuseLight = uLightDefuse * uMaterialDefuse *max(dot(lightDirection, transformedNormals), 0.0);"
		"vec3 specularLight = uLightSpecular * uMaterialSpecular * pow(max(dot(reflectionVector, viewerVector), 0.0), uMaterialShininess);"
		"oPhong_ADS_Light = ambientLight + defuseLight + specularLight;"
		"}"
		"else"
		"{"
		"oPhong_ADS_Light = vec3(0.0, 0.0, 0.0);"
		"}"
		"gl_Position = uProjectionMatrix * uViewMatrix * uModelMatrix * aPosition;"
		"}";

	GLuint vertexShaderObject = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertexShaderObject, 1, (const GLchar **)&vertexShaderSourceCode, NULL);

	glCompileShader(vertexShaderObject);

	//
	GLint status = 0;
	GLint infoLogLength = 0;
	GLchar *szInfoLog = NULL;

	glGetShaderiv(vertexShaderObject, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		glGetShaderiv(vertexShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);

		szInfoLog = (GLchar *)malloc(infoLogLength);
		if (infoLogLength > 0)
		{
			szInfoLog = (GLchar *)malloc(infoLogLength);

			if (szInfoLog != NULL)
			{
				glGetShaderInfoLog(vertexShaderObject, GL_INFO_LOG_LENGTH, NULL, szInfoLog);
				fprintf(gpfile, "vertex shader compilation error log - %s\n", szInfoLog);

				free(szInfoLog);
				szInfoLog = NULL;
			}
		}
		uninitialize();
	}
	// Fragment Shader
	const GLchar *fragmentShaderSourceCode =
		"#version 460 core"
		"\n"
		"in vec4 oColor;"
		"in vec3 oPhong_ADS_Light;"
		"uniform int uKeyPress;"
		"out vec4 fragColor;"
		"void main(void)"
		"{"
		"if (uKeyPress == 1)"
		"{"
		"fragColor = vec4(oPhong_ADS_Light, 1.0f);"
		"}"
		"else"
		"{"
		"fragColor = vec4(1.0, 1.0f, 1.0, 1.0);"
		"}"
		"}";

	GLuint fragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragmentShaderObject, 1, (const GLchar **)&fragmentShaderSourceCode, NULL);

	glCompileShader(fragmentShaderObject);

	status = 0;
	infoLogLength = 0;
	szInfoLog = NULL;

	glGetShaderiv(fragmentShaderObject, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		glGetShaderiv(fragmentShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);

		if (infoLogLength > 0)
		{
			szInfoLog = (GLchar *)malloc(infoLogLength);

			if (szInfoLog != NULL)
			{
				glGetShaderInfoLog(fragmentShaderObject, GL_INFO_LOG_LENGTH, NULL, szInfoLog);
				fprintf(gpfile, "fragment shader compilation error log : %s \n", szInfoLog);
				free(szInfoLog);
				szInfoLog = NULL;
			}
		}

		uninitialize();
	}

	// shader Program
	shaderProgramObject = glCreateProgram();

	glAttachShader(shaderProgramObject, vertexShaderObject);
	glAttachShader(shaderProgramObject, fragmentShaderObject);

	glBindAttribLocation(shaderProgramObject, AMC_ATTRIBUTE_POSITION, "aPosition");
	glBindAttribLocation(shaderProgramObject, AMC_ATTRIBUTE_NORMAL, "aNormal");

	glLinkProgram(shaderProgramObject);

	status = 0;
	infoLogLength = 0;
	szInfoLog = NULL;

	glGetProgramiv(shaderProgramObject, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		glGetProgramiv(shaderProgramObject, GL_INFO_LOG_LENGTH, &infoLogLength);
		if (infoLogLength > 0)
		{
			szInfoLog = (GLchar *)malloc(infoLogLength);
			if (szInfoLog != NULL)
			{
				glGetProgramInfoLog(shaderProgramObject, infoLogLength, NULL, szInfoLog);
				fprintf(gpfile, "shader program linking error log - %s \n", szInfoLog);
				free(szInfoLog);
				szInfoLog = NULL;
			}
		}
		uninitialize();
	}

	// Get shader Uniform locations
	modelMatrixUniform = glGetUniformLocation(shaderProgramObject, "uModelMatrix");
	viewMatrixUniform = glGetUniformLocation(shaderProgramObject, "uViewMatrix");
	projectionViewMatrixUniform = glGetUniformLocation(shaderProgramObject, "uProjectionMatrix");
	lAmbientUniform = glGetUniformLocation(shaderProgramObject, "uLightAmbient");
	lDiffuseUniform = glGetUniformLocation(shaderProgramObject, "uLightDefuse");
	lSpecularUniform = glGetUniformLocation(shaderProgramObject, "uLightSpecular");
	lightPositionUniform = glGetUniformLocation(shaderProgramObject, "uLightPosition");
	materialAmbientUniform = glGetUniformLocation(shaderProgramObject, "uMaterialAmbient");
	materialDiffuseUniform = glGetUniformLocation(shaderProgramObject, "uMaterialDefuse");
	materialSpecularUniform = glGetUniformLocation(shaderProgramObject, "uMaterialSpecular");
	materialShininessUniform = glGetUniformLocation(shaderProgramObject, "uMaterialShininess");
	keyPressUniform = glGetUniformLocation(shaderProgramObject, "uKeyPress");

	// Sphere
	getSphereVertexData(sphere_positions, sphere_normals, sphere_texcoords, sphere_elements);
	gNumVertices = getNumberOfSphereVertices();
	numSphereElements = getNumberOfSphereElements();

	// Sphere
	// VAO
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// VBO for position
	glGenBuffers(1, &vbo_position);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_position);
	glBufferData(GL_ARRAY_BUFFER, sizeof(sphere_positions), sphere_positions, GL_STATIC_DRAW);
	glVertexAttribPointer(AMC_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// VBO for Normal
	glGenBuffers(1, &vbo_normal);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_normal);
	glBufferData(GL_ARRAY_BUFFER, sizeof(sphere_normals), sphere_normals, GL_STATIC_DRAW);
	glVertexAttribPointer(AMC_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(AMC_ATTRIBUTE_NORMAL);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// VBO for textures
	/*
	glGenBuffers(1, &vbo_normal);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_normal);
	glBufferData(GL_ARRAY_BUFFER, sizeof(sphere_texcoords), sphere_texcoords, GL_STATIC_DRAW);
	// glVertexAttribPointer(AMC_ATTRIBUTE_NORMAL, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	// glEnableVertexAttribArray(AMC_ATTRIBUTE_NORMAL);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
*/
	// VBO for elements
	glGenBuffers(1, &vbo_elements);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_elements);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(sphere_elements), sphere_elements, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// unbind vao
	glBindVertexArray(0);

	// for Color
	// glVertexAttrib3f(AMC_ATTRIBUTE_COLOR, 1.0f, 1.0f, 1.0f);

	glBindVertexArray(0);

	// Enabling Depth...
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	// Set The Clearcolor of window to blue
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // alpha(transperancy, opaqueness)

	// perspective projection
	perspectiveProjectionMatrix = vmath::mat4::identity();

	resize(WIN_WIDTH, WIN_HEIGHT);

	return (0);
}

void printGLInfo(void)
{
	// variable declarations
	GLint numExtensions;
	GLint i;

	// code
	fprintf(gpfile, "OpenGL Vendor : %s \n", glGetString(GL_VENDOR));
	fprintf(gpfile, "OpenGL Renderer : %s \n", glGetString(GL_RENDERER));
	fprintf(gpfile, "OpenGL Version : %s \n", glGetString(GL_VERSION));
	fprintf(gpfile, "OpenGL GLSL Version : %s \n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	fprintf(gpfile, "***********************************************\n\n");

	// Listing of supported extensions
	glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions);

	for (i = 0; i < numExtensions; i++)
	{
		fprintf(gpfile, "%s \n", glGetStringi(GL_EXTENSIONS, i));
	}

	fprintf(gpfile, "***********************************************\n\n");
}

void resize(int width, int height)
{
	// Code
	if (height <= 0)
		height = 1;

	glViewport(0, 0, (GLsizei)width, (GLsizei)height); // 0x to 0y, total Width and Height of my window, binoculars

	// Set perspective projection matrix
	perspectiveProjectionMatrix = vmath::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
}

void display(void)
{

	// code
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(shaderProgramObject);

	// Cube
	vmath::mat4 translationMatrix = vmath::translate(0.0f, 0.0f, -2.0f);

	vmath::mat4 modelMatrix = translationMatrix;

	vmath::mat4 viewMatrix = vmath::mat4::identity();

	// Push All Uniform values into vertex shader
	glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(projectionViewMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	if (bLightingEnabled == TRUE)
	{
		glUniform1i(keyPressUniform, 1);
		glUniform3fv(lAmbientUniform, 1, lightAmbient);
		glUniform3fv(lDiffuseUniform, 1, lightDiffuse);
		glUniform3fv(lSpecularUniform, 1, lightSpecular);
		glUniform4fv(lightPositionUniform, 1, lightPosition);

		glUniform3fv(materialAmbientUniform, 1, materialAmbient);
		glUniform3fv(materialDiffuseUniform, 1, materialDiffuse);
		glUniform3fv(materialSpecularUniform, 1, materialSpecular);
		glUniform1f(materialShininessUniform, materialShininess);
	}
	else
	{
		glUniform1i(keyPressUniform, 0);
	}

	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_elements);
	glDrawElements(GL_TRIANGLES, numSphereElements, GL_UNSIGNED_SHORT, 0);
	glBindVertexArray(0);

	glUseProgram(0);

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
	if (shaderProgramObject)
	{
		glUseProgram(shaderProgramObject);

		GLint numShaders = 0;
		glGetProgramiv(shaderProgramObject, GL_ATTACHED_SHADERS, &numShaders);
		if (numShaders > 0)
		{
			GLuint *pShaders = (GLuint *)malloc(numShaders * sizeof(GLuint));
			if (pShaders != NULL)
			{
				glGetAttachedShaders(shaderProgramObject, numShaders, NULL, pShaders);
				for (GLint i = 0; i < numShaders; i++)
				{
					glDetachObjectARB(shaderProgramObject, pShaders[i]);
					glDeleteShader(pShaders[i]);
					pShaders[i] = 0;
				}
				free(pShaders);
				pShaders = NULL;
			}

			glUseProgram(0);
			glDeleteProgram(shaderProgramObject);
		}

		// Cube

		// delete vbo of positoion
		if (vbo_position)
		{
			glDeleteBuffers(1, &vbo_position);
			vbo_position = 0;
		}

		if (vbo_elements)
		{
			glDeleteVertexArrays(1, &vbo_elements);
			vbo_elements = 0;
		}

		if (vbo_normal)
		{
			glDeleteVertexArrays(1, &vbo_normal);
			vbo_normal = 0;
		}

		if (vbo_texcoord)
		{
			glDeleteVertexArrays(1, &vbo_texcoord);
			vbo_texcoord = 0;
		}
	}
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
