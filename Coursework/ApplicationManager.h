#pragma once

#include "DirectXManager.h"
#include "DirectSound.h"
#include "Constants.h"
#include "PerformanceManager.h"
#include "Scene3D.h"
#include "MainScene.h"
#include "SceneLoadingScreen.h"
#include "ShaderManager.h"
#include "DataStructs.h"

enum SceneState
{
	MAIN,
	LOADING
};

class ApplicationManager : public Singleton<ApplicationManager>
{
public:
	ApplicationManager();
	ApplicationManager(const ApplicationManager&);
	~ApplicationManager();

	// Initialising
	bool Initialise(HWND hwnd, ScreenResolution WindowResolution);

	// Shutdown
	void Shutdown();

	// Frame
	bool Frame();

	// Scene Switching
	void SetScene(SceneState scene);
	
private:
	// Update
	void UpdateScene();

	// Variables
	bool Result_; 

	// Manager
	DirectXManager* DirectXManager_;
	DirectSound* DirectSound_;
	InputManager* InputManager_;
	ShaderManager* ShaderManager_;
	PerformanceManager* PerformanceManager_;
	
	// Scenes
	MainScene* MainScene_;
	SceneLoadingScreen* SceneLoading_;

	// Scene Switching
	Scene3D* CurrentScene_;
	SceneState NewScene_;
	bool ChangeScene_;
};


