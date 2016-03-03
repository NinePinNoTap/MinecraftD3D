#pragma once

// RGBA Colour Data Struct
struct Colour
{
	Colour()
	{
		r = g = b = a = 1.0f;
	}

	Colour(float _r, float _g, float _b)
	{
		r = _r;
		g = _g;
		b = _b;
		a = 1.0f;
	}

	Colour(float _r, float _g, float _b, float _a)
	{
		r = _r;
		g = _g;
		b = _b;
		a = _a;
	}

	float r;
	float g;
	float b;
	float a;
};