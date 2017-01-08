#pragma once

#include "Model.h"
#include "perlinNoise.h"
#include "Rect.h"
#include "Vector2.h"
#include "Vector3.h"

class TerrainFactory
{
public:
	TerrainFactory();
	~TerrainFactory();

	bool createTerrain(Rect3D terrainSize, Vector2 textureRepeat, int terrainScale, Model& terrainModel);
	bool createTerrainWithPerlinNoise(const Rect3D terrainSize, Vector2 textureRepeat, int Scale, float Smoothing, Model& model, int Seed);

private:
	// Model Building
	bool buildModelFromData(Rect3D Terrain, VertexData* ModelData, Model& model);
	bool calculateNormals(Rect3D terrainSize, VertexData* ModelData); 
	bool m_result;
};