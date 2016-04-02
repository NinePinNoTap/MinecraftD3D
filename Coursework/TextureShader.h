#pragma once

#include "GameShader.h"
#include "ConstantBuffer.h"

class TextureShader : public GameShader
{
public:
	TextureShader();
	~TextureShader();

	bool Prepare(Mesh3D* objMesh, Material* objMaterial, Transform* objTransform);
};