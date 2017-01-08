#pragma once

#include "GameShader.h"
#include "ConstantBuffer.h"

class FontShader : public GameShader
{
public:
	FontShader();
	~FontShader();

	bool initialise(HWND hwnd);

	bool prepare(Material* objMaterial, Transform* objTransform = 0);
};