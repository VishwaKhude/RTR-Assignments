package gles_blue;

import android.content.Context;

//OpenGLES Related Packages
import android.opengl.GLES20;
import android.opengl.GLSurfaceView;
import android.opengl.GLES32; //3.2 version
import javax.microedition.khronos.opengles.GL10; // 1.0 version
import javax.microedition.khronos.egl.EGLConfig;


//Event Related Packages
import android.view.MotionEvent;
import android.view.GestureDetector;
import android.view.GestureDetector.OnDoubleTapListener;
import android.view.GestureDetector.OnGestureListener;


public class GLESView extends GLSurfaceView implements GLSurfaceView.Renderer, OnDoubleTapListener,OnGestureListener
{
    private GestureDetector gestureDetector;

    public GLESView(Context context)
    {
        super(context);

        //OpenGLES Related
        //Create set Current OpenGLES Context for version 3
        setEGLContextClientVersion(3);
        setRenderer(this);
        setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);

        //Event Related
        //Create and Set GestureDetector Object
        gestureDetector = new GestureDetector(context,this,null,false);
        gestureDetector.setOnDoubleTapListener(this);
    }

    //Implementation of 3 GLSurfaceView.Renderer Interface Methods
    @Override
    public void onSurfaceCreated(GL10 gl,EGLConfig config)
    {
        //code
        initialise(gl);
    }

    @Override
    public void onSurfaceChanged(GL10 gl,int width,int height)
    {
        //code
        resize(width,height);
    }

    @Override
    public void onDrawFrame(GL10 gl)
    {
        //code
       display();
    }

    //Implementation of onTouchEvent of view class (AppCompatTextView is extended from view class)
    @Override
    public boolean onTouchEvent(MotionEvent e)
    {

        //code
        if(!gestureDetector.onTouchEvent(e)) 
        {
            super.onTouchEvent(e);
        }
        return true;
    }


    //Implementation of 3 methods of OnDoubleTapListener Interface
    @Override
    public boolean onDoubleTap(MotionEvent e)
    {

        return true;
    }

    @Override
    public boolean onDoubleTapEvent(MotionEvent e)
    {
        return true;
    }

    @Override
    public boolean onSingleTapConfirmed(MotionEvent e)
    {

        return true;
    }

    //Implementation of 6 methods of OnGestureListener Interface
    @Override
    public boolean onDown(MotionEvent e)
    {
        return true;
    }

    @Override
    public boolean onFling(MotionEvent e1,MotionEvent e2,float velocityX,float velocityY)
    {

        return true;
    }
    @Override
    public void onLongPress(MotionEvent e)
    {

    }

    //Swipe
    @Override
    public boolean onScroll(MotionEvent e1,MotionEvent e2,float distanceX,float distanceY)
    {
        //code
        uninitialise();
        System.exit(0);

        return true;
    }

    @Override
    public void onShowPress(MotionEvent e)
    {

    }

    @Override
    public boolean onSingleTapUp(MotionEvent e)
    {

        return true;
    }


    //Implementation of private Methods
    private void initialise(GL10 gl)
    {
        //code
        //print OpenGLES Information
        printGLInfo(gl);

        //Depth Enable Setting
        GLES32.glClearDepthf(1.0f);
        GLES32.glEnable(GLES32.GL_DEPTH_TEST);
        GLES32.glDepthFunc(GLES32.GL_LEQUAL);

        //Culling
        GLES32.glEnable(GLES32.GL_CULL_FACE);
        GLES32.glCullFace(GLES32.GL_BACK);

        //Clear Color
        GLES32.glClearColor(0.0f,0.0f,1.0f,1.0f);


    }

    private void resize(int width,int height)
    {
        //code
        if(height<=0)
            height = 1;

        GLES32.glViewport(0,0,width,height);

    }

    private void display()
    {
        //code
        GLES32.glClear(GLES32.GL_COLOR_BUFFER_BIT | GLES32.GL_DEPTH_BUFFER_BIT);

        //Render
        requestRender();
    }

    private void uninitialise()
    {
        //code
    }

    private void printGLInfo(GL10 gl)
    {
        //code
        System.out.println("AMC:OpenGLES Renderer: " + gl.glGetString(GL10.GL_RENDERER));
        System.out.println("AMC:OpenGLES Version :" + gl.glGetString(GL10.GL_VERSION));
        System.out.println("AMC:OpenGLES Shading Language Version :" + gl.glGetString(GLES32.GL_SHADING_LANGUAGE_VERSION));
    }
    


}

