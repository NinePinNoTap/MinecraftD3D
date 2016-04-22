#include "LoadingScene.h"
#include "WindowManager.h"

LoadingScene::LoadingScene() : GameScene()
{
	Background_ = 0;
}
LoadingScene::~LoadingScene()
{

}

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

	//================
	// Initialise GUI
	//================

	LoadingText_ = new Text;
	LoadingText_->Initialise(GetActiveWindow(), "shruti.txt", "shruti.dds", 95);
	LoadingText_->CreateText("Load Time : ", Vector2(windowWidth - 25, windowHeight - 25), Colour::White, RIGHT);
	LoadingProgress_ = timeGetTime();

	return true;
}

void LoadingScene::Shutdown()
{
	if (Background_)
	{
		Background_->Shutdown();
		Background_ = 0;
	}
}

bool LoadingScene::Frame()
{
	LoadingText_->SetValue(0, (timeGetTime() - LoadingProgress_) / 1000);

	Render();

	return true;
}

void LoadingScene::Render()
{
	DirectXManager::Instance()->BeginScene();

	// Render the loading screen
	Background_->Render();

	// Render Text
	LoadingText_->Render();

	DirectXManager::Instance()->EndScene();
}

void LoadingScene::Reset()
{
	return;
}