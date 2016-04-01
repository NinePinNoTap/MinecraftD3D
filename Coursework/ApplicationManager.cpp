#include "ApplicationManager.h"

ApplicationManager::ApplicationManager()
{
	// Initialise pointers to null
	Camera_ = 0;
	CurrentScene_ = 0;
	DirectXManager_ = 0;
	DirectSound_ = 0;
	InputManager_ = 0;
	Light_ = 0;
	MainScene_ = 0;
	ShaderManager_ = 0;
	PerformanceManager_ = 0;
	SceneLoading_ = 0;
}

ApplicationManager::ApplicationManager(const ApplicationManager& other)
{
}

ApplicationManager::~ApplicationManager()
{
}

bool ApplicationManager::Initialise(HWND hwnd, Rect2D WindowResolution)
{
	//===================
	// Initialise Camera
	//===================

	Camera_ = new Camera;
	if (!Camera_) { return false; }
	Camera_->Initialise();

	//
	//
	//

	AssetManager_ = new AssetManager;
	if (!AssetManager_)
	{
		MessageBox(hwnd, L"Could not create the Asset Manager.", L"Error", MB_OK);
		return false;
	}

	//====================
	// Initialise DirectX
	//====================

	DirectXManager_ = new DirectXManager;
	if (!DirectXManager_)
	{
		MessageBox(hwnd, L"Could not create the DirectX Manager.", L"Error", MB_OK);
		return false;
	}

	Result_ = DirectXManager_->Initialise(WindowResolution, hwnd);
	if (!Result_)
	{
		MessageBox(hwnd, L"Could not initialise DirectX Manager.", L"Error", MB_OK);
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

	//==================
	// Initialise Light
	//==================

	Light_ = new Light;
	if (!Light_) { return false; }
	Light_->Initialise();
	Light_->GetTransform()->SetPosition(-3500.0f, 9900.0f, 2100.0f);
	Light_->SetAmbientColor(0.5f, 0.5f, 0.5f, 1.0f);
	Light_->SetDiffuseColor(0.5f, 0.5f, 0.5f, 1.0f);
	Light_->SetDirection(0.5f, -0.75f, 0.25f);
	Light_->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);

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

	if (AssetManager_)
	{
		AssetManager_->Shutdown();
		AssetManager_ = 0;
	}
	if (Camera_)
	{
		Camera_->Shutdown();
		delete Camera_;
		Camera_ = 0;
	}

	if (DirectXManager_)
	{
		DirectXManager_->Shutdown();
		DirectXManager_ = 0;
	}

	if (DirectSound_)
	{
		DirectSound_->Shutdown();
		DirectSound_ = 0;
	}

	if (InputManager_)
	{
		delete InputManager_;
		InputManager_ = 0;
	}

	if (Light_)
	{
		Light_->Shutdown();
		delete Light_;
		Light_ = 0;
	}

	if (ShaderManager_)
	{
		ShaderManager_->Shutdown();
		ShaderManager_ = 0;
	}
	if (PerformanceManager_)
	{
		PerformanceManager_->Shutdown();
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