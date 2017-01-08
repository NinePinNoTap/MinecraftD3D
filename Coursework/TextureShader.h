#pragma once

#include "GameShader.h"
#include "ConstantBuffer.h"

class TextureShader : public GameShader
{
public:
	TextureShader();
	~TextureShader();

	bool initialise(HWND hwnd);

	bool prepare(Material* objMaterial, Transform* objTransform);
};