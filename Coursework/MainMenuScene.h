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

	// initialise
	bool initialise(HWND hwnd);

	// terminate
	void terminate();

	// update
	bool update();
	
	// Scene Change
	void onLoad();
	void onUnload();

private:
	void createButton(string buttonText, int x, int y, int sceneID);
	void render();

	Sprite* m_background;
	vector<Button*> m_buttons;
};

