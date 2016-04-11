#pragma once

#include "GameShader.h"
#include "ConstantBuffer.h"

class CloudShader : public GameShader
{
public:
	CloudShader();
	~CloudShader();

	bool Initialise(HWND hwnd);

	bool Prepare(Material* objMaterial, Transform* objTransform);
};