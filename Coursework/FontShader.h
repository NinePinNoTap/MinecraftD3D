#pragma once

#include "GameShader.h"
#include "ConstantBuffer.h"

class FontShader : public GameShader
{
public:
	FontShader();
	~FontShader();

	bool Initialise(HWND hwnd);

	bool Prepare(Mesh3D* objMesh, Material* objMaterial, Transform* objTransform = 0);
};