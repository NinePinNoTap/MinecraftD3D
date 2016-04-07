#pragma once

#include "GameShader.h"
#include "ConstantBuffer.h"

class FireShader : public GameShader
{
public:
	FireShader();
	~FireShader();

	bool Initialise(HWND hwnd);

	bool Prepare(Mesh3D* objMesh, Material* objMaterial, Transform* objTransform);
};