#pragma once

#include "GameShader.h"
#include "ConstantBuffer.h"

class OceanShader : public GameShader
{
public:
	OceanShader();
	~OceanShader();

	bool Initialise(HWND hwnd);

	bool Prepare(Material* objMaterial, Transform* objTransform);
};