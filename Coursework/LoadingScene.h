#pragma once

#include "Sprite.h"
#include "DirectXManager.h"
#include "GameScene.h"
#include "ShaderManager.h"

class LoadingScene : public GameScene
{
public:
	LoadingScene();
	~LoadingScene();

	// Initialising
	bool Initialise();

	// Shutdown
	void Shutdown();

	// Frame
	bool Frame();

	// Load and Unloading
	void Load();
	void Unload();

	// Transitions
	void LoadScene(SceneState sceneState);

private:
	void Render();

	Sprite* Background_;

	SceneState LoadTarget_;
};


