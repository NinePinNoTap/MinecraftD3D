#pragma once

#include "GameShader.h"
#include "ConstantBuffer.h"

class TerrainReflectionShader : public GameShader
{
public:
	TerrainReflectionShader();
	~TerrainReflectionShader();

	bool Prepare(Mesh3D* objMesh, Material* objMaterial, Transform* objTransform);
};