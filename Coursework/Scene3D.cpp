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

// File Loading
void Scene3D::SetDirectory(string FileDirectory)
{
	FileDirectory_ = FileDirectory;
}

// GameObject Creation
AudioClip* Scene3D::CreateSound(char* filename, float Volume, bool is3D, D3DXVECTOR3 position)
{
	// Create a sound
	AudioClip* Sound = new AudioClip;
	if (!Sound)
	{
		OutputErrorMessage("Could not create sound", " - Scene3D");
		return 0;
	}

	// Load the file
	Result_ = Sound->LoadFile(filename, is3D);
	if (!Result_)
	{
		OutputErrorMessage("Could not load sound", " - Scene3D");
		return 0;
	}

	// Set the properties
	Sound->SetVolume(Volume);
	Sound->SetPosition(position);

	return Sound;
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
