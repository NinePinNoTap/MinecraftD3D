#pragma once

#include "GameShader.h"

class TintShader : public GameShader
{
public:
	TintShader();
	~TintShader();

	bool initialise(HWND hwnd);

	bool prepare(Material* objMaterial, Transform* objTransform);
};

