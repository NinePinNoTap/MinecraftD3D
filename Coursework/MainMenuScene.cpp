#include "MainMenuScene.h"
#include "ApplicationManager.h"
#include "WindowManager.h"

MainMenuScene::MainMenuScene()
{
}


MainMenuScene::~MainMenuScene()
{
}

// Initialising
bool MainMenuScene::initialise(HWND hwnd)
{
	//===================
	// create Background
	//===================

	m_background = new Sprite;
	if (!m_background)
	{
		return false;
	}

	m_result = m_background->initialise(Rect3D(WindowManager::getInstance()->getWindowResolution()), "bg_mainmenu.dds");
	if (!m_result)
	{
		return false;
	}
	m_background->setShader("texture");

	//===============
	// create Buttons
	//===============

	createButton("PLAY GAME", 350, 325, SceneState::MINECRAFT);
	createButton("EXIT GAME", 350, 475, SceneState::EXIT);

	//=================
	// initialise Vars
	//=================

	ShowCursor(true);
	m_isloaded = true;

	return true;
}

// terminate
void MainMenuScene::terminate()
{

}

// update
bool MainMenuScene::update()
{
	// update Input
	InputManager::getInstance()->update();

	// update Buttons
	if (!m_buttons.empty())
	{
		for (unsigned int i = 0; i < m_buttons.size(); i++)
		{
			m_result = m_buttons[i]->update();
			if (!m_result)
			{
				m_buttons[i]->execute();
			}
		}
	}

	// render the Menu
	render();

	return true;
}

void MainMenuScene::render()
{
	DirectXManager::getInstance()->beginScene();

	// render Background
	m_background->render();

	// render Buttons
	if (!m_buttons.empty())
	{
		for (unsigned int i = 0; i < m_buttons.size(); i++)
		{
			m_buttons[i]->render();
		}
	}

	DirectXManager::getInstance()->endScene();
}

// onload and onUnload
void MainMenuScene::onLoad()
{
	SetCursor(LoadCursor(NULL, IDC_ARROW));

	return;
}

void MainMenuScene::onUnload()
{
	return;
}

// Functionality
void MainMenuScene::createButton(string buttonText, int x, int y, int sceneID)
{
	// create the button
	Button* createdButton = new Button;
	if (!createdButton)
	{
		return;
	}

	// initialise the button
	m_result = createdButton->initialise(Rect3D(410, 85, 0), "bg_button.dds");
	if (!m_result)
	{
		return;
	}

	// set Data
	createdButton->setButton(buttonText, x, y);
	createdButton->setHighlightColour(D3DXVECTOR4(.2f, .2f, 1, 0.2f));

	// set Function
	createdButton->setFunction(std::bind(&ApplicationManager::setScene, ApplicationManager::getInstance(), (SceneState)sceneID));

	// set Shader
	createdButton->setShader("tint");

	// Enable alpha blending
	createdButton->setRenderModes(renderMode::Off, renderMode::Off, renderMode::Off, BlendMode::AlphaMasked);

	// Add to list
	m_buttons.push_back(createdButton);

	// Clean Up
	createdButton = 0;
}