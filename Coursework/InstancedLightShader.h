#pragma once

#include "GameShader.h"
#include "ConstantBuffer.h"

class InstancedLightShader : public GameShader
{
public:
	InstancedLightShader();
	~InstancedLightShader();

	bool initialise(HWND hwnd);

	bool prepare(Material* objMaterial, Transform* objTransform);
};