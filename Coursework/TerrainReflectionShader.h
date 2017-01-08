#pragma once

#include "GameShader.h"
#include "ConstantBuffer.h"

class TerrainReflectionShader : public GameShader
{
public:
	TerrainReflectionShader();
	~TerrainReflectionShader();

	bool initialise(HWND hwnd);

	bool prepare(Material* objMaterial, Transform* objTransform);
};