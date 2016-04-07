#pragma once

#include "GameShader.h"
#include "ConstantBuffer.h"

class ColourShader : public GameShader
{
public:
	ColourShader();
	~ColourShader();

	bool Initialise(HWND hwnd);

	bool Prepare(Mesh3D* objMesh, Material* objMaterial, Transform* objTransform);
};