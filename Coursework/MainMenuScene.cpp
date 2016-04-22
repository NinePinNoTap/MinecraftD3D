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

	Render();

	return true;
}

void MainMenuScene::Render()
{
	DirectXManager::Instance()->BeginScene();

	Background_->Render();

	DirectXManager::Instance()->EndScene();
}

void MainMenuScene::Reset()
{
	return;
}