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

	// Initialise
	bool Initialise(HWND hwnd);

	// Shutdown
	void Shutdown();

	// Frame
	bool Frame();
	
	// Scene Change
	void Load();
	void Unload();

private:
	void CreateButton(string buttonText, int x, int y, int sceneID);
	void Render();

	Sprite* Background_;
	vector<Button*> Buttons_;
};

