#include "ApplicationManager.h"

ApplicationManager::ApplicationManager()
{
	// initialise pointers to null
	m_camera = 0;
	m_currentScene = 0;
	m_directX = 0;
	m_directSound = 0;
	m_inputManager = 0;
	m_light = 0;
	m_minecraftScene = 0;
	m_shaderManager = 0;
	m_performanceManager = 0;
	m_loadingScene = 0;
}

ApplicationManager::ApplicationManager(const ApplicationManager& other)
{
}

ApplicationManager::~ApplicationManager()
{
}

// initialise
bool ApplicationManager::initialise(HWND hwnd, Rect2D WindowResolution)
{
	//===================
	// initialise Camera
	//===================

	m_camera = new Camera;
	if (!m_camera)
	{
		return false;
	}
	m_camera->initialise();

	//=========================
	// initialise AssetManager
	//=========================

	m_assetManager = new AssetManager;
	if (!m_assetManager)
	{
		MessageBox(hwnd, L"Could not create the Asset Manager.", L"Error", MB_OK);
		return false;
	}

	//====================
	// initialise DirectX
	//====================

	m_directX = new DirectXManager;
	if (!m_directX)
	{
		MessageBox(hwnd, L"Could not create the DirectX Manager.", L"Error", MB_OK);
		return false;
	}

	m_result = m_directX->initialise(WindowResolution, hwnd);
	if (!m_result)
	{
		MessageBox(hwnd, L"Could not initialise DirectX Manager.", L"Error", MB_OK);
		return false;
	}
	
	//========================
	// initialise DirectSound
	//========================

	m_directSound = new DirectSound;
	if (!m_directSound)
	{
		MessageBox(hwnd, L"Could not create the DirectSound engine.", L"Error", MB_OK);
		return false;
	}

	m_result = m_directSound->initialise(hwnd);
	if (!m_result)
	{
		MessageBox(hwnd, L"Could not initialise DirectSound.", L"Error", MB_OK);
		return false;
	}

	//=========================
	// initialise InputManager
	//=========================

	m_inputManager = new InputManager;
	if (!m_inputManager)
	{
		MessageBox(hwnd, L"Could not create the InputManager Manager", L"Error", MB_OK);
		return false;
	}
	m_inputManager->initialise();

	//==================
	// initialise Light
	//==================

	m_light = new Light;
	if (!m_light)
	{
		return false;
	}
	m_light->initialise();
	m_light->getTransform()->setPosition(-3500.0f, 9900.0f, 2100.0f);
	m_light->setAmbientColor(0.5f, 0.5f, 0.5f, 1.0f);
	m_light->setDiffuseColor(0.5f, 0.5f, 0.5f, 1.0f);
	m_light->setDirection(0.5f, -0.75f, 0.25f);
	m_light->setSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);

	//==========================
	// initialise ShaderManager
	//==========================

	m_shaderManager = new ShaderManager;
	if(!m_shaderManager)
	{
		MessageBox(hwnd, L"Could not create the Shader engine.", L"Error", MB_OK);
		return false;
	}
	m_result = m_shaderManager->initialise(hwnd);
	if (!m_result)
	{
		MessageBox(hwnd, L"Could not initialise the Shader Engine.", L"Error", MB_OK);
		return false;
	}

	//========================
	// initialise Performance
	//========================

	m_performanceManager = new PerformanceManager;
	if (!m_performanceManager)
	{
		MessageBox(hwnd, L"Could not create the Shader engine.", L"Error", MB_OK);
		return false;
	}
	m_performanceManager->initialise();

	//=========================
	// initialise View Frustum
	//=========================

	m_viewFrustumManager = new ViewFrustumManager;

	//===========================
	// initialise loading Screen
	//===========================

	m_loadingScene = new LoadingScene;
	m_result = m_loadingScene->initialise();
	if (!m_result)
	{
		MessageBox(hwnd, L"Could not initialise the loading screen", L"Error", MB_OK);
		return false;
	}

	// set starting scene as loading and render it
	setScene(LOADING);

	// Refresh current scene
	updateScene();

	// create a thread to keep the application rendering until we have finished loading
	m_loadingSceneThread = new ManagedThread < bool() > ;
	m_loadingSceneThread->setFunction(std::bind(&ApplicationManager::update, this));
	m_loadingSceneThread->begin(true);

	//======================
	// initialise Main Menu
	//======================

	m_mainMenuScene = new MainMenuScene;
	m_result = m_mainMenuScene->initialise(hwnd);
	if (!m_result)
	{
		MessageBox(hwnd, L"Could not initialise the main menu scene", L"Error", MB_OK);
		return false;
	}

	//=================
	// initialise Game
	//=================
	
	m_minecraftScene = new MinecraftScene;
	m_result = m_minecraftScene->initialise(hwnd);
	if (!m_result)
	{
		MessageBox(hwnd, L"Could not initialise the minecraft scene", L"Error", MB_OK);
		return false;
	}

	// Switch once we have loaded
	setScene(MAINMENU);

	// Close the thread
	m_loadingSceneThread->forceQuit();

	outputToDebug("isComplete loading");

	return true;
}

// terminate
void ApplicationManager::terminate()
{
	//=================
	// terminate Scenes
	//=================

	if (m_minecraftScene)
	{
		m_minecraftScene->terminate();
		delete m_minecraftScene;
		m_minecraftScene = 0;
	}

	if (m_loadingScene)
	{
		m_loadingScene->terminate();
		delete m_loadingScene;
		m_loadingScene = 0;
	}

	//=====================
	// terminate Singletons
	//=====================

	if (m_assetManager)
	{
		m_assetManager->terminate();
		m_assetManager = 0;
	}
	if (m_camera)
	{
		m_camera->terminate();
		delete m_camera;
		m_camera = 0;
	}

	if (m_directX)
	{
		m_directX->terminate();
		m_directX = 0;
	}

	if (m_directSound)
	{
		m_directSound->terminate();
		m_directSound = 0;
	}

	if (m_inputManager)
	{
		delete m_inputManager;
		m_inputManager = 0;
	}

	if (m_light)
	{
		m_light->terminate();
		delete m_light;
		m_light = 0;
	}

	if (m_shaderManager)
	{
		m_shaderManager->terminate();
		m_shaderManager = 0;
	}
	if (m_performanceManager)
	{
		m_performanceManager->terminate();
		m_performanceManager = 0;
	}

	// Clean Up Pointers
	m_currentScene = 0;

	return;
}

// update
bool ApplicationManager::update()
{
	//==========================
	// Check for a scene change
	//==========================

	if (m_changeScene)
	{
		updateScene();
	}

	//=================
	// Process a update
	//=================

	m_result = m_currentScene->update();
	if (!m_result)
	{
		return false;
	}

	return true;
}

// Scene Management
void ApplicationManager::updateScene()
{
	// onUnload the current scene
	if (m_currentScene)
	{
		m_currentScene->onUnload();
	}

	// Check which scene to change to
	switch (m_newSceneState)
	{
		case MAINMENU:
			m_currentScene = m_mainMenuScene;
			break;

		case LOADING:
			m_currentScene = m_loadingScene;
			break;

		case MINECRAFT:
			m_currentScene = m_minecraftScene;
			break;

		case NO_SCENE:
			m_newSceneState = MAINMENU;
			updateScene();
			return;
			break;

		case EXIT:
			exit(1);
			return;
			break;
	}

	// update current state
	m_currentState = m_newSceneState;
	m_newSceneState = SceneState::NO_SCENE;
	m_changeScene = false;

	// onload the new scene
	m_currentScene->onLoad();
}

void ApplicationManager::setScene(SceneState scene)
{
	m_newSceneState = scene;
	m_changeScene = true;
}

bool ApplicationManager::checkSceneLoaded(SceneState scene)
{
	// Check which scene to change to
	switch (m_newSceneState)
	{
		case MAINMENU:
			m_mainMenuScene->isLoaded();
			break;

		case LOADING:
			m_loadingScene->isLoaded();
			break;

		case MINECRAFT:
			m_minecraftScene->isLoaded();
			break;

		default:
			return false;
			break;
	}

	return false;
}