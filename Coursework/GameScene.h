#pragma once

#include <vector>

#include "DirectXManager.h"
#include "Camera.h"
#include "Light.h"
#include "Utilities.h"
#include "Texture.h"
#include "ShaderManager.h"
#include "AudioClip.h"
#include "Text.h"

class GameScene
{
public:
	GameScene()
	{

	}
	~GameScene()
	{

	}

	// Shutdown
	virtual void Shutdown() = 0;

	// Frame
	virtual bool Frame() = 0;
	virtual void Reset() = 0;
	
protected:
	// Flags
	bool Result_;
};


