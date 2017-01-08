#pragma once

#include "GameShader.h"
#include "ConstantBuffer.h"

class InstancedTextureShader : public GameShader
{
public:
	InstancedTextureShader();
	~InstancedTextureShader();

	bool initialise(HWND hwnd);

	bool prepare(Material* objMaterial, Transform* objTransform);
};