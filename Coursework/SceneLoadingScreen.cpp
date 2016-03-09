#include "SceneLoadingScreen.h"

SceneLoadingScreen::SceneLoadingScreen() : Scene3D()
{
	Background_ = 0;
}
SceneLoadingScreen::~SceneLoadingScreen()
{

}

bool SceneLoadingScreen::Initialise(Rect2D WindowResolution)
{
	//=======================
	// Initialise the Camera
	//=======================

	Camera::Instance()->Get2DViewMatrix(BaseViewMatrix_);
	
	//===============================
	// Initialise the Loading Screen
	//===============================

	// Loading Screen BG
	Background_ = new Sprite;
	if (!Background_) { return false; }
	Result_ = Background_->Initialise(Rect3D(WindowResolution.width, WindowResolution.height));
	if (!Result_)
	{
		return false;
	}
	Result_ = Background_->SetTexture(L"Data/Textures/LoadingScreen.dds");
	if (!Result_)
	{
		return false;
	}
	Background_->GetTransform()->MoveZ(-5.0f);

	//================
	// Initialise GUI
	//================

	LoadingText_ = new Text;
	LoadingText_->Initialise(GetActiveWindow(), WindowResolution);
	LoadingText_->CreateText("Loading", Vector2(10, 10), WHITE, LEFT);
	LoadingProgress_ = 0;

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
	LoadingProgress_ += 3;
	Wrap(LoadingProgress_, 0, 300);

	LoadingText_->SetText(0, "Loading : ", LoadingProgress_);

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
	ShaderManager::Instance()->TextureRender(Background_);

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