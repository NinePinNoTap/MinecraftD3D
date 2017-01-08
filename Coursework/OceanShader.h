#pragma once

#include "GameShader.h"
#include "ConstantBuffer.h"

class OceanShader : public GameShader
{
public:
	OceanShader();
	~OceanShader();

	bool initialise(HWND hwnd);

	bool prepare(Material* objMaterial, Transform* objTransform);
};