package com.vmk.window;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;

import androidx.appcompat.widget.AppCompatTextView;

import android.graphics.Color;

import android.view.Gravity;

// packages for fullscreen
import androidx.core.view.WindowCompat;

import androidx.core.view.WindowInsetsControllerCompat; 

import androidx.core.view.WindowInsetsCompat;

public class MainActivity extends AppCompatActivity
{
	@Override
	protected void onCreate(Bundle savedInstanceState)
	{
		
		super.onCreate(savedInstanceState);
		
		// FullScreen
		// Get and hide the action bar
		getSupportActionBar().hide();

		// Do FullScreen
		WindowCompat.setDecorFitsSystemWindows(getWindow(), false);
		
		// get Window Insets Controller
		WindowInsetsControllerCompat windowInsetsControllerCompat = WindowCompat.getInsetsController(getWindow(), getWindow().getDecorView());

		// Tell above object to hide system bars means status bar, navigation bar, caption bar and InputMethodEditor
		windowInsetsControllerCompat.hide(WindowInsetsCompat.Type.systemBars() | WindowInsetsCompat.Type.ime());

		// Setting background Color to Black
		getWindow().getDecorView().setBackgroundColor(Color.BLACK);
		AppCompatTextView myTextView = new AppCompatTextView(this);
		myTextView.setTextColor(Color.rgb(0, 255, 0));
		myTextView.setTextSize(40);
		myTextView.setGravity(Gravity.CENTER);
		myTextView.setText("Hello World !!!");

		setContentView(myTextView);
	}
}
