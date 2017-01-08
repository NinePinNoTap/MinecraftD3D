#include "loadingScene.h"
#include "WindowManager.h"

LoadingScene::LoadingScene() : GameScene()
{
	m_background = 0;
}

LoadingScene::~LoadingScene()
{

}

// Initialising
bool LoadingScene::initialise()
{
	int windowWidth;
	int windowHeight;

	// get window width and height
	windowWidth = WindowManager::getInstance()->getWindowResolution().width;
	windowHeight = WindowManager::getInstance()->getWindowResolution().height;
	
	//===============================
	// initialise the loading Screen
	//===============================

	// loading Screen BG
	m_background = new Sprite;
	if (!m_background)
	{
		return false;
	}
	m_result = m_background->initialise(Rect3D(windowWidth, windowHeight, 1.0f), "bg_loading.dds");
	if (!m_result)
	{
		return false;
	}
	m_background->setShader("texture");

	//============
	// initialise
	//============

	m_loadTarget = SceneState::NO_SCENE;
	m_isloaded = true;

	return true;
}

// terminate
void LoadingScene::terminate()
{
	if (m_background)
	{
		m_background->terminate();
		m_background = 0;
	}
}

// update
bool LoadingScene::update()
{
	// Check for Scene Processing
	if (m_loadTarget != SceneState::NO_SCENE)
	{
		// Check if we can switch
		if (ApplicationManager::getInstance()->checkSceneLoaded(m_loadTarget))
		{
			ApplicationManager::getInstance()->setScene(m_loadTarget);
		}
	}

	// render
	render();

	return true;
}

void LoadingScene::render()
{
	DirectXManager::getInstance()->beginScene();

	// render the loading screen
	m_background->render();

	DirectXManager::getInstance()->endScene();
}

// onload and Unloading
void LoadingScene::onLoad()
{
	return;
}

void LoadingScene::onUnload()
{
	// Clear Target
	m_loadTarget = SceneState::NO_SCENE;
}

void LoadingScene::loadScene(SceneState sceneState)
{
	// Store the loading target
	m_loadTarget = sceneState;
}