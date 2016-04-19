#pragma once

enum LayerType
{
	Absolute, // Overrides current height
	Additive // Applys terrain ontop of existing height
};

struct TerrainLayer
{
	TerrainLayer()
	{

	}

	TerrainLayer(string name, string block, float h, float f, float a, float e)
	{
		// Info
		layerName = name;
		blockName = block;

		// Simplex Noise
		baseHeight = h;
		frequency = f;
		amplitude = a;
		exponent = e;

		// Generation
		layerType = LayerType::Additive;
	}

	TerrainLayer(string name, string block, float h, float f, float a, float e, LayerType type)
	{
		// Info
		layerName = name;
		blockName = block;

		// Simplex Noise
		baseHeight = h;
		frequency = f;
		amplitude = a;
		exponent = e;

		// Generation
		layerType = type;
	}

	string layerName;
	string blockName;
	float baseHeight;
	float frequency;
	float amplitude;
	float exponent;
	LayerType layerType;
};