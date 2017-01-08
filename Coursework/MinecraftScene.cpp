#include "MinecraftScene.h"
#include "ApplicationManager.h"
#include "PerformanceManager.h"
#include "WindowManager.h"

MinecraftScene::MinecraftScene()
{
	// initialise pointers
	m_clouds = 0;
	m_fire = 0;
	m_campFire = 0;
	m_refractionTexture = 0;
	m_reflectionTexture = 0;
	m_renderTexture = 0;
	m_windowSprite = 0;
	m_skySphere = 0;
	m_text = 0;
	Ocean_ = 0;
	m_voxelWorld = 0;
}

MinecraftScene::~MinecraftScene()
{
}

// initialise
bool MinecraftScene::initialise(HWND hwnd)
{
	int windowWidth;
	int windowHeight;

	// get window width and height
	windowWidth = WindowManager::getInstance()->getWindowResolution().width;
	windowHeight = WindowManager::getInstance()->getWindowResolution().height;

	//============================
	// initialise render Textures
	//============================

	AssetManager::getInstance()->loadTexture(&m_renderTexture,		"renderTexture",	 Rect2D(windowWidth, windowHeight));
	AssetManager::getInstance()->loadTexture(&m_reflectionTexture,  "ReflectionTexture", Rect2D(windowWidth, windowHeight));
	AssetManager::getInstance()->loadTexture(&m_refractionTexture,  "RefractionTexture", Rect2D(windowWidth, windowHeight));
	
	//======================
	// initialise the Ocean 
	//======================

	Ocean_ = new Ocean;
	if (!Ocean_)
	{
		return false;
	}
	m_result = Ocean_->initialise("water_normal.dds", Rect3D(512.0f, 512.0f, 0.0f));
	if (!m_result)
	{
		MessageBox(hwnd, L"Could not initialise the ocean object.", L"Error", MB_OK);
		return false;
	}
	Ocean_->getTransform()->setX(256);
	Ocean_->getTransform()->setZ(256);

	//=======================
	// initialise 2D Bitmaps
	//=======================

	// UI Cursor
	m_cursor = new Sprite;
	if (!m_cursor)
	{
		return false;
	}
	m_result = m_cursor->initialise(Rect3D(72, 72), "ui_cursor.dds");
	if (!m_result)
	{
		MessageBox(hwnd, L"Could not initialise the action bar object.", L"Error", MB_OK);
		return false;
	}
	m_cursor->setShader("texture");
	m_cursor->setBlendMode(BlendMode::AlphaMasked);

	// Post Processing Window
	m_windowSprite = new Sprite;
	if (!m_windowSprite)
	{
		return false;
	}
	m_result = m_windowSprite->initialise(Rect3D(WindowManager::getInstance()->getWindowResolution()), "renderTexture");
	if (!m_result)
	{
		MessageBox(hwnd, L"Could not initialise the screen window object.", L"Error", MB_OK);
		return false;
	}
	m_windowSprite->setShader("tint");
	m_windowSprite->getModel()->getMaterial()->setVector4("BaseColour", Colour::Water);

	// Toolbar UI Icons
	m_toolbarInterface = new Toolbar;
	if (!m_toolbarInterface)
	{
		return false;
	}
	m_result = m_toolbarInterface->initialise();
	if (!m_result)
	{
		return false;
	}


	//====================
	// initialise the Sky
	//====================

	m_clouds = new Clouds;
	if (!m_clouds)
	{
		return false;
	}
	m_result = m_clouds->initialise("cloud.dds", "cloudperturb.dds");
	if (!m_result)
	{
		MessageBox(hwnd, L"Could not initialise the sky plane object.", L"Error", MB_OK);
		return false;
	}

	m_skySphere = new SkySphere;
	if (!m_skySphere)
	{
		return false;
	}
	m_skySphere->initialise("sphere.txt");
	if (!m_result)
	{
		MessageBox(hwnd, L"Could not initialise the sky sphere object.", L"Error", MB_OK);
		return false;
	}

	//===================
	// initialise Sounds
	//===================

	AssetManager::getInstance()->loadAudio(&m_ambientSound, "ambience.wav");
	m_ambientSound->setVolume(1.0f);

	//=================
	// initialise Text
	//=================

	m_text = new Text;
	if (!m_text)
	{
		return false;
	}
	m_result = m_text->initialise(hwnd, "shruti.txt", "shruti.dds", 95);
	if (!m_result)
	{
		MessageBox(hwnd, L"Could not initialise the text object.", L"Error", MB_OK);
		return false;
	}
	m_text->createText("FPS:", Vector2(10, 10), Colour::White); // FPS
	m_text->createText("CPU:", Vector2(10, 30), Colour::White); // CPU
	m_text->createText("Camera X :", Vector2(10, 50), Colour::White); // CameraX
	m_text->createText("Camera Y :", Vector2(10, 70), Colour::White); // CameraY
	m_text->createText("Camera Z :", Vector2(10, 90), Colour::White); // CameraZ
	m_text->createText("Rotation X :", Vector2(10, 110), Colour::White); // RotationX
	m_text->createText("Rotation Y :", Vector2(10, 130), Colour::White); // RotationY
	m_text->createText("Rotation Z :", Vector2(10, 150), Colour::White); // Rotation Z
	m_text->createText("build Time (s) :", Vector2(10, 170), Colour::White);

	m_text->createText("CONTROLS", Vector2(windowWidth - 10, 10), Colour::White, RIGHT);
	m_text->createText("Toggle Wireframe [9]", Vector2(windowWidth - 10, 30), Colour::White, RIGHT);
	m_text->createText("Toggle Time [0]", Vector2(windowWidth - 10, 50), Colour::White, RIGHT);
	m_text->createText("Back to Menu [ESC]", Vector2(windowWidth - 10, 110), Colour::White, RIGHT);

	//==============
	// create World
	//==============

	m_voxelWorld = new VoxelWorld;
	m_voxelWorld->initialise();

	//==================
	// initialise flags
	//==================

	m_whiteOut = false;
	m_nightTime = false;
	m_result = false;
	m_isloaded = true;
	
	return true;
}

// terminate
void MinecraftScene::terminate()
{
	//===================
	// Shut down objects
	//===================
	
	if (m_clouds)
	{
		m_clouds->terminate();
		delete m_clouds;
		m_clouds = 0;
	}

	if (m_fire)
	{
		m_fire->terminate();
		delete m_fire;
		m_fire = 0;
	}

	if (m_windowSprite)
	{
		m_windowSprite->terminate();
		delete m_windowSprite;
		m_windowSprite = 0;
	}

	if (m_skySphere)
	{
		m_skySphere->terminate();
		delete m_skySphere;
		m_skySphere = 0;
	}

	if (m_text)
	{
		m_text->terminate();
		delete m_text;
		m_text = 0;
	}

	if (Ocean_)
	{
		Ocean_->terminate();
		delete Ocean_;
		Ocean_ = 0;
	}

	return;
}

// onload and Unloading
void MinecraftScene::onLoad()
{
	//===============
	// Reset Objects
	//===============

	// Reset Mouse
	lockMouseToCenter();
	SetCursor(0);
	SetClassLong(WindowManager::getInstance()->getHWND(), GCL_HCURSOR, 0);

	//============
	// play Sound
	//============

	m_ambientSound->play(true);
}

void MinecraftScene::onUnload()
{
	//============
	// stop Audio
	//============

	m_ambientSound->stop();
	return;
}

// update
bool MinecraftScene::update()
{
	// handle user InputManager
	m_result = handleInput();
	if (!m_result)
	{
		return false;
	}

	// handle frame updates
	m_result = handleObjects();
	if (!m_result)
	{
		return false;
	}

	// render the scene
	m_result = render();
	if (!m_result)
	{
		return false;
	}

	return true;
}

bool MinecraftScene::render()
{
	//================
	// Reset Textures
	//================

	m_renderTexture->clearRenderTarget(Colour::Black);
	m_reflectionTexture->clearRenderTarget(Colour::Black);
	m_refractionTexture->clearRenderTarget(Colour::Black);

	//==============
	// render Scene
	//==============

	// render the scene normally to the screen
	m_result = renderScene();
	if (!m_result)
	{
		return false;
	}

	return true;
}

bool MinecraftScene::renderScene()
{
	// Begin rendering
	DirectXManager::getInstance()->beginScene();

	Camera::getInstance()->render();
	Camera::getInstance()->renderReflection(0.0f);

	//================
	// render the Sky 
	//================

	m_result = m_skySphere->render();
	if (!m_result)
	{
		return false;
	}

	m_result = m_clouds->render();
	if (!m_result)
	{
		return false;
	}

	//==================
	// render the World 
	//==================

	m_voxelWorld->render();

	m_result = Ocean_->render();
	if (!m_result)
	{
		return false;
	}

	//============
	// render GUI
	//============

	m_result = m_text->render();
	if (!m_result)
	{
		return false;
	}

	m_result = m_toolbarInterface->render();
	if (!m_result)
	{
		return false;
	}

	m_toolbarInterface->render();

	m_result = m_cursor->render();
	if (!m_result)
	{
		return false;
	}

	//========================
	// render Post Processing
	//========================

	if (m_whiteOut)
	{
		m_result = m_windowSprite->render();
		if (!m_result)
		{
			return false;
		}
	}

	// End rendering
	DirectXManager::getInstance()->endScene();

	return true;
}

// handlers
bool MinecraftScene::handleObjects()
{
	//=======================
	// update System Objects
	//=======================

	InputManager::getInstance()->update();
	PerformanceManager::getInstance()->update();
	ViewFrustumManager::getInstance()->update();

	//======================
	// update Scene Objects
	//======================

	m_voxelWorld->update();
	m_clouds->update();
	Ocean_->update();
	m_skySphere->update();
	m_toolbarInterface->update();
	
	//===========================
	// update System Information
	//===========================

	m_result = handleText();
	if (!m_result)
	{
		return false;
	}

	return true;
}

bool MinecraftScene::handleText()
{
	//===========================
	// update System Information
	//===========================

	m_result = m_text->setValue(0, PerformanceManager::getInstance()->getFPS());
	if (!m_result) { return false; }

	m_result = m_text->setValue(1, PerformanceManager::getInstance()->getUsage());
	if (!m_result) { return false; }

	m_result = m_text->setValue(2, (int)Camera::getInstance()->getTransform()->getX());
	if (!m_result) { return false; }

	m_result = m_text->setValue(3, (int)Camera::getInstance()->getTransform()->getY());
	if (!m_result) { return false; }

	m_result = m_text->setValue(4, (int)Camera::getInstance()->getTransform()->getZ());
	if (!m_result) { return false; }

	m_result = m_text->setValue(5, (int)Camera::getInstance()->getTransform()->getPitch());
	if (!m_result) { return false; }

	m_result = m_text->setValue(6, (int)Camera::getInstance()->getTransform()->getYaw());
	if (!m_result) { return false; }

	m_result = m_text->setValue(7, (int)Camera::getInstance()->getTransform()->getRoll());
	if (!m_result) { return false; }

	m_result = m_text->setValue(8, m_voxelWorld->getBuildTime());
	if (!m_result) { return false; }

	return true;
}

bool MinecraftScene::handleInput()
{
	//=================
	// Go Back to Menu
	//=================

	if (InputManager::getInstance()->getKeyDown(VK_ESCAPE))
	{
		ApplicationManager::getInstance()->setScene(SceneState::MAINMENU);
	}

	//=======================
	// Toggle Wireframe Mode
	//=======================

	if (InputManager::getInstance()->getKeyDown(VK_9))
	{
		// Switch to the opposite state for wireframe mode
		DirectXManager::getInstance()->toggleWireframe(!DirectXManager::getInstance()->getWireframeStatus());
	}

	//=============
	// Toggle Time
	//=============

	if (InputManager::getInstance()->getKeyDown(VK_0))
	{
		// Switch to the opposite state
		m_nightTime = !m_nightTime;

		// Toggle objects
		Light::getInstance()->toggleTime(m_nightTime);
		m_skySphere->toggleTime(m_nightTime);
		Ocean_->toggleTime(m_nightTime);
		m_whiteOut = !m_whiteOut;
	}

	return true;
}