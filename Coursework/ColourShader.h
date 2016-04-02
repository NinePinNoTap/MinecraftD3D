#pragma once

#include "GameShader.h"
#include "ConstantBuffer.h"

class ColourShader : public GameShader
{
public:
	ColourShader();
	~ColourShader();

	bool Prepare(Mesh3D* objMesh, Material* objMaterial, Transform* objTransform);
};