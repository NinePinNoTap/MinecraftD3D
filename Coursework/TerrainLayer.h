#pragma once

struct TerrainLayer
{
	TerrainLayer()
	{

	}

	TerrainLayer(float h, float f, float a, float e)
	{
		baseHeight = h;
		frequency = f;
		amplitude = a;
		exponent = e;
	}

	float baseHeight;
	float frequency;
	float amplitude;
	float exponent;
};