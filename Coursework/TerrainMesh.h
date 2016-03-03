#pragma once

#include "Mesh.h"
#include "PerlinNoiseGenerator.h"

class TerrainMesh : public Mesh
{
public:
	TerrainMesh();
	~TerrainMesh();

	bool CreateTerrain(ObjSize TerrainSize, Vector2 TextureRepeat, int Scale);
	void CreateTerrainWithPerlinNoise(ObjSize TerrainSize, Vector2 TextureRepeat, int Scale, float Smoothing, int Seed = -1);

private:
	bool BuildFlatTerrain(Vector2 TextureRepeat, int Scale);
	bool CalculateTerrainNormals();

	int Seed_;
};

