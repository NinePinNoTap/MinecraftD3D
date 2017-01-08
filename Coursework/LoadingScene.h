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
	bool initialise();

	// terminate
	void terminate();

	// update
	bool update();

	// onload and Unloading
	void onLoad();
	void onUnload();

	// Transitions
	void loadScene(SceneState sceneState);

private:
	void render();

	Sprite* m_background;

	SceneState m_loadTarget;
};


