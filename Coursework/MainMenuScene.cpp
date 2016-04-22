#include "MainMenuScene.h"
#include "ApplicationManager.h"
#include "WindowManager.h"

MainMenuScene::MainMenuScene()
{
}


MainMenuScene::~MainMenuScene()
{
}

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

	CreateButton(350, 325);
	CreateButton(350, 475);
	

	return true;
}

void MainMenuScene::Shutdown()
{

}

bool MainMenuScene::Frame()
{
	if (InputManager::Instance()->GetKey(VK_1))
	{
		ApplicationManager::Instance()->SetScene(SceneState::GAME);
	}

	/*if (!Buttons_.empty())
	{
		for (unsigned int i = 0; i < Buttons_.size(); i++)
		{
			Buttons_[i]->Frame();
		}
	}*/

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

void MainMenuScene::Reset()
{
	ShowCursor(true);

	return;
}

void MainMenuScene::CreateButton(int x, int y)
{
	float drawX;
	float drawY;

	// Calculate where to draw the button
	drawX = (float)(((WindowManager::Instance()->GetWindowResolution().width / 2) * -1) + x);
	drawY = (float)((WindowManager::Instance()->GetWindowResolution().height / 2) - y);

	// Create the button
	Sprite* createdButton = new Sprite;
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

	// Set position
	createdButton->GetTransform()->SetPosition(drawX, drawY, 1);

	// Set Shader
	createdButton->SetShader("texture");

	// Enable alpha blending
	createdButton->SetRenderModes(RenderMode::Off, RenderMode::Off, RenderMode::Off, BlendMode::AlphaMasked);

	// Add to list
	Buttons_.push_back(createdButton);

	// Clean Up
	createdButton = 0;
}