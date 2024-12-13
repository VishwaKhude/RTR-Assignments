package com.vmk.window;

import android.content.Context;

// Packages relatd to
import android.view.MotionEvent;
import android.view.GestureDetector;
import android.view.GestureDetector.OnDoubleTapListener;
import android.view.GestureDetector.OnGestureListener;

// Packages for TextView
import androidx.appcompat.widget.AppCompatTextView;
import android.graphics.Color;
import android.view.Gravity;

public class MyTextView extends AppCompatTextView implements OnDoubleTapListener, OnGestureListener
{
	private GestureDetector gestureDetector = null;

	public MyTextView(Context context)
	{
		super(context);

		setTextColor(Color.rgb(0, 255, 0));
		setTextSize(40);
		setGravity(Gravity.CENTER);
		setText("Hello World !!!");

		// Create and set gestureDetector object
		gestureDetector = new GestureDetector(context, this, null, false); 

		gestureDetector.setOnDoubleTapListener(this);

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
		setText("Double Tap");
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
		setText("Single Tap");
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
		setText("Long Press");
	}

	@Override
	public boolean onScroll(MotionEvent e1, MotionEvent e2, float distanceX, float distanceY)
	{
		setText("Scroll/Swipe");
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
}


