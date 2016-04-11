#pragma once

#include "GameShader.h"
#include "ConstantBuffer.h"

class InstancedLightShader : public GameShader
{
public:
	InstancedLightShader();
	~InstancedLightShader();

	bool Initialise(HWND hwnd);

	bool Prepare(Material* objMaterial, Transform* objTransform);
};