#pragma once

#include "GameShader.h"
#include "ConstantBuffer.h"

class InstancedTextureShader : public GameShader
{
public:
	InstancedTextureShader();
	~InstancedTextureShader();

	bool Initialise(HWND hwnd);

	bool Prepare(Material* objMaterial, Transform* objTransform);
};