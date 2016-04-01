#pragma once

#include "GameShader.h"

class Shader : public GameShader
{
public:
	Shader() : GameShader()
	{
	}
	~Shader()
	{
	}

	void SendDataToShader();

};