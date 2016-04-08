#include "SceneLoadingScreen.h"
#include "WindowManager.h"

SceneLoadingScreen::SceneLoadingScreen() : Scene3D()
{
	Background_ = 0;
}
SceneLoadingScreen::~SceneLoadingScreen()
{

}

bool SceneLoadingScreen::Initialise()
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
	LoadingText_->CreateText("Load Time : ", Vector2(windowWidth - 50, windowHeight - 50), WHITE, RIGHT);
	LoadingProgress_ = timeGetTime();

	return true;
}

void SceneLoadingScreen::Reset()
{
	return;
}

void SceneLoadingScreen::Shutdown()
{
	if (Background_)
	{
		Background_->Shutdown();
		Background_ = 0;
	}
}

bool SceneLoadingScreen::Frame()
{
	LoadingText_->SetValue(0, (timeGetTime() - LoadingProgress_) / 1000);

	Render();

	return true;
}

void SceneLoadingScreen::Render()
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


void SceneLoadingScreen::GenerateMatrices()
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

void SceneLoadingScreen::SetShaderManagerVars()
{
	return;
}