#pragma once

#include "GameShader.h"
#include "ConstantBuffer.h"

class LightShader : public GameShader
{
public:
	LightShader();
	~LightShader();

	bool Initialise(HWND hwnd);

	bool Prepare(Mesh3D* objMesh, Material* objMaterial, Transform* objTransform);
};