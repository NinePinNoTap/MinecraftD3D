#include "Scene3D.h"

Scene3D::Scene3D()
{
	// Initialise pointers
	SceneTexture_ = 0;
	Text_ = 0;
}

Scene3D::~Scene3D()
{

}

// Shutdown
void Scene3D::ShutdownGameObjects()
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

void Scene3D::SetShaderManager2DVars()
{
	//==============================================
	// Send 2D Rendering Matrices to Shader Manager
	//==============================================

	ShaderManager::Instance()->SetWorldMatrix(WorldMatrix_);
	ShaderManager::Instance()->SetProjectionMatrix(OrthoMatrix_);
	ShaderManager::Instance()->SetViewMatrix(BaseViewMatrix_);
}
