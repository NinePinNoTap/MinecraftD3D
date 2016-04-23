#pragma once

#include "Button.h"
#include "GameScene.h"
#include "InputManager.h"
#include "Sprite.h"
#include "Text.h"

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
	void CreateButton(string buttonText, int x, int y);
	void Render();

	Sprite* Background_;
	vector<Button*> Buttons_;
};

