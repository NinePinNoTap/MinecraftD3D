#pragma once

#include "GameShader.h"
#include "ConstantBuffer.h"

class SkyShader : public GameShader
{
public:
	SkyShader();
	~SkyShader();

	bool Initialise(HWND hwnd);

	bool Prepare(Material* objMaterial, Transform* objTransform);
};