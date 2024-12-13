package Perspective;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;

import androidx.appcompat.widget.AppCompatTextView;

import android.graphics.Color;

// packages for fullscreen
import androidx.core.view.WindowCompat;

import androidx.core.view.WindowInsetsControllerCompat; 

import androidx.core.view.WindowInsetsCompat;

import android.content.pm.ActivityInfo;

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

		// Do LandScape
		setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);

		// Setting background Color to Black
		getWindow().getDecorView().setBackgroundColor(Color.BLACK);
		GLESView glesView = new GLESView(this);					
		
		setContentView(glesView);
	}

	@Override
		protected void onPause()
		{
			super.onPause();
		}

	@Override
		protected void onResume()
		{
			super.onResume();
		}
}

