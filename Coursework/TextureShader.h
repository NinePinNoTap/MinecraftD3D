#pragma once

#include "GameShader.h"
#include "ConstantBuffer.h"

class TextureShader : public GameShader
{
public:
	TextureShader();
	~TextureShader();

	bool Initialise(HWND hwnd);

	bool Prepare(Material* objMaterial, Transform* objTransform);
};