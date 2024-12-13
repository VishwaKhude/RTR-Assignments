package Perspective;

import android.content.Context;
import java.nio.ShortBuffer;

//OpenGLES related packages
import android.opengl.GLSurfaceView;
import android.opengl.GLES32; // 32 IS 3.2 VERSION OF android
import javax.microedition.khronos.opengles.GL10;
import javax.microedition.khronos.egl.EGLConfig;

// Packages related to
import android.view.MotionEvent;
import android.view.GestureDetector;
import android.view.GestureDetector.OnDoubleTapListener;
import android.view.GestureDetector.OnGestureListener;

// Javaio related packages
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.ByteBuffer;
import java.nio.FloatBuffer;

public class GLESView extends GLSurfaceView implements GLSurfaceView.Renderer, OnDoubleTapListener, OnGestureListener
{
	private GestureDetector gestureDetector = null;

	private int shaderProgramObject;
	private int[] vao = new int[1];
	private int[] vbo = new int[1];
	private int[] mvpMatrixUniform;
	private float[] perspectiveProjectionMatrix = new float[16];

	public GLESView(Context context)
	{
		super(context);

		//OpenGLES related
		setEGLContextClientVersion(3);

		setRenderer(this);

		setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);

		// Create and set gestureDetector object
		gestureDetector = new GestureDetector(context, this, null, false);  

		gestureDetector.setOnDoubleTapListener(this);

	}

	// IMPLEMENTATION OF 3 METHODS OF GLSurfaceView Interface
	@Override
		public void onSurfaceCreated(GL10 gl, EGLConfig config)
		{
			// code
			initialize(gl);
		}

	@Override
		public void onSurfaceChanged(GL10 gl, int width, int height)
		{
			// code
			resize(width, height);
		}

	@Override
		public void onDrawFrame(GL10 gl)
		{
			// code
			display();
		}

	// Implementation of OnTouch Event of viewClass
	@Override 
		public boolean onTouchEvent(MotionEvent e)
		{
			if(!gestureDetector.onTouchEvent(e))
			{
				super.onTouchEvent(e);
			}
			return(true);
			
		}

	// Implementation of 3 methods of OnDoubleTapListener interface
	@Override
	public boolean onDoubleTap(MotionEvent e)
	{
		
		return(true);
	}

	@Override
	public boolean onDoubleTapEvent(MotionEvent e)
	{
		return(true);
	}

	@Override
	public boolean onSingleTapConfirmed(MotionEvent e)
	{
		
		return(true);
	}

	// Implementation of 6 methods of OnGestureListener interface
	@Override
	public boolean onDown(MotionEvent e)
	{
		return(true);
	}

	@Override
	public boolean onFling(MotionEvent e1, MotionEvent e2, float velocityX, float velocityY)
	{		
		return(true);
	}

	@Override
	public void onLongPress(MotionEvent e)
	{
		
	}

	@Override
	public boolean onScroll(MotionEvent e1, MotionEvent e2, float distanceX, float distanceY)
	{
		uninitialize();
		System.exit(0);
		return(true);
	}

	@Override
	public void onShowPress(MotionEvent e)
	{

	}

	@Override
	public boolean onSingleTapUp(MotionEvent e)
	{
		return(true);
	}

	// implementation of private methods
	private void initialize(GL10 gl)
	{
		// code
		// print OpenGLES information
		printGLInfo(gl);

		//Depth Enabling setting
		GLES32.glClearDepthf(1.0f);
		GLES32.glEnable(GLES32.GL_DEPTH_TEST);
		GLES32.glDepthFunc(GLES32.GL_LEQUAL);

		// enable backface culling
		GLES32.glEnable(GLES32.GL_CULL_FACE);
		GLES32.glCullFace(GLES32.GL_BACK);


		//ClearColor
		GLES32.glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
	}

	private void printGLInfo(GL10 gl)
	{
		// CODE
		System.out.println("VMK- OpenGL-ES renderer - " + gl.glGetString(GL10.GL_RENDERER));
		System.out.println("VMK- OpenGL-ES version - " + gl.glGetString(GL10.GL_VERSION));
		System.out.println("VMK- OpenGL-ES Shading Language Version - " + gl.glGetString(GLES32.GL_SHADING_LANGUAGE_VERSION));
	}

	// vertex shader
	final String vertexShaderSourceCode = String.format 
		(
		"#version 320 es" +
		"\n" +
		"in vec4 aPosition;" +
		"in vec4 aColor;" +
		"out vec4 oColor;" +
		"void main(void)" +
		"{" +
		"gl_Position=aPosition;" +
		"oColor=aColor;" +
		"}" 
		);

		int vertexShaderObject = GLES32.glCreateShader(GLES32.GL_VERTEX_SHADER);
		GLES32.glShaderSource(vertexShaderObject, vertexShaderSourceCode);
		GLES32.glCompileShader(vertexShaderObject);
		int [] shaderCompileStatus = new int[1];
		int [] infoLogLength = new int[1];
		String infoLog = null;

		GLES32.glGetShaderiv(vertexShaderObject, GLES32.GL_COMPILE_STATUS, shaderCompileStatus, 0);
		if (shaderCompileStatus[0] == GLES32.GL_FALSE)
		{
			GLES32.glGetShaderiv(vertexShaderObject, GLES32.GL_INFO_LOG_LENGTH, infoLogLength, 0[0]);

			if (infoLogLength[0]) >= GLES32.glGetShaderInfoLog(vertexShaderObject);
			{	System.out.println("VMK- VertexShader compilation log - " + infoLog);
				uninitialize();
				System.exit(0);
			}
		}

		// fragment shader
		final String fragmentShaderSourceCode = String.format 
		(
		"#version 320 es" +
		"\n" +
		"in vec4 oColor;" +
		"out vec4 FragColor;" +
		"void main(void)" +
		"{" +
		"FragColor=oColor;" +
		"}"
		);

		int fragmentShaderObject = GLES32.glCreateShader(GLES32.GL_FRAGMENT_SHADER);
		GLES32.glShaderSource(fragmentShaderObject, fragmentShaderSourceCode);
		GLES32.glCompileShader(fragmentShaderObject);
		shaderCompileStatus[0] = 0;
		infoLogLength[0] = 0;
		infoLog = null;

		GLES32.glGetShaderiv(fragmentShaderObject, GLES32.GL_COMPILE_STATUS, shaderCompileStatus, 0);
		if (shaderCompileStatus[0] == GLES32.GL_FALSE)
		{
			GLES32.glGetShaderiv(fragmentShaderObject, GLES32.GL_INFO_LOG_LENGTH, infoLogLength, 0[0]);

			if (infoLogLength[0]) >= GLES32.glGetShaderInfoLog(fragmentShaderObject);
			{	System.out.println("VMK- FragmentShader compilation log - " + infoLog);
				uninitialize();
				System.exit(0);
			}
		}

		// shader program
		shaderProgramObject = glES32.glCreateProgram();

		GLES32.glAttachShader(shaderProgramObject, vertexShaderObject);
		GLES32.glAttachShader(shaderProgramObject, fragmentShaderObject);

		GLES32.glBindAttribLocation(shaderProgramObject, vertexAttributesEnum.AMC_ATTRIBUTE_POSITION, "aPosition");
		
		GLES32.glLinkProgram(shaderProgramObject);

		int[]programLinkStatus = new int[1];
		infoLogLength[0] = 0;
		infoLog = null;

		GLES32.glGetProgamiv(shaderProgamObject, GLES32.GL_LINK_STATUS, programLinkStatus, 0);
		if (programLinkStatus[0] != GLES32.GL_FALSE)
		{
			GLES32.glGetProgramiv(fragmentShaderObject, GLES32.GL_INFO_LOG_LENGTH, infoLogLength, 0);

			if (infoLogLength > 0) >= GLES32.glGetProgramInfoLog(shaderPrgramObject);
			{	System.out.println("VMK- shaderProgram compilation log - " + infoLog);
				uninitialize();
				System.exit(0);
			}
		}
	
	mvpMatrixUniform = GLES32.glGetUniformLocation(shaderProgamObject, "uMVPMatrix");

	// geometry 
	final float[] trianglePositions = float[]
	{
		0.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f
	};

	// VAO
	GLES32.glGenVertexArrays(1, vao, 0);
	GLES32.glBindVertexArray(vao[0]);
	GLES32.glGenBuffers(1, vbo, 0);
	GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, vbo[0]);

	// Prepare triangle vertices array for glBufferData
	ByteBuffer byteBuffer.allocateDirect(trianglePositions.length * 4);

	private void resize(int width, int height)
	{
		// code
		if (height <= 0)
		{
			height = 1;
		
		}

		GLES32.glViewport(0,0,width,height);
	}

	private void display()
	{
		// code
		GLES32.glClear(GLES32.GL_COLOR_BUFFER_BIT | GLES32.GL_DEPTH_BUFFER_BIT);

		// Render
		requestRender();
	}

	private void uninitialize()
	{
		// code

	}
}


