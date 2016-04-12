#pragma once

#include <thread>

#include "Camera.h"
#include "Constants.h"
#include "DirectXManager.h"
#include "DirectSound.h"
#include "Light.h"
#include "MinecraftScene.h"
#include "PerformanceManager.h"
#include "GameScene.h"
#include "LoadingScene.h"
#include "ShaderManager.h"
#include "ViewFrustumManager.h"

enum SceneState
{
	GAME,
	LOADING
};

class ApplicationManager : public Singleton<ApplicationManager>
{
public:
	ApplicationManager();
	ApplicationManager(const ApplicationManager&);
	~ApplicationManager();

	// Initialising
	bool Initialise(HWND hwnd, Rect2D WindowResolution);

	// Shutdown
	void Shutdown();

	// Frame
	bool Frame();

	// Scene Switching
	void SetScene(SceneState scene);

	SceneState GetScene() { return NewScene_; }
	
private:
	// Update
	void UpdateScene();

	// Variables
	bool Result_; 

	// Manager
	AssetManager* AssetManager_;
	DirectXManager* DirectXManager_;
	DirectSound* DirectSound_;
	InputManager* InputManager_;
	ShaderManager* ShaderManager_;
	PerformanceManager* PerformanceManager_;
	ViewFrustumManager* ViewFrustumManager_;
	
	// Scenes
	MinecraftScene* MinecraftScene_;
	LoadingScene* LoadingScene_;
	std::thread LoadingScreenThread_;

	// Global Objects
	Camera* Camera_;
	Light* Light_;

	// Scene Switching
	GameScene* CurrentScene_;
	SceneState NewScene_;
	bool ChangeScene_;
};


