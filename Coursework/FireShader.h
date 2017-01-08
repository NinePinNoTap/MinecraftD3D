#pragma once

#include "GameShader.h"
#include "ConstantBuffer.h"

class FireShader : public GameShader
{
public:
	FireShader();
	~FireShader();

	bool initialise(HWND hwnd);

	bool prepare(Material* objMaterial, Transform* objTransform);
};