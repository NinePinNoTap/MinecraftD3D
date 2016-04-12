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

	// Setters
	void Reset();
	void SetSceneLoaded();

private:
	void Render();

	void GenerateMatrices();
	void SetShaderManagerVars();

	Sprite* Background_;
	Sprite* Bar_;
	Sprite* BarMask_;
	int LoadingProgress_;
	Text* LoadingText_;
	bool SceneCanSwitch_;
};


