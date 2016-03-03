#include "ApplicationManager.h"

ApplicationManager::ApplicationManager()
{
	// Initialise pointers to null
	MainScene_ = 0;
	CurrentScene_ = 0;
}

ApplicationManager::ApplicationManager(const ApplicationManager& other)
{
}

ApplicationManager::~ApplicationManager()
{
}

bool ApplicationManager::Initialise(HWND hwnd, ScreenResolution WindowResolution)
{
	//====================
	// Initialise DirectX
	//====================

	DirectXManager_ = new DirectXManager;
	if (!DirectXManager_)
	{
		MessageBox(hwnd, L"Could not create the DirectXManager.", L"Error", MB_OK);
		return false;
	}

	Result_ = DirectXManager_->Initialise(WindowResolution, hwnd);
	if (!Result_)
	{
		MessageBox(hwnd, L"Could not initialise DirectXManager.", L"Error", MB_OK);
		return false;
	}
	
	//========================
	// Initialise DirectSound
	//========================

	DirectSound_ = new DirectSound;
	if (!DirectSound_)
	{
		MessageBox(hwnd, L"Could not create the DirectSound engine.", L"Error", MB_OK);
		return false;
	}

	Result_ = DirectSound_->Initialise(hwnd);
	if (!Result_)
	{
		MessageBox(hwnd, L"Could not initialise DirectSound.", L"Error", MB_OK);
		return false;
	}

	//=========================
	// Initialise InputManager
	//=========================

	InputManager_ = new InputManager;
	if (!InputManager_)
	{
		MessageBox(hwnd, L"Could not create the InputManager Manager", L"Error", MB_OK);
		return false;
	}
	InputManager_->Initialise();

	//==========================
	// Initialise ShaderManager
	//==========================

	ShaderManager_ = new ShaderManager;
	if(!ShaderManager_)
	{
		MessageBox(hwnd, L"Could not create the Shader engine.", L"Error", MB_OK);
		return false;
	}
	Result_ = ShaderManager_->Initialise(hwnd);
	if (!Result_)
	{
		MessageBox(hwnd, L"Could not initialise the Shader Engine.", L"Error", MB_OK);
		return false;
	}

	//========================
	// Initialise Performance
	//========================

	PerformanceManager_ = new PerformanceManager;
	if (!PerformanceManager_)
	{
		MessageBox(hwnd, L"Could not create the Shader engine.", L"Error", MB_OK);
		return false;
	}
	PerformanceManager_->Initialise();

	//===================
	// Initialise Scenes
	//===================

	// Loading Screen
	SceneLoading_ = new SceneLoadingScreen;
	Result_ = SceneLoading_->Initialise(WindowResolution);
	if (!Result_)
	{
		MessageBox(hwnd, L"Could not initialise the loading screen", L"Error", MB_OK);
		return false;
	}

	// Set starting scene as loading and render it
	SetScene(LOADING);
	Frame();

	// Inside the Painting
	MainScene_ = new MainScene;
	Result_ = MainScene_->Initialise(hwnd, WindowResolution);
	if (!Result_)
	{
		MessageBox(hwnd, L"Could not initialise the inside scene", L"Error", MB_OK);
		return false;
	}
		
	// Set Default Scene
	SetScene(MAIN);
	CurrentScene_ = MainScene_;

	return true;
}

void ApplicationManager::Shutdown()
{
	//=================
	// Shutdown Scenes
	//=================

	if (MainScene_)
	{
		MainScene_ -> Shutdown();
		delete MainScene_;
		MainScene_ = 0;
	}

	if (SceneLoading_)
	{
		SceneLoading_->Shutdown();
		delete SceneLoading_;
		SceneLoading_ = 0;
	}

	//=====================
	// Shutdown Singletons
	//=====================

	if (DirectXManager_)
	{
		DirectXManager_->Shutdown();
		delete DirectXManager_;
		DirectXManager_ = 0;
	}
	if (DirectSound_)
	{
		DirectSound_->Shutdown();
		delete DirectSound_;
		DirectSound_ = 0;
	}
	if (InputManager_)
	{
		delete InputManager_;
		InputManager_ = 0;
	}
	if (ShaderManager_)
	{
		ShaderManager_->Shutdown();
		delete ShaderManager_;
		ShaderManager_ = 0;
	}
	if (PerformanceManager_)
	{
		PerformanceManager_->Shutdown();
		delete PerformanceManager_;
		PerformanceManager_ = 0;
	}

	// Clean Up Pointers
	CurrentScene_ = 0;

	return;
}

bool ApplicationManager::Frame()
{
	//==========================
	// Check for a scene change
	//==========================

	if (ChangeScene_)
	{
		UpdateScene();
	}

	//=================
	// Process a Frame
	//=================

	Result_ = CurrentScene_ -> Frame();
	if (!Result_)
	{
		return false;
	}

	return true;
}

void ApplicationManager::UpdateScene()
{
	// Check which scene to change to
	switch (NewScene_)
	{
		case MAIN:
			CurrentScene_ = MainScene_;
			break;

		case LOADING:
			CurrentScene_ = SceneLoading_;
			break;
	}

	// Reset the scene back to its original state
	CurrentScene_ -> Reset();

	// Unflag the change
	ChangeScene_ = false;
}

void ApplicationManager::SetScene(SceneState scene)
{
	NewScene_ = scene;
	ChangeScene_ = true;
}