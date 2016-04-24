#include "LoadingScene.h"
#include "WindowManager.h"

LoadingScene::LoadingScene() : GameScene()
{
	Background_ = 0;
}

LoadingScene::~LoadingScene()
{

}

// Initialising
bool LoadingScene::Initialise()
{
	int windowWidth;
	int windowHeight;

	// Get window width and height
	windowWidth = WindowManager::Instance()->GetWindowResolution().width;
	windowHeight = WindowManager::Instance()->GetWindowResolution().height;
	
	//===============================
	// Initialise the Loading Screen
	//===============================

	// Loading Screen BG
	Background_ = new Sprite;
	if (!Background_)
	{
		return false;
	}
	Result_ = Background_->Initialise(Rect3D(windowWidth, windowHeight, 1.0f), "bg_loading.dds");
	if (!Result_)
	{
		return false;
	}
	Background_->SetShader("texture");

	//============
	// Initialise
	//============

	LoadTarget_ = SceneState::NO_SCENE;
	IsLoaded_ = true;

	return true;
}

// Shutdown
void LoadingScene::Shutdown()
{
	if (Background_)
	{
		Background_->Shutdown();
		Background_ = 0;
	}
}

// Frame
bool LoadingScene::Frame()
{
	// Check for Scene Processing
	if (LoadTarget_ != SceneState::NO_SCENE)
	{
		// Check if we can switch
		if (ApplicationManager::Instance()->CheckSceneLoaded(LoadTarget_))
		{
			ApplicationManager::Instance()->SetScene(LoadTarget_);
		}
	}

	// Render
	Render();

	return true;
}

void LoadingScene::Render()
{
	DirectXManager::Instance()->BeginScene();

	// Render the loading screen
	Background_->Render();

	DirectXManager::Instance()->EndScene();
}

// Load and Unloading
void LoadingScene::Load()
{
	return;
}

void LoadingScene::Unload()
{
	// Clear Target
	LoadTarget_ = SceneState::NO_SCENE;
}

void LoadingScene::LoadScene(SceneState sceneState)
{
	// Store the loading target
	LoadTarget_ = sceneState;
}