#pragma once

// Data struct for representing the size of a 2D object
struct Rect2D
{
	Rect2D()
	{
		width = 0;
		height = 0;
	}

	Rect2D(int w, int h)
	{
		width = w;
		height = h;
	}

	int width;
	int height;
};

// Data struct for representing the size of a 3D object
struct Rect3D
{
	// Default
	Rect3D()
	{
		width = 0;
		height = 0;
		depth = 0;
	}

	Rect3D(float w, float h)
	{
		width = w;
		height = h;
		depth = 0.0f;
	}

	Rect3D(float w, float h, float d)
	{
		width = w;
		height = h;
		depth = d;
	}

	float width;
	float height;
	float depth;
};
