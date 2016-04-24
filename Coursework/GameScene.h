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

enum SceneState
{
	NO_SCENE,
	MAINMENU,
	LOADING,
	MINECRAFT,
	EXIT
};

class GameScene
{
public:
	// Shutdown
	virtual void Shutdown() = 0;

	// Frame
	virtual bool Frame() = 0;

	// Functionality
	virtual void Load() = 0;
	virtual void Unload() = 0;

	inline bool IsLoaded() { return IsLoaded_; }

protected:
	// Flags
	bool Result_;
	bool IsLoaded_;
};


