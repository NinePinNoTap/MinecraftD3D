#pragma once

#include "GameShader.h"
#include "ConstantBuffer.h"

class LightShader : public GameShader
{
public:
	LightShader();
	~LightShader();

	bool Prepare(Mesh3D* objMesh, Material* objMaterial, Transform* objTransform);
};