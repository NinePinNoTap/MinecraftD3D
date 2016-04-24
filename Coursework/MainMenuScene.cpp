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
bool MainMenuScene::Initialise(HWND hwnd)
{
	//===================
	// Create Background
	//===================

	Background_ = new Sprite;
	if (!Background_)
	{
		return false;
	}

	Result_ = Background_->Initialise(Rect3D(WindowManager::Instance()->GetWindowResolution()), "bg_mainmenu.dds");
	if (!Result_)
	{
		return false;
	}
	Background_->SetShader("texture");

	//===============
	// Create Buttons
	//===============

	CreateButton("PLAY GAME", 350, 325, SceneState::MINECRAFT);
	CreateButton("EXIT GAME", 350, 475, SceneState::EXIT);

	//=================
	// Initialise Vars
	//=================

	ShowCursor(true);
	IsLoaded_ = true;

	return true;
}

// Shutdown
void MainMenuScene::Shutdown()
{

}

// Frame
bool MainMenuScene::Frame()
{
	// Update Input
	InputManager::Instance()->Frame();

	// Update Buttons
	if (!Buttons_.empty())
	{
		for (unsigned int i = 0; i < Buttons_.size(); i++)
		{
			Result_ = Buttons_[i]->Frame();
			if (!Result_)
			{
				Buttons_[i]->Execute();
			}
		}
	}

	// Render the Menu
	Render();

	return true;
}

void MainMenuScene::Render()
{
	DirectXManager::Instance()->BeginScene();

	// Render Background
	Background_->Render();

	// Render Buttons
	if (!Buttons_.empty())
	{
		for (unsigned int i = 0; i < Buttons_.size(); i++)
		{
			Buttons_[i]->Render();
		}
	}

	DirectXManager::Instance()->EndScene();
}

// Load and Unload
void MainMenuScene::Load()
{
	SetCursor(LoadCursor(NULL, IDC_ARROW));

	return;
}

void MainMenuScene::Unload()
{
	return;
}

// Functionality
void MainMenuScene::CreateButton(string buttonText, int x, int y, int sceneID)
{
	// Create the button
	Button* createdButton = new Button;
	if (!createdButton)
	{
		return;
	}

	// Initialise the button
	Result_ = createdButton->Initialise(Rect3D(410, 85, 0), "bg_button.dds");
	if (!Result_)
	{
		return;
	}

	// Set Data
	createdButton->SetButton(buttonText, x, y);
	createdButton->SetHighlightColour(D3DXVECTOR4(.2f, .2f, 1, 0.2f));

	// Set Function
	createdButton->SetFunction(std::bind(&ApplicationManager::SetScene, ApplicationManager::Instance(), (SceneState)sceneID));

	// Set Shader
	createdButton->SetShader("tint");

	// Enable alpha blending
	createdButton->SetRenderModes(RenderMode::Off, RenderMode::Off, RenderMode::Off, BlendMode::AlphaMasked);

	// Add to list
	Buttons_.push_back(createdButton);

	// Clean Up
	createdButton = 0;
}