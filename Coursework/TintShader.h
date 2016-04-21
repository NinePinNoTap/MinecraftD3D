#pragma once

#include "GameShader.h"

class TintShader : public GameShader
{
public:
	TintShader();
	~TintShader();

	bool Initialise(HWND hwnd);

	bool Prepare(Material* objMaterial, Transform* objTransform);
};

