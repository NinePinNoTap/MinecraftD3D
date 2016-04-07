#pragma once

#include "GameShader.h"
#include "ConstantBuffer.h"

class SkyShader : public GameShader
{
public:
	SkyShader();
	~SkyShader();

	bool Initialise(HWND hwnd);

	bool Prepare(Mesh3D* objMesh, Material* objMaterial, Transform* objTransform);
};