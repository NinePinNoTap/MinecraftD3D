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

	inline Rect2D operator * (const float multiplier)
	{
		return Rect2D(width * multiplier, height * multiplier);
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

	Rect3D(Rect2D rect)
	{
		width = rect.width;
		height = rect.height;
		depth = 0.0f;
	}

	Rect3D(float w, float h, float d)
	{
		width = w;
		height = h;
		depth = d;
	}
	Rect3D(Rect2D rect, float d)
	{
		width = rect.width;
		height = rect.height;
		depth = d;
	}

	inline Rect3D operator * (const float multiplier)
	{
		return Rect3D(width * multiplier, height * multiplier, depth * multiplier);
	}

	float width;
	float height;
	float depth;
};
