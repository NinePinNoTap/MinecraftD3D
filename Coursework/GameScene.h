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
	// terminate
	virtual void terminate() = 0;

	// update
	virtual bool update() = 0;

	// Functionality
	virtual void onLoad() = 0;
	virtual void onUnload() = 0;

	inline bool isLoaded() { return m_isloaded; }

protected:
	// Flags
	bool m_result;
	bool m_isloaded;
};


