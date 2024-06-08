package com.vmk.window;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;

import androidx.appcompat.widget.AppCompatTextView;

import android.graphics.Color;

import android.view.Gravity;

public class MainActivity extends AppCompatActivity
{
	@Override
	protected void onCreate(Bundle savedInstanceState)
	{
		
		super.onCreate(savedInstanceState);
		
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
