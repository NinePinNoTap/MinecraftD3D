#pragma once

#include "GameShader.h"
#include "ConstantBuffer.h"

class TerrainShader : public GameShader
{
public:
	TerrainShader();
	~TerrainShader();

	bool Initialise(HWND hwnd);

	bool Prepare(Material* objMaterial, Transform* objTransform);
};