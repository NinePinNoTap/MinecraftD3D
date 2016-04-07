#pragma once

#include "GameShader.h"
#include "ConstantBuffer.h"

class InstancedLightShader : public GameShader
{
public:
	InstancedLightShader();
	~InstancedLightShader();

	bool Initialise(HWND hwnd);

	bool Prepare(Mesh3D* objMesh, Material* objMaterial, Transform* objTransform);
};