#pragma once

#include "GameShader.h"
#include "ConstantBuffer.h"

class ColourShader : public GameShader
{
public:
	ColourShader();
	~ColourShader();

	bool Initialise(HWND hwnd);

	bool Prepare(Material* objMaterial, Transform* objTransform);
};