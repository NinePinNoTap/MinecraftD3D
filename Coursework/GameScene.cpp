#include "GameScene.h"

GameScene::GameScene()
{
	// Initialise pointers
	Text_ = 0;
}

GameScene::~GameScene()
{

}

// Shutdown
void GameScene::ShutdownGameObjects()
{
	// Shutdown each gameobject
	for each (GameObject* obj in Objects_)
	{
		obj->Shutdown();
		delete obj;
		obj = 0;
	}

	// Clean Up
	Objects_.clear();
}

void GameScene::SetShaderManager2DVars()
{
	//==============================================
	// Send 2D Rendering Matrices to Shader Manager
	//==============================================

	ShaderManager::Instance()->SetWorldMatrix(WorldMatrix_);
	ShaderManager::Instance()->SetProjectionMatrix(OrthoMatrix_);
	ShaderManager::Instance()->SetViewMatrix(BaseViewMatrix_);
}
