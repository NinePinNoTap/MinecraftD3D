#pragma once

#include "GameShader.h"
#include "ConstantBuffer.h"

class CloudShader : public GameShader
{
public:
	CloudShader();
	~CloudShader();

	bool initialise(HWND hwnd);

	bool prepare(Material* objMaterial, Transform* objTransform);
};