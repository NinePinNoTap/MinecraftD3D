#pragma once

#include "GameShader.h"
#include "ConstantBuffer.h"

class TerrainShader : public GameShader
{
public:
	TerrainShader();
	~TerrainShader();

	bool Prepare(Mesh3D* objMesh, Material* objMaterial, Transform* objTransform);
};