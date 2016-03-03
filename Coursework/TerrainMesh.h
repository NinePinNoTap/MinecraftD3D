#pragma once

#include "Mesh.h"
#include "PerlinNoiseGenerator.h"
#include "Rect.h"
#include "Vector2.h"
#include "Vector3.h"

class TerrainMesh : public Mesh
{
public:
	TerrainMesh();
	~TerrainMesh();

	bool CreateTerrain(Rect3D TerrainSize, Vector2 TextureRepeat, int Scale);
	void CreateTerrainWithPerlinNoise(Rect3D TerrainSize, Vector2 TextureRepeat, int Scale, float Smoothing, int Seed = -1);

private:
	bool BuildFlatTerrain(Vector2 TextureRepeat, int Scale);
	bool CalculateTerrainNormals();

	int Seed_;
};

