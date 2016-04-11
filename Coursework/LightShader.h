#pragma once

#include "GameShader.h"
#include "ConstantBuffer.h"

class LightShader : public GameShader
{
public:
	LightShader();
	~LightShader();

	bool Initialise(HWND hwnd);

	bool Prepare(Material* objMaterial, Transform* objTransform);
};