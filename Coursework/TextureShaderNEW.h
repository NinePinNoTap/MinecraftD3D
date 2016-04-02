#pragma once

#include "GameShader.h"
#include "ConstantBuffer.h"

class TextureShaderNEW : public GameShader
{
public:
	TextureShaderNEW();
	~TextureShaderNEW();

	bool Prepare(Mesh3D* objMesh, Material* objMaterial, Transform* objTransform);
};