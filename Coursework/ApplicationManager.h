#pragma once

#include <thread>

#include "Camera.h"

#include "DirectXManager.h"
#include "DirectSound.h"
#include "Light.h"
#include "MinecraftScene.h"
#include "PerformanceManager.h"
#include "GameScene.h"
#include "loadingScene.h"
#include "MainMenuScene.h"
#include "ShaderManager.h"
#include "ViewFrustumManager.h"

class ApplicationManager : public Singleton<ApplicationManager>
{
public:
	ApplicationManager();
	ApplicationManager(const ApplicationManager&);
	~ApplicationManager();

	// Initialising
	bool initialise(HWND hwnd, Rect2D WindowResolution);

	// terminate
	void terminate();

	// update
	bool update();

	// Scene Switching
	void setScene(SceneState scene);

	// Scene isComplete
	bool checkSceneLoaded(SceneState scene);
	
private:
	// update
	void updateScene();

	// Variables
	bool m_result; 

	// Manager
	AssetManager* m_assetManager;
	DirectXManager* m_directX;
	DirectSound* m_directSound;
	InputManager* m_inputManager;
	ShaderManager* m_shaderManager;
	PerformanceManager* m_performanceManager;
	ViewFrustumManager* m_viewFrustumManager;
	
	// Scenes
	MainMenuScene* m_mainMenuScene;
	MinecraftScene* m_minecraftScene;
	LoadingScene* m_loadingScene;
	ManagedThread<bool()>* m_loadingSceneThread;

	// Global Objects
	Camera* m_camera;
	Light* m_light;

	// Scene Switching
	GameScene* m_currentScene;
	SceneState m_currentState;
	SceneState m_newSceneState;
	bool m_changeScene;
};


