// OpenGL starts here...
// Common Windows Header Files
#include <windows.h>		  // same as <stdio.h> library function, // includes around 3,50,000 API's, Win32 API
#include <stdio.h>            // For File I/O
#include <stdlib.h>           // For exit()				

// OpenGl Header Files
#include <GL/glew.h> // This must be before gl/GL.h
#include <GL/gl.h> // '\' also works

// header file for programmable pipeline
#include "vmath.h"
using namespace vmath;

#include "OGL.h"		      // Our HeaderFile   
#include "Sphere.h"

//Macros
#define WIN_WIDTH 800
#define WIN_HEIGHT 600

#define FBO_WIDTH 512
#define FBO_HEIGHT 512

// link with opengl library
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "Sphere.lib")

//OpenGl Related Global Variables
HDC ghdc = NULL;
HGLRC ghrc = NULL;

GLuint shaderProgramObject_cube = 0;
enum
{
	AMC_ATTRIBUTE_POSITION = 0,
	AMC_ATTRIBUTE_COLOR,
	AMC_ATTRIBUTE_TEXCOORD,
	AMC_ATTRIBUTE_NORMAL
};

// for Triangle
GLuint vao_pyramid = 0;
GLuint vbo_position_pyramid = 0;
GLuint vbo_texcoord_pyramid = 0;

// for square
GLuint vao_cube = 0;
GLuint vbo_position_cube = 0;
GLuint vbo_texcoord_cube = 0;

// FBO
GLint winWidth;
GLint winHeight;

GLuint mvpMatrixUniform_cube = 0;
GLuint textureSamplerUniform_cube = 0;

GLuint pervertex_shaderProgramObject_sphere = 0;
GLuint perfragment_shaderProgramObject_sphere = 0;

mat4 perspectiveProjectionMatrix_cube;		//mat4 is in vmath.h

//texture object
GLuint texture_kundali = 0;
GLuint textureFBO = 0;

GLfloat fAnglePyramid = 0.0f;
GLfloat fAngleCube = 0.0f;

// Sphere related
struct Light
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec4 position;
};

struct Light light[3];

GLuint vao_sphere = 0;
GLuint vbo_sphere_position = 0;
GLuint vbo_sphere_normal = 0;
GLuint vbo_sphere_element = 0;
GLuint vbo_sphere_texcoord = 0;
// GLuint vbo_color_sphere = 0;

// GLuint mvpMatrixUniform = 0;
GLuint modelViewMatrixUniform = 0;
GLuint projectionMatrixUniform = 0;
GLuint modelMatrixUniform, uMatrixUniform;
// material
GLuint lightAmbientUniform[3];
GLuint lightDiffuseUniform[3];
GLuint lightSpecularUniform[3];
GLuint lightPositionUniform[3];
GLuint materialAmbientUniform = 0;
GLuint materialDiffuseUniform = 0;
GLuint materialSpecularUniform = 0;
GLuint materialShininessUniform = 0;
GLuint keyPressUniform = 0;

BOOL bLightingEnabled = FALSE;

GLuint gNumElements;
GLuint gNumVertices;

GLfloat materialAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat materialDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat materialSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat materialShininess = 128.0f;

mat4 perspectiveProjetionMatrix_sphere;

char chosenShader = 'v';

float my_angle = 0.0f;

// FBO Related Global Variables
GLuint FBO = 0;
GLuint RBO = 0;   // render buffer objects
GLuint FBO_texture = 0;
BOOL bFBOResult = FALSE;

BOOL initializeSphere(GLint, GLint);

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
	int initialize_cube(void);
	void uninitialize_cube(void);
	void display_cube(void);
	void update_cube(void);

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
	fprintf(gpfile, "***********************************************\n\n");

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
	iResult = initialize_cube();
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
				display_cube();

				// Update
				update_cube();
			}
		}
	}

	// Uninitialization
	uninitialize_cube();

	return((int)msg.wParam);
}

// CallBack Function
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam) // UINT- Unsigned INTeger iMsg(integer Message) 
{
	//Function Declarations
	void ToggleFullScreen(void);
	void resize_cube(int, int);

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
		resize_cube(LOWORD(lParam), HIWORD(lParam));
		break;

	case WM_ERASEBKGND: // WM_ERASEBKGND - wParam EraseBackGround(Erase The BackGround Of My Window)
		return(0);  

	case WM_KEYDOWN:
		switch (LOWORD(wParam))
		{
		case VK_ESCAPE:


		}
		break;

	case WM_CHAR:
		switch (LOWORD(wParam))
		{

		case 'S':
		case 's':
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

		case 'V':
		case 'v':
			if (bLightingEnabled == TRUE)
			{
				chosenShader = 'v';

				modelMatrixUniform = glGetUniformLocation(pervertex_shaderProgramObject_sphere, "uModelMatrix");
				modelViewMatrixUniform = glGetUniformLocation(pervertex_shaderProgramObject_sphere, "uViewMatrix");
				projectionMatrixUniform = glGetUniformLocation(pervertex_shaderProgramObject_sphere, "uProjectionMatrix");

				lightAmbientUniform[0] = glGetUniformLocation(pervertex_shaderProgramObject_sphere, "uLightAmbient[0]");
				lightDiffuseUniform[0] = glGetUniformLocation(pervertex_shaderProgramObject_sphere, "uLightDiffuse[0]");
				lightSpecularUniform[0] = glGetUniformLocation(pervertex_shaderProgramObject_sphere, "uLightSpecular[0]");
				lightPositionUniform[0] = glGetUniformLocation(pervertex_shaderProgramObject_sphere, "uLightPosition[0]");

				lightAmbientUniform[1] = glGetUniformLocation(pervertex_shaderProgramObject_sphere, "uLightAmbient[1]");
				lightDiffuseUniform[1] = glGetUniformLocation(pervertex_shaderProgramObject_sphere, "uLightDiffuse[1]");
				lightSpecularUniform[1] = glGetUniformLocation(pervertex_shaderProgramObject_sphere, "uLightSpecular[1]");
				lightPositionUniform[1] = glGetUniformLocation(pervertex_shaderProgramObject_sphere, "uLightPosition[1]");

				lightAmbientUniform[2] = glGetUniformLocation(pervertex_shaderProgramObject_sphere, "uLightAmbient[2]");
				lightDiffuseUniform[2] = glGetUniformLocation(pervertex_shaderProgramObject_sphere, "uLightDiffuse[2]");
				lightSpecularUniform[2] = glGetUniformLocation(pervertex_shaderProgramObject_sphere, "uLightSpecular[2]");
				lightPositionUniform[2] = glGetUniformLocation(pervertex_shaderProgramObject_sphere, "uLightPosition[2]");

				materialAmbientUniform = glGetUniformLocation(pervertex_shaderProgramObject_sphere, "uMaterialAmbient");
				materialDiffuseUniform = glGetUniformLocation(pervertex_shaderProgramObject_sphere, "uMaterialDiffuse");
				materialSpecularUniform = glGetUniformLocation(pervertex_shaderProgramObject_sphere, "uMaterialSpecular");
				materialShininessUniform = glGetUniformLocation(pervertex_shaderProgramObject_sphere, "uMaterialShininess");

				keyPressUniform = glGetUniformLocation(pervertex_shaderProgramObject_sphere, "uKeyPress");
			}
			break;

		case 'F':
		case 'f':
			if (bLightingEnabled == TRUE)
			{
				chosenShader = 'f';

				modelMatrixUniform = glGetUniformLocation(perfragment_shaderProgramObject_sphere, "uModelMatrix");
				modelViewMatrixUniform = glGetUniformLocation(perfragment_shaderProgramObject_sphere, "uViewMatrix");
				projectionMatrixUniform = glGetUniformLocation(perfragment_shaderProgramObject_sphere, "uProjectionMatrix");

				lightAmbientUniform[0] = glGetUniformLocation(perfragment_shaderProgramObject_sphere, "uLightAmbient[0]");
				lightDiffuseUniform[0] = glGetUniformLocation(perfragment_shaderProgramObject_sphere, "uLightDiffuse[0]");
				lightSpecularUniform[0] = glGetUniformLocation(perfragment_shaderProgramObject_sphere, "uLightSpecular[0]");
				lightPositionUniform[0] = glGetUniformLocation(perfragment_shaderProgramObject_sphere, "uLightPosition[0]");

				lightAmbientUniform[1] = glGetUniformLocation(perfragment_shaderProgramObject_sphere, "uLightAmbient[1]");
				lightDiffuseUniform[1] = glGetUniformLocation(perfragment_shaderProgramObject_sphere, "uLightDiffuse[1]");
				lightSpecularUniform[1] = glGetUniformLocation(perfragment_shaderProgramObject_sphere, "uLightSpecular[1]");
				lightPositionUniform[1] = glGetUniformLocation(perfragment_shaderProgramObject_sphere, "uLightPosition[1]");

				lightAmbientUniform[2] = glGetUniformLocation(perfragment_shaderProgramObject_sphere, "uLightAmbient[2]");
				lightDiffuseUniform[2] = glGetUniformLocation(perfragment_shaderProgramObject_sphere, "uLightDiffuse[2]");
				lightSpecularUniform[2] = glGetUniformLocation(perfragment_shaderProgramObject_sphere, "uLightSpecular[2]");
				lightPositionUniform[2] = glGetUniformLocation(perfragment_shaderProgramObject_sphere, "uLightPosition[2]");

				materialAmbientUniform = glGetUniformLocation(perfragment_shaderProgramObject_sphere, "uMaterialAmbient");
				materialDiffuseUniform = glGetUniformLocation(perfragment_shaderProgramObject_sphere, "uMaterialDiffuse");
				materialSpecularUniform = glGetUniformLocation(perfragment_shaderProgramObject_sphere, "uMaterialSpecular");
				materialShininessUniform = glGetUniformLocation(perfragment_shaderProgramObject_sphere, "uMaterialShininess");

				keyPressUniform = glGetUniformLocation(perfragment_shaderProgramObject_sphere, "uKeyPress");
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
			break;

		case 'Q':
		case 'q':
			DestroyWindow(hwnd);
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

int initialize_cube(void)
{
	
	void resize_cube(int, int);

	BOOL bResult;

	// function declarations
	void uninitialize_cube(void);
	void printGLInfo(void);

	BOOL createFBO(GLint, GLint);
		
	// Vertex Shader 
	const GLchar* vertexShaderSourceCode =
		"#version 460 core" \
		"\n" \
		"in vec4 aPosition;" \
		"in vec2 aTexCoord;" \
		"uniform mat4 uMVPMatrix;" \
		"out vec2 oTexCoord;" \
		"void main(void)" \
		"{" \
		"gl_Position = aPosition;" \
		"oTexCoord = aTexCoord;" \
		"gl_Position = uMVPMatrix*aPosition;" \
		"}";

	GLuint vertexShaderObject = glCreateShader(GL_VERTEX_SHADER);
	
	glShaderSource(vertexShaderObject, 1, (const GLchar**)& vertexShaderSourceCode, NULL);
	
	glCompileShader(vertexShaderObject);

	// 
	GLint status = 0;
	GLint infoLogLength = 0;
	GLchar* szInfoLog = NULL;

	glGetShaderiv(vertexShaderObject, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		glGetShaderiv(vertexShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);

		szInfoLog = (GLchar*)malloc(infoLogLength);
		if (infoLogLength > 0)
		{
			szInfoLog = (GLchar*)malloc(infoLogLength);

			if (szInfoLog != NULL)
			{
				glGetShaderInfoLog(vertexShaderObject, GL_INFO_LOG_LENGTH, NULL, szInfoLog);
				fprintf(gpfile, "vertex shader compilation error log - %s\n", szInfoLog);

				free(szInfoLog);
				szInfoLog = NULL;
				
			}
		}
		uninitialize_cube();
	}
	
	//Fragment Shader 
	const GLchar* fragmentShaderSourceCode =
		"#version 460 core" \
		"\n" \
		"in vec2 oTexCoord;" \
		"uniform sampler2D uTextureSampler;" \
		"out vec4 FragColor;" \
		"void main(void)" \
		"{" \
		"FragColor = texture(uTextureSampler, oTexCoord);" \
		"}";
	
	GLuint fragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragmentShaderObject, 1, (const GLchar**)&fragmentShaderSourceCode, NULL);

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
			szInfoLog = (GLchar*)malloc(infoLogLength);

			if (szInfoLog != NULL)
			{
				glGetShaderInfoLog(fragmentShaderObject, GL_INFO_LOG_LENGTH, NULL, szInfoLog);
				fprintf(gpfile, "fragment shader compilation error log : %s \n", szInfoLog);
				free(szInfoLog);
				szInfoLog = NULL;
				
			}
		}

		uninitialize_cube();
	}

	// shader Program
	shaderProgramObject_cube = glCreateProgram();

	glAttachShader(shaderProgramObject_cube, vertexShaderObject);
	glAttachShader(shaderProgramObject_cube, fragmentShaderObject);

	glBindAttribLocation(shaderProgramObject_cube, AMC_ATTRIBUTE_POSITION, "aPosition");
	glBindAttribLocation(shaderProgramObject_cube, AMC_ATTRIBUTE_TEXCOORD, "aTexCoord");

	glLinkProgram(shaderProgramObject_cube);

	status = 0;
	infoLogLength = 0;
	szInfoLog = NULL;

	glGetProgramiv(shaderProgramObject_cube, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		glGetProgramiv(shaderProgramObject_cube, GL_INFO_LOG_LENGTH, &infoLogLength);
		if (infoLogLength > 0)
		{
			szInfoLog = (GLchar*)malloc(infoLogLength);
			if (szInfoLog != NULL)
			{
				glGetProgramInfoLog(shaderProgramObject_cube, infoLogLength, NULL, szInfoLog);
				fprintf(gpfile, "shader program linking error log - %s \n", szInfoLog);
				free(szInfoLog);
				szInfoLog = NULL;
			}
		}
		uninitialize_cube();
	}

	// Get shader Uniform locations
	mvpMatrixUniform_cube = glGetUniformLocation(shaderProgramObject_cube, "uMVPMatrix");
	textureSamplerUniform_cube = glGetUniformLocation(shaderProgramObject_cube, "uTextureSampler");

	const GLfloat cube_position[] =
	{
		// top
		1.0f, 1.0f, -1.0f,
	   -1.0f, 1.0f, -1.0f,
	   -1.0f, 1.0f, 1.0f,
	    1.0f, 1.0f, 1.0f,

	   // bottom
	   1.0f, -1.0f, -1.0f,
	   -1.0f, -1.0f, -1.0f,
	   -1.0f, -1.0f,  1.0f,
	   1.0f, -1.0f,  1.0f,

	   // front
	   1.0f, 1.0f, 1.0f,
	   -1.0f, 1.0f, 1.0f,
	   -1.0f, -1.0f, 1.0f,
	   1.0f, -1.0f, 1.0f,

	   // back
	   1.0f, 1.0f, -1.0f,
	   -1.0f, 1.0f, -1.0f,
	   -1.0f, -1.0f, -1.0f,
	   1.0f, -1.0f, -1.0f,

	   // right
	   1.0f, 1.0f, -1.0f,
	   1.0f, 1.0f, 1.0f,
	   1.0f, -1.0f, 1.0f,
	   1.0f, -1.0f, -1.0f,

	   // left
	   -1.0f, 1.0f, 1.0f,
	   -1.0f, 1.0f, -1.0f,
	   -1.0f, -1.0f, -1.0f,
	   -1.0f, -1.0f, 1.0f,
	};

	
	GLfloat cube_texcoords[] =
	{
	    0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,

		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,

		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,

		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,

		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,

		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
	};

	//CUBE
	//VAO
	glGenVertexArrays(1, &vao_cube);
	glBindVertexArray(vao_cube);

	//VBO for Texcoords
	glGenBuffers(1, &vbo_texcoord_cube);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_texcoord_cube);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_texcoords), cube_texcoords, GL_STATIC_DRAW);
	glVertexAttribPointer(AMC_ATTRIBUTE_TEXCOORD, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(AMC_ATTRIBUTE_TEXCOORD);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//VBO for position
	glGenBuffers(1, &vbo_position_cube);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_position_cube);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_position), cube_position, GL_STATIC_DRAW);
	glVertexAttribPointer(AMC_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
		
	//for Color
	glVertexAttrib3f(AMC_ATTRIBUTE_COLOR, 1.0f, 1.0f, 1.0f);

	glBindVertexArray(0);

	//Enabling Depth...
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	
	// Set The Clearcolor of window to blue
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // alpha(transperancy, opaqueness)

	//Tell OpenGl To Enable the Texture
	glEnable(GL_TEXTURE_2D);

	//perspective projection 
	perspectiveProjectionMatrix_cube = vmath::mat4::identity();

	resize_cube(WIN_WIDTH, WIN_HEIGHT);
		
	return(0);
	
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


BOOL initialize_sphere(GLint textureWidth, GLint textureHeight)
{
	// function declarations
	void printgGLInfo(void);
	void resize_sphere(int, int);
	void uninitialize_sphere(void);
		

	// code
	// variable declaration
	PIXELFORMATDESCRIPTOR PFD;
	int iPixelFormatIndex = 0;

	// initializing PFD struct with 0
	ZeroMemory(&PFD, sizeof(PIXELFORMATDESCRIPTOR));

	// initialization of PixelFormatDescriptor
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

	// get dc
	ghdc = GetDC(ghwnd);
	if (ghdc == NULL)
	{
		fprintf(gpFILE, "GetDC Failed\n");
		return (-1);
	}

	// choosing pixel format which closlely matches to our initialised PFD
	iPixelFormatIndex = ChoosePixelFormat(ghdc, &PFD);

	if (iPixelFormatIndex == 0)
	{
		fprintf(gpFILE, "Choose PixelFormat function() failed");
		return (-2);
	}

	// set obtain PixelFormat
	if (SetPixelFormat(ghdc, iPixelFormatIndex, &PFD) == FALSE)
	{
		fprintf(gpFILE, "SetPixelFormat\n");
		return (-3);
	}

	// create opengl context from device context
	ghrc = wglCreateContext(ghdc);
	if (ghrc == NULL)
	{
		fprintf(gpFILE, "wglCreateContext() failed");
		return (-4);
	}

	// make rendering context current
	if (wglMakeCurrent(ghdc, ghrc) == FALSE)
	{
		fprintf(gpFILE, "wglMakeCurrent() Failed\n");
		return (-5);
	}

	// initialise GLEW
	if (glewInit() != GLEW_OK)
	{
		fprintf(gpFILE, "glewInit() failed\n");
		return (-6);
	}

	/////////---------------------------------------------pervertex-----------------------------------------------//////////

		// Vertex Shader
	const GLchar* pervertex_vertexShaderSourceCode =
		"#version 460 core"
		"\n"
		"in vec4 aPosition;"
		"in vec3 aNormal;"
		"uniform mat4 uModelMatrix;"
		"uniform mat4 uViewMatrix;"
		"uniform mat4 uProjectionMatrix;"
		"uniform vec3 uLightAmbient[3];"
		"uniform vec3 uLightDiffuse[3];"
		"uniform vec3 uLightSpecular[3];"
		"uniform vec4 uLightPosition[3];"
		"uniform vec3 uMaterialDiffuse;"
		"uniform vec3 uMaterialAmbient;"
		"uniform vec3 uMaterialSpecular;"
		"uniform float uMaterialShininess;"
		"uniform int uKeyPress;"
		"out vec3 oPhong_ADS_Light;"
		"void main(void)"
		"{"
		"if(uKeyPress == 1)"
		"{"
		"vec3 lightDirection[3];"
		"vec3 diffuseLight[3];"
		"vec3 ambientLight[3];"
		"vec3 specularLight[3];"
		"vec3 reflectionVector[3];"
		"vec4 iCoordinates = uViewMatrix * uModelMatrix * aPosition;"
		"vec3 transformedNormals = normalize(mat3(uViewMatrix * uModelMatrix) * aNormal);"
		"for(int i=0; i<3 ; i++)"
		"{"
		"lightDirection[i] = normalize(vec3(uLightPosition[i] - iCoordinates));"
		"reflectionVector[i] = reflect(-lightDirection[i],transformedNormals);"
		"vec3 viewerVector = normalize(-iCoordinates.xyz);"
		"ambientLight[i] = uLightAmbient[i] * uMaterialAmbient;"
		"diffuseLight[i] = uLightDiffuse[i] * uMaterialDiffuse * max(dot(lightDirection[i],transformedNormals),0.0);"
		"specularLight[i] = uLightSpecular[i] * uMaterialSpecular * pow(max(dot(reflectionVector[i],viewerVector),0.0),uMaterialShininess);"
		"oPhong_ADS_Light = oPhong_ADS_Light + ambientLight[i] + diffuseLight[i] + specularLight[i];"
		"}"
		"}"
		"else"
		"{"
		"oPhong_ADS_Light = vec3(0.0f,0.0f,0.0f);"
		"}"
		"gl_Position = uProjectionMatrix * uViewMatrix * uModelMatrix * aPosition;"
		"}";

	GLuint pervertex_vertexShaderObject = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(pervertex_vertexShaderObject, 1, (const GLchar**)&pervertex_vertexShaderSourceCode, NULL);

	glCompileShader(pervertex_vertexShaderObject);

	GLint status = 0;
	GLint infoLogLength = 0;
	GLchar* szinfolog = NULL;

	glGetShaderiv(pervertex_vertexShaderObject, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		glGetShaderiv(pervertex_vertexShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);
		if (infoLogLength > 0)
		{
			szinfolog = (GLchar*)malloc(infoLogLength);
			if (szinfolog != NULL)
			{
				glGetShaderInfoLog(pervertex_vertexShaderObject, GL_INFO_LOG_LENGTH, NULL, szinfolog);
				fprintf(gpFILE, "VertexShader Compilation error log : %s\n", szinfolog);
				free(szinfolog);
				szinfolog = NULL;
			}
		}
		uninitialize_sphere();
	}

	// fragment shader
	const GLchar* pervertex_fragmentShaderSourceCode =
		"#version 460 core"
		"\n"
		"in vec3 oPhong_ADS_Light;"
		"uniform int uKeyPress;"
		"out vec4 FragColor;"
		"void main(void)"
		"{"
		" if(uKeyPress == 1)"
		"FragColor = vec4(oPhong_ADS_Light,1.0f);"
		"else"
		"{"
		"FragColor = vec4(1.0, 1.0, 1.0, 1.0);"
		"}"
		"}";

	GLuint pervertex_fragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(pervertex_fragmentShaderObject, 1, (const GLchar**)&pervertex_fragmentShaderSourceCode, NULL);

	glCompileShader(pervertex_fragmentShaderObject);

	status = 0;
	infoLogLength = 0;
	szinfolog = NULL;

	glGetShaderiv(pervertex_fragmentShaderObject, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		glGetShaderiv(pervertex_fragmentShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);
		if (infoLogLength > 0)
		{
			szinfolog = (GLchar*)malloc(infoLogLength);
			if (szinfolog != NULL)
			{
				glGetShaderInfoLog(pervertex_fragmentShaderObject, GL_INFO_LOG_LENGTH, NULL, szinfolog);
				fprintf(gpFILE, "FragmentShader Compilation error log : %s\n", szinfolog);
				free(szinfolog);
				szinfolog = NULL;
			}
		}
		uninitialize_sphere();
	}

	// shader program
	pervertex_shaderProgramObject_sphere = glCreateProgram();

	glAttachShader(pervertex_shaderProgramObject_sphere, pervertex_vertexShaderObject);
	glAttachShader(pervertex_shaderProgramObject_sphere, pervertex_fragmentShaderObject);

	// prelinking
	glBindAttribLocation(pervertex_shaderProgramObject_sphere, AMC_ATTRIBUTE_POSITION, "aPosition");
	glBindAttribLocation(pervertex_shaderProgramObject_sphere, AMC_ATTRIBUTE_NORMAL, "aNormal");

	glLinkProgram(pervertex_shaderProgramObject_sphere);

	status = 0;
	infoLogLength = 0;
	szinfolog = NULL;

	glGetProgramiv(pervertex_shaderProgramObject_sphere, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		glGetProgramiv(pervertex_shaderProgramObject_sphere, GL_INFO_LOG_LENGTH, &infoLogLength);
		if (infoLogLength > 0)
		{
			szinfolog = (GLchar*)malloc(infoLogLength);
			if (szinfolog != NULL)
			{
				glGetProgramInfoLog(pervertex_shaderProgramObject_sphere, infoLogLength, NULL, szinfolog);
				fprintf(gpFILE, "Shader Program linking error log: %s\n", szinfolog);
				free(szinfolog);
				szinfolog = NULL;
			}
		}
		uninitialize_sphere();
	}

	modelMatrixUniform = glGetUniformLocation(pervertex_shaderProgramObject_sphere, "uModelMatrix");
	modelViewMatrixUniform = glGetUniformLocation(pervertex_shaderProgramObject_sphere, "uViewMatrix");
	projectionMatrixUniform = glGetUniformLocation(pervertex_shaderProgramObject_sphere, "uProjectionMatrix");

	lightAmbientUniform[0] = glGetUniformLocation(pervertex_shaderProgramObject_sphere, "uLightAmbient[0]");
	lightDiffuseUniform[0] = glGetUniformLocation(pervertex_shaderProgramObject_sphere, "uLightDiffuse[0]");
	lightSpecularUniform[0] = glGetUniformLocation(pervertex_shaderProgramObject_sphere, "uLightSpecular[0]");
	lightPositionUniform[0] = glGetUniformLocation(pervertex_shaderProgramObject_sphere, "uLightPosition[0]");

	lightAmbientUniform[1] = glGetUniformLocation(pervertex_shaderProgramObject_sphere, "uLightAmbient[1]");
	lightDiffuseUniform[1] = glGetUniformLocation(pervertex_shaderProgramObject_sphere, "uLightDiffuse[1]");
	lightSpecularUniform[1] = glGetUniformLocation(pervertex_shaderProgramObject_sphere, "uLightSpecular[1]");
	lightPositionUniform[1] = glGetUniformLocation(pervertex_shaderProgramObject_sphere, "uLightPosition[1]");

	lightAmbientUniform[2] = glGetUniformLocation(pervertex_shaderProgramObject_sphere, "uLightAmbient[2]");
	lightDiffuseUniform[2] = glGetUniformLocation(pervertex_shaderProgramObject_sphere, "uLightDiffuse[2]");
	lightSpecularUniform[2] = glGetUniformLocation(pervertex_shaderProgramObject_sphere, "uLightSpecular[2]");
	lightPositionUniform[2] = glGetUniformLocation(pervertex_shaderProgramObject_sphere, "uLightPosition[2]");

	materialAmbientUniform = glGetUniformLocation(pervertex_shaderProgramObject_sphere, "uMaterialAmbient");
	materialDiffuseUniform = glGetUniformLocation(pervertex_shaderProgramObject_sphere, "uMaterialDiffuse");
	materialSpecularUniform = glGetUniformLocation(pervertex_shaderProgramObject_sphere, "uMaterialSpecular");
	materialShininessUniform = glGetUniformLocation(pervertex_shaderProgramObject_sphere, "uMaterialShininess");

	keyPressUniform = glGetUniformLocation(pervertex_shaderProgramObject_sphere, "uKeyPress");


	// Vertex Shader
	const GLchar* perfragment_vertexShaderSourceCode =
		"#version 460 core"
		"\n"
		"in vec4 aPosition;"
		"in vec3 aNormal;"
		"uniform mat4 uModelMatrix;"
		"uniform mat4 uViewMatrix;"
		"uniform mat4 uProjectionMatrix;"
		"uniform vec4 uLightPosition[3];"
		"uniform int uKeyPress;"
		"out vec3 oTransformedNormals;"
		"out vec3 oViewerVector;"
		"out vec3 oLightDirection[3];"
		"void main(void)"
		"{"
		"if(uKeyPress == 1)"
		"{"
		"vec4 iCoordinates = uViewMatrix * uModelMatrix * aPosition;"
		"oTransformedNormals = mat3(uViewMatrix * uModelMatrix) * aNormal;"
		"oViewerVector = -iCoordinates.xyz;"
		"for(int i=0; i<3 ; i++)"
		"{"
		"oLightDirection[i] = vec3(uLightPosition[i] - iCoordinates);"
		"}"
		"}"
		"else"
		"{"
		"oTransformedNormals = vec3(0.0f,0.0f,0.0f);"
		"oViewerVector = vec3(0.0f,0.0f,0.0f);"
		"oLightDirection[0] = vec3(0.0f,0.0f,0.0f);"
		"oLightDirection[1] = vec3(0.0f,0.0f,0.0f);"
		"oLightDirection[2]= vec3(0.0f, 0.0f, 0.0f);"
		"}"
		" gl_Position = uProjectionMatrix * uViewMatrix * uModelMatrix * aPosition;"
		"}";

	GLuint perfragment_vertexShaderObject = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(perfragment_vertexShaderObject, 1, (const GLchar**)&perfragment_vertexShaderSourceCode, NULL);

	glCompileShader(perfragment_vertexShaderObject);

	status = 0;
	infoLogLength = 0;
	szinfolog = NULL;

	glGetShaderiv(perfragment_vertexShaderObject, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		glGetShaderiv(perfragment_vertexShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);
		if (infoLogLength > 0)
		{
			szinfolog = (GLchar*)malloc(infoLogLength);
			if (szinfolog != NULL)
			{
				glGetShaderInfoLog(perfragment_vertexShaderObject, GL_INFO_LOG_LENGTH, NULL, szinfolog);
				fprintf(gpFILE, "VertexShader Compilation error log : %s\n", szinfolog);
				free(szinfolog);
				szinfolog = NULL;
			}
		}
		uninitialize_sphere();
	}

	// fragment shader
	const GLchar* perfragment_fragmentShaderSourceCode =
		"#version 460 core"
		"\n"
		"in vec3 oTransformedNormals;"
		"in vec3 oViewerVector;"
		"in vec3 oLightDirection[3];"
		"uniform vec3 uLightDiffuse[3];"
		"uniform vec3 uLightAmbient[3];"
		"uniform vec3 uLightSpecular[3];"
		"uniform vec3 uMaterialDiffuse;"
		"uniform vec3 uMaterialAmbient;"
		"uniform vec3 uMaterialSpecular;"
		"uniform float uMaterialShininess;"
		"uniform int uKeyPress;"
		"out vec4 FragColor;"
		"void main(void)"
		"{"
		"  vec3 phongADSLight = vec3(0.0f,0.0f,0.0f);"
		"  if(uKeyPress == 1)"
		"	{"
		"		vec3 viewerVector = normalize(oViewerVector);"
		"		vec3 transformedNormals = normalize(oTransformedNormals);"
		"		vec3 lightDirection[3];"
		"		vec3 diffuseLight[3];"
		"		vec3 ambientLight[3];"
		"		vec3 specularLight[3];"
		"		vec3 reflectionVector[3];"
		"		for(int i=0; i<3 ; i++)"
		"		{"
		"		   lightDirection[i] = normalize(oLightDirection[i]);"
		"		   reflectionVector[i] = reflect(-lightDirection[i],transformedNormals);"
		"		   ambientLight[i] = uLightAmbient[i] * uMaterialAmbient;"
		"		   diffuseLight[i] = uLightDiffuse[i] * uMaterialDiffuse * max(dot(lightDirection[i],transformedNormals),0.0);"
		"		   specularLight[i] = uLightSpecular[i] * uMaterialSpecular * pow(max(dot(reflectionVector[i],viewerVector),0.0),uMaterialShininess);"
		"		   phongADSLight = phongADSLight + ambientLight[i] + diffuseLight[i] + specularLight[i];"
		"		}"
		"	}"
		"  else"
		"	phongADSLight = vec3(1.0f,1.0f,1.0f);"
		"  FragColor = vec4(phongADSLight,1.0f);"
		"}";

	GLuint perfragment_fragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(perfragment_fragmentShaderObject, 1, (const GLchar**)&perfragment_fragmentShaderSourceCode, NULL);

	glCompileShader(perfragment_fragmentShaderObject);

	status = 0;
	infoLogLength = 0;
	szinfolog = NULL;

	glGetShaderiv(perfragment_fragmentShaderObject, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		glGetShaderiv(perfragment_fragmentShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);
		if (infoLogLength > 0)
		{
			szinfolog = (GLchar*)malloc(infoLogLength);
			if (szinfolog != NULL)
			{
				glGetShaderInfoLog(perfragment_fragmentShaderObject, GL_INFO_LOG_LENGTH, NULL, szinfolog);
				fprintf(gpFILE, "FragmentShader Compilation error log : %s\n", szinfolog);
				free(szinfolog);
				szinfolog = NULL;
			}
		}
		uninitialize_sphere();
	}

	// shader program
	perfragment_shaderProgramObject_sphere = glCreateProgram();

	glAttachShader(perfragment_shaderProgramObject_sphere, perfragment_vertexShaderObject);
	glAttachShader(perfragment_shaderProgramObject_sphere, perfragment_fragmentShaderObject);

	// prelinking
	glBindAttribLocation(perfragment_shaderProgramObject_sphere, AMC_ATTRIBUTE_POSITION, "aPosition");
	glBindAttribLocation(perfragment_shaderProgramObject_sphere, AMC_ATTRIBUTE_NORMAL, "aNormal");

	glLinkProgram(perfragment_shaderProgramObject_sphere);

	status = 0;
	infoLogLength = 0;
	szinfolog = NULL;

	glGetProgramiv(perfragment_shaderProgramObject_sphere, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		glGetProgramiv(perfragment_shaderProgramObject_sphere, GL_INFO_LOG_LENGTH, &infoLogLength);
		if (infoLogLength > 0)
		{
			szinfolog = (GLchar*)malloc(infoLogLength);
			if (szinfolog != NULL)
			{
				glGetProgramInfoLog(perfragment_shaderProgramObject_sphere, infoLogLength, NULL, szinfolog);
				fprintf(gpFILE, "Shader Program linking error log: %s\n", szinfolog);
				free(szinfolog);
				szinfolog = NULL;
			}
		}
		uninitialize_sphere();
	}

	float sphere_positons[1146];
	float sphere_normals[1146];
	float sphere_texcoords[764];
	unsigned short sphere_elements[2280];

	light[0].ambient = vec3(0.0f, 0.0f, 0.0f);
	light[1].ambient = vec3(0.0f, 0.0f, 0.0f);
	light[2].ambient = vec3(0.0f, 0.0f, 0.0f);

	light[0].diffuse = vec3(1.0f, 0.0f, 0.0f); // Red Light Source
	light[1].diffuse = vec3(0.0f, 1.0f, 0.0f); // Blue Light Source
	light[2].diffuse = vec3(0.0f, 0.0f, 1.0f); // Green Light Source

	light[0].specular = vec3(1.0f, 1.0f, 1.0f);
	light[1].specular = vec3(1.0f, 1.0f, 1.0f);
	light[2].specular = vec3(1.0f, 1.0f, 1.0f);

	light[0].position = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	light[1].position = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	light[2].position = vec4(0.0f, 0.0f, 0.0f, 1.0f);

	getSphereVertexData(sphere_positons, sphere_normals, sphere_texcoords, sphere_elements);
	gNumVertices = getNumberOfSphereVertices();
	gNumElements = getNumberOfSphereElements();

	// vao sphere
	glGenVertexArrays(1, &vao_sphere);
	glBindVertexArray(vao_sphere);

	// position vbo sphere
	glGenBuffers(1, &vbo_sphere_position);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_sphere_position);
	glBufferData(GL_ARRAY_BUFFER, sizeof(sphere_positons), sphere_positons, GL_STATIC_DRAW);
	glVertexAttribPointer(AMC_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// normal vbo sphere
	glGenBuffers(1, &vbo_sphere_normal);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_sphere_normal);
	glBufferData(GL_ARRAY_BUFFER, sizeof(sphere_normals), sphere_normals, GL_STATIC_DRAW);
	glVertexAttribPointer(AMC_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(AMC_ATTRIBUTE_NORMAL);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// texture vbo sphere note- will not use it in light
	glGenBuffers(1, &vbo_sphere_texcoord);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_sphere_texcoord);
	glBufferData(GL_ARRAY_BUFFER, sizeof(sphere_texcoords), sphere_texcoords, GL_STATIC_DRAW);
	glVertexAttribPointer(AMC_ATTRIBUTE_TEXCOORD, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	// glEnableVertexAttribArray(AMC_ATTRIBUTE_TEXCOORD);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// element vbo sphere
	glGenBuffers(1, &vbo_sphere_element);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(sphere_elements), sphere_elements, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// unbind vao sphere
	glBindVertexArray(0);

	// Enabling Depth
	glClearDepth(1.0f);		 // Compulsory
	glEnable(GL_DEPTH_TEST); // Compulsory
	glDepthFunc(GL_LEQUAL);	 // Compulsory

	// set the clear color of window to blue
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// initialise
	perspectiveProjetionMatrix_sphere = vmath::mat4::identity();

	resize_sphere(WIN_WIDTH, WIN_HEIGHT);
	// here opengl starts

	// FBO related code 
	if (createFBO(FBO_WIDTH, FBO_HEIGHT) == TRUE)
	{
		bFBOResult = initialize_sphere(FBO_WIDTH, FBO_HEIGHT);
	}
	else
	{
		fprintf(gpfile, "create FBO Failed");
		return(false);
	}

	return (0);
}

BOOL createFBO(GLint textureWidth, GLint textureHeight)
{
	// 1-check capacity of render buffer
	GLint maxRenderBufferSize;
	
	glGetIntegerv(enum GL_MAX_RENDERBUFFER_SIZE, &maxRenderBufferSize);

	if (maxRenderBufferSize < textureWidth || maxRenderBufferSize < textureHeight)
	{
		fprintf(gpfile, "Texture size overflow\n");
		return(false);
	}

	// create custom frame buffer
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(gl_framebuffer, FBO);

	// create texture for fbo in which we are going to render the sphere
	glGenTextures(1, &textureFBO);
	glBindTexture(GL_TEXTURE_2D, textureFBO);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, NULL);
	
	// attach above texture to frmebuffer at default color attachment 0
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureFBO, 0);

	// now create render buffer to depth 
	glGenRenderBuffers(1, &RBO);
	glBindRenderBuffer(GL_RENDERBUFFER, RBO);

	// set the storage of above render buffer of texture size for depth
	glRenderBufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, textureWidth, textureHeight);

	// attach aboce depth related render buffer to FBO at depth attachment
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, RBO);

	// check the framebuffer status whether successful or not
	GLenum result = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (result != GL_FRAMEBUFFER_COMPLETE)
	{
		fprintf(gpfile, "framebuffer status is not complete");
		return(false);
	}

	glBindFrameBuffer(GL_FRAMEBUFFER, 0);
	return(true);

}

void resize_cube(int width, int height)
{
	// Code
	if (height <= 0)
		height = 1;

	WIN_WIDTH = width;
	WIN_HEIGHT = height;
	
	glViewport(0, 0, (GLsizei)width, (GLsizei)height); // 0x to 0y, total Width and Height of my window, binoculars

	// Set perspective projection matrix
	perspectiveProjectionMatrix_cube = vmath::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);

}

void resize_sphere(int width, int height)
{
	// code
	if (height <= 0)
		height = 1;

	// resize_sphere
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);

	// set perspective projection matrix
	perspectiveProjetionMatrix_sphere = vmath::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
}

void display_cube(void)
{	
	void display_sphere(GLint, GLint);

	// Code
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Depth-Buffer Added For 3D(Depth)
		
	glUseProgram(shaderProgramObject_cube);

	//transformation matrix

	vmath::mat4 modelViewMatrix = vmath::mat4::identity();
	vmath::mat4 translationMatrix = vmath::translate(-1.5f, 0.0f, -6.0f);

	mat4 rotationMatrix = mat4::identity();
	//rotationMatrix = vmath::rotate(fAnglePyramid, 0.0f, 1.0f, 0.0f);
	modelViewMatrix = translationMatrix * rotationMatrix;

	vmath::mat4 modelViewProjectionMatrix = vmath::mat4::identity(); //order of multiplication is v.imp in matrix multiplication
	modelViewProjectionMatrix = perspectiveProjectionMatrix_cube * modelViewMatrix;
	
	// Cube
	//transformation matrix
	translationMatrix = vmath::translate(0.4f, 0.0f, -6.0f);

	mat4 scaleMatrix = mat4::identity();
	scaleMatrix = vmath::scale(0.75f, 0.75f, 0.75f);

	rotationMatrix = vmath::rotate(fAngleCube, 1.0f, 0.0f, 0.0f);
	mat4 rotationMatrix1 = vmath::rotate(fAngleCube, 1.0f, 0.0f, 0.0f);
	mat4 rotationMatrix2 = vmath::rotate(fAngleCube, 0.0f, 1.0f, 0.0f);
	mat4 rotationMatrix3 = vmath::rotate(fAngleCube, 0.0f, 0.0f, 1.0f);

	rotationMatrix = rotationMatrix1 * rotationMatrix2 * rotationMatrix3;
	modelViewMatrix = translationMatrix * scaleMatrix * rotationMatrix;

	modelViewProjectionMatrix = vmath::mat4::identity();
	modelViewProjectionMatrix = perspectiveProjectionMatrix_cube * modelViewMatrix; //order of multiplication is v.imp in matrix multiplication
	
	// push above mvp into veretex shader's mvp uniform
	glUniformMatrix4fv(mvpMatrixUniform_cube, 1, GL_FALSE, modelViewProjectionMatrix);

	//For Texture
	glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, texture_kundali);
	glUniform1i(textureSamplerUniform_cube, 0);

	glBindVertexArray(vao_cube);

	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 4, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 8, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 12, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 16, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 20, 4);

	glBindVertexArray(0);

	glUseProgram(0);

	SwapBuffers(ghdc);
}

void update_cube(void)
{
	// Code
	fAngleCube = fAngleCube - 0.1f;
	if (fAngleCube <= 0.2f)
	{
		fAngleCube = fAngleCube + 360.0f;
	}

	void update_sphere(void);
	
}

void uninitialize_cube(void)
{
	// Function Declarations
	void ToggleFullScreen(void);

	// Code
	if (shaderProgramObject_cube)
	{
		glUseProgram(shaderProgramObject_cube);

		GLint numShaders = 0;
		glGetProgramiv(shaderProgramObject_cube, GL_ATTACHED_SHADERS, &numShaders);
		if (numShaders > 0)
		{
			GLuint* pShaders = (GLuint*)malloc(numShaders * sizeof(GLuint));
			if (pShaders != NULL)
			{
				glGetAttachedShaders(shaderProgramObject_cube, numShaders, NULL, pShaders);
				for (GLint i = 0; i < numShaders; i++)
				{
					glDetachObjectARB(shaderProgramObject_cube, pShaders[i]);
					glDeleteShader(pShaders[i]);
					pShaders[i] = 0;
				}
				free(pShaders);
				pShaders = NULL;
			}
		
			glUseProgram(0);
			glDeleteProgram(shaderProgramObject_cube);
		}

		// Cube
			
		//delete vbo of positoion
		if (vbo_position_cube)
		{
			glDeleteBuffers(1, &vbo_position_cube);
			vbo_position_cube = 0;
		}

		//delete vao
		if (vao_cube)
		{
			glDeleteVertexArrays(1, &vao_cube);
			vao_cube = 0;
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

	if (texture_kundali)
	{
		glDeleteTextures(1, &texture_kundali);
		texture_kundali = 0;
	}

	if (vbo_texcoord_cube)
	{
		glDeleteBuffers(1, &vbo_texcoord_cube);
		vbo_texcoord_cube = 0;
	}

	// Close The LogFile
	if (gpfile)
	{
		fprintf(gpfile, "Program Ended Successfully");
		fclose(gpfile);
		gpfile = NULL;
	}
}

