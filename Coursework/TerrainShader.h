#pragma once

#include "GameShader.h"
#include "ConstantBuffer.h"

class TerrainShader : public GameShader
{
public:
	TerrainShader();
	~TerrainShader();

	bool initialise(HWND hwnd);

	bool prepare(Material* objMaterial, Transform* objTransform);
};