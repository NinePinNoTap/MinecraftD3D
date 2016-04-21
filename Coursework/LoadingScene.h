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

private:
	void Render();

	Sprite* Background_;
	Sprite* Bar_;
	Sprite* BarMask_;
	int LoadingProgress_;
	Text* LoadingText_;
};


