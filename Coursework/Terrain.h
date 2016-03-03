#pragma once

#include <d3d11.h>
#include <d3dx10math.h>
#include <stdio.h>

#include "GameObject.h"
#include "Texture.h"

#include "TerrainMesh.h"

class Terrain : public GameObject
{
public:
	Terrain();
	Terrain(const Terrain&);
	~Terrain();

	// Initialise
	bool Initialise(Rect3D Size, WCHAR* textureFilename, WCHAR* normalTextureFilename, Vector2 TextureRepeat, float scale = 1.0f);
	void Shutdown();

	TerrainMesh* GetMesh();

private:
	// Terrain Information
	int TerrainWidth_;
	int TerrainHeight_;

	TerrainMesh* Mesh_;
};


