#pragma once

#include "GameShader.h"
#include "ConstantBuffer.h"

class SkyShader : public GameShader
{
public:
	SkyShader();
	~SkyShader();

	bool initialise(HWND hwnd);

	bool prepare(Material* objMaterial, Transform* objTransform);
};