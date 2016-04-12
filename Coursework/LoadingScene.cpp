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

	//=======================
	// Initialise the Camera
	//=======================

	Camera::Instance()->Get2DViewMatrix(BaseViewMatrix_);
	
	//===============================
	// Initialise the Loading Screen
	//===============================

	// Loading Screen BG
	Background_ = new Sprite;
	if (!Background_)
	{
		return false;
	}
	Result_ = Background_->Initialise(Rect3D(windowWidth, windowHeight));
	if (!Result_)
	{
		return false;
	}
	Result_ = Background_->SetTexture("LoadingScreen.dds");
	if (!Result_)
	{
		return false;
	}
	Background_->GetTransform()->MoveZ(1.0f);
	Background_->SetShader("texture");

	//================
	// Initialise GUI
	//================

	LoadingText_ = new Text;
	LoadingText_->Initialise(GetActiveWindow(), "shruti", "shruti.dds", 95);
	LoadingText_->CreateText("Load Time : ", Vector2(windowWidth - 25, windowHeight - 25), WHITE, RIGHT);
	LoadingProgress_ = timeGetTime();

	// Initialise Flags
	SceneCanSwitch_ = false;

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
	if (SceneCanSwitch_)
	{
		if (InputManager::Instance()->GetKeyDown(VK_SPACE))
		{
			ApplicationManager::Instance()->SetScene(MAIN);
		}
	}
	else
	{
		LoadingText_->SetValue(0, (timeGetTime() - LoadingProgress_) / 1000);
	}

	Render();

	return true;
}

void LoadingScene::Render()
{
	DirectXManager::Instance()->BeginScene();

	// Generate Matrices and Send to Shader
	GenerateMatrices();
	SetShaderManager2DVars();

	// Render the loading screen
	Background_->Render();

	SetShaderManager2DVars();

	// Render Text
	LoadingText_->Render();

	DirectXManager::Instance()->EndScene();
}

void LoadingScene::Reset()
{
	return;
}

void LoadingScene::SetSceneLoaded()
{
	SceneCanSwitch_ = true;
	LoadingText_->SetText(0, "Press SPACE to continue");
}

void LoadingScene::GenerateMatrices()
{
	//===============================================
	// Generate World/Ortho/Projection/View Matrices
	//===============================================

	Camera::Instance()->Render();
	Camera::Instance()->GetViewMatrix(ViewMatrix_);
	Camera::Instance()->Get2DViewMatrix(BaseViewMatrix_);

	DirectXManager::Instance()->GetWorldMatrix(WorldMatrix_);
	DirectXManager::Instance()->GetProjectionMatrix(ProjectionMatrix_);
	DirectXManager::Instance()->GetOrthoMatrix(OrthoMatrix_);
}

void LoadingScene::SetShaderManagerVars()
{
	return;
}