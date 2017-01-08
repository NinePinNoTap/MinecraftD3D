#pragma once

#include "GameShader.h"
#include "ConstantBuffer.h"

class LightShader : public GameShader
{
public:
	LightShader();
	~LightShader();

	bool initialise(HWND hwnd);

	bool prepare(Material* objMaterial, Transform* objTransform);
};