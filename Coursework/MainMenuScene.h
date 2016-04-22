#pragma once

#include "GameScene.h"
#include "InputManager.h"
#include "Sprite.h"

class MainMenuScene : public GameScene
{
public:
	MainMenuScene();
	~MainMenuScene();

	bool Initialise(HWND hwnd);
	void Shutdown();

	bool Frame();
	void Reset();

private:
	void Render();

	Sprite* Background_;
};

