#pragma once

#include "GameShader.h"
#include "ConstantBuffer.h"

class ColourShader : public GameShader
{
public:
	ColourShader();
	~ColourShader();

	bool initialise(HWND hwnd);

	bool prepare(Material* objMaterial, Transform* objTransform);
};