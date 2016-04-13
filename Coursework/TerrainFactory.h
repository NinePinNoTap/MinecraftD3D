#pragma once

#include "Model.h"
#include "PerlinNoise.h"
#include "Rect.h"
#include "Vector2.h"
#include "Vector3.h"

class TerrainFactory
{
public:
	TerrainFactory();
	~TerrainFactory();

	bool CreateTerrain(Rect3D terrainSize, Vector2 textureRepeat, int terrainScale, Model& terrainModel);
	bool CreateTerrainWithPerlinNoise(const Rect3D terrainSize, Vector2 textureRepeat, int Scale, float Smoothing, Model& model, int Seed);

private:
	// Model Building
	bool BuildModelFromModelData(Rect3D Terrain, VertexData* ModelData, Model& model);
	bool CalculateTerrainNormals(Rect3D terrainSize, VertexData* ModelData); 
	bool Result_;
};