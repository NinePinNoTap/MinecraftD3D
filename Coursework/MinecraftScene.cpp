#include "MinecraftScene.h"
#include "ApplicationManager.h"
#include "PerformanceManager.h"
#include "WindowManager.h"

MinecraftScene::MinecraftScene()
{
	// Initialise pointers
	Clouds_ = 0;
	Fire_ = 0;
	CampFire_ = 0;
	RefractionTexture_ = 0;
	ReflectionTexture_ = 0;
	RenderTexture_ = 0;
	WindowSprite_ = 0;
	SkySphere_ = 0;
	Text_ = 0;
	Ocean_ = 0;
	World_ = 0;
}

MinecraftScene::~MinecraftScene()
{
}

// Initialise
bool MinecraftScene::Initialise(HWND hwnd)
{
	int windowWidth;
	int windowHeight;

	// Get window width and height
	windowWidth = WindowManager::Instance()->GetWindowResolution().width;
	windowHeight = WindowManager::Instance()->GetWindowResolution().height;

	//============================
	// Initialise Render Textures
	//============================

	AssetManager::Instance()->LoadTexture(&RenderTexture_,		"RenderTexture",	 Rect2D(windowWidth, windowHeight));
	AssetManager::Instance()->LoadTexture(&ReflectionTexture_,  "ReflectionTexture", Rect2D(windowWidth, windowHeight));
	AssetManager::Instance()->LoadTexture(&RefractionTexture_,  "RefractionTexture", Rect2D(windowWidth, windowHeight));
	
	//======================
	// Initialise the Ocean 
	//======================

	Ocean_ = new Ocean;
	if (!Ocean_)
	{
		return false;
	}
	Result_ = Ocean_->Initialise("water_normal.dds", Rect3D(512.0f, 512.0f, 0.0f));
	if (!Result_)
	{
		MessageBox(hwnd, L"Could not initialise the ocean object.", L"Error", MB_OK);
		return false;
	}
	Ocean_->GetTransform()->SetX(256);
	Ocean_->GetTransform()->SetZ(256);

	//=======================
	// Initialise 2D Bitmaps
	//=======================

	// UI Cursor
	Cursor_ = new Sprite;
	if (!Cursor_)
	{
		return false;
	}
	Result_ = Cursor_->Initialise(Rect3D(72, 72), "ui_cursor.dds");
	if (!Result_)
	{
		MessageBox(hwnd, L"Could not initialise the action bar object.", L"Error", MB_OK);
		return false;
	}
	Cursor_->SetShader("texture");
	Cursor_->SetBlendMode(BlendMode::AlphaMasked);

	// Post Processing Window
	WindowSprite_ = new Sprite;
	if (!WindowSprite_)
	{
		return false;
	}
	Result_ = WindowSprite_->Initialise(Rect3D(WindowManager::Instance()->GetWindowResolution()), "RenderTexture");
	if (!Result_)
	{
		MessageBox(hwnd, L"Could not initialise the screen window object.", L"Error", MB_OK);
		return false;
	}
	WindowSprite_->SetShader("tint");
	WindowSprite_->GetModel()->GetMaterial()->SetVector4("BaseColour", Colour::Water);

	// Toolbar UI Icons
	ToolbarUI_ = new Toolbar;
	if (!ToolbarUI_)
	{
		return false;
	}
	Result_ = ToolbarUI_->Initialise();
	if (!Result_)
	{
		return false;
	}


	//====================
	// Initialise the Sky
	//====================

	Clouds_ = new Clouds;
	if (!Clouds_)
	{
		return false;
	}
	Result_ = Clouds_->Initialise("cloud.dds", "cloudperturb.dds");
	if (!Result_)
	{
		MessageBox(hwnd, L"Could not initialise the sky plane object.", L"Error", MB_OK);
		return false;
	}

	SkySphere_ = new SkySphere;
	if (!SkySphere_)
	{
		return false;
	}
	SkySphere_ -> Initialise("sphere.txt");
	if (!Result_)
	{
		MessageBox(hwnd, L"Could not initialise the sky sphere object.", L"Error", MB_OK);
		return false;
	}

	//===================
	// Initialise Sounds
	//===================

	AssetManager::Instance()->LoadAudio(&AmbientSound_, "ambience.wav");
	AmbientSound_->SetVolume(1.0f);

	//=================
	// Initialise Text
	//=================

	Text_ = new Text;
	if (!Text_)
	{
		return false;
	}
	Result_ = Text_->Initialise(hwnd, "shruti.txt", "shruti.dds", 95);
	if (!Result_)
	{
		MessageBox(hwnd, L"Could not initialise the text object.", L"Error", MB_OK);
		return false;
	}
	Text_->CreateText("FPS:", Vector2(10, 10), Colour::White); // FPS
	Text_->CreateText("CPU:", Vector2(10, 30), Colour::White); // CPU
	Text_->CreateText("Camera X :", Vector2(10, 50), Colour::White); // CameraX
	Text_->CreateText("Camera Y :", Vector2(10, 70), Colour::White); // CameraY
	Text_->CreateText("Camera Z :", Vector2(10, 90), Colour::White); // CameraZ
	Text_->CreateText("Rotation X :", Vector2(10, 110), Colour::White); // RotationX
	Text_->CreateText("Rotation Y :", Vector2(10, 130), Colour::White); // RotationY
	Text_->CreateText("Rotation Z :", Vector2(10, 150), Colour::White); // Rotation Z
	Text_->CreateText("Build Time (s) :", Vector2(10, 170), Colour::White);

	Text_->CreateText("CONTROLS", Vector2(windowWidth - 10, 10), Colour::White, RIGHT);
	Text_->CreateText("Toggle Wireframe [9]", Vector2(windowWidth - 10, 30), Colour::White, RIGHT);
	Text_->CreateText("Toggle Time [0]", Vector2(windowWidth - 10, 50), Colour::White, RIGHT);
	Text_->CreateText("Back to Menu [ESC]", Vector2(windowWidth - 10, 110), Colour::White, RIGHT);

	//==============
	// Create World
	//==============

	World_ = new VoxelWorld;
	World_->Initialise();

	//==================
	// Initialise flags
	//==================

	WhiteOut_ = false;
	NightTimeMode_ = false;
	Result_ = false;
	IsLoaded_ = true;
	
	return true;
}

// Shutdown
void MinecraftScene::Shutdown()
{
	//===================
	// Shut down objects
	//===================
	
	if (Clouds_)
	{
		Clouds_ -> Shutdown();
		delete Clouds_;
		Clouds_ = 0;
	}

	if (Fire_)
	{
		Fire_ -> Shutdown();
		delete Fire_;
		Fire_ = 0;
	}

	if (WindowSprite_)
	{
		WindowSprite_ -> Shutdown();
		delete WindowSprite_;
		WindowSprite_ = 0;
	}

	if (SkySphere_)
	{
		SkySphere_ -> Shutdown();
		delete SkySphere_;
		SkySphere_ = 0;
	}

	if (Text_)
	{
		Text_ -> Shutdown();
		delete Text_;
		Text_ = 0;
	}

	if (Ocean_)
	{
		Ocean_ -> Shutdown();
		delete Ocean_;
		Ocean_ = 0;
	}

	return;
}

// Load and Unloading
void MinecraftScene::Load()
{
	//===============
	// Reset Objects
	//===============

	// Reset Mouse
	LockMouseToCenter();
	SetCursor(0);
	SetClassLong(WindowManager::Instance()->GetHWND(), GCL_HCURSOR, 0);

	//============
	// Play Sound
	//============

	AmbientSound_->Play(true);
}

void MinecraftScene::Unload()
{
	//============
	// Stop Audio
	//============

	AmbientSound_->Stop();
	return;
}

// Frame
bool MinecraftScene::Frame()
{
	// Handle user InputManager
	Result_ = HandleInput();
	if (!Result_)
	{
		return false;
	}

	// Handle frame updates
	Result_ = HandleObjects();
	if (!Result_)
	{
		return false;
	}

	// Render the scene
	Result_ = Render();
	if (!Result_)
	{
		return false;
	}

	return true;
}

bool MinecraftScene::Render()
{
	//================
	// Reset Textures
	//================

	RenderTexture_->ClearRenderTarget(Colour::Black);
	ReflectionTexture_->ClearRenderTarget(Colour::Black);
	RefractionTexture_->ClearRenderTarget(Colour::Black);

	//==============
	// Render Scene
	//==============

	// Render the scene normally to the screen
	Result_ = RenderScene();
	if (!Result_)
	{
		return false;
	}

	return true;
}

bool MinecraftScene::RenderScene()
{
	// Begin rendering
	DirectXManager::Instance()->BeginScene();

	Camera::Instance()->Render();
	Camera::Instance()->RenderReflection(0.0f);

	//================
	// Render the Sky 
	//================

	Result_ = SkySphere_->Render();
	if (!Result_)
	{
		return false;
	}

	Result_ = Clouds_->Render();
	if (!Result_)
	{
		return false;
	}

	//==================
	// Render the World 
	//==================

	World_->Render();

	Result_ = Ocean_->Render();
	if (!Result_)
	{
		return false;
	}

	//============
	// Render GUI
	//============

	Result_ = Text_->Render();
	if (!Result_)
	{
		return false;
	}

	Result_ = ToolbarUI_->Render();
	if (!Result_)
	{
		return false;
	}

	ToolbarUI_->Render();

	Result_ = Cursor_->Render();
	if (!Result_)
	{
		return false;
	}

	//========================
	// Render Post Processing
	//========================

	if (WhiteOut_)
	{
		Result_ = WindowSprite_->Render();
		if (!Result_)
		{
			return false;
		}
	}

	// End rendering
	DirectXManager::Instance()->EndScene();

	return true;
}

// Handlers
bool MinecraftScene::HandleObjects()
{
	//=======================
	// Update System Objects
	//=======================

	InputManager::Instance()->Frame();
	PerformanceManager::Instance()->Frame();
	ViewFrustumManager::Instance()->Frame();

	//======================
	// Update Scene Objects
	//======================

	World_->Frame();
	Clouds_ -> Frame();
	Ocean_->Frame();
	SkySphere_->Frame();
	ToolbarUI_->Frame();
	
	//===========================
	// Update System Information
	//===========================

	Result_ = HandleText();
	if (!Result_)
	{
		return false;
	}

	return true;
}

bool MinecraftScene::HandleText()
{
	//===========================
	// Update System Information
	//===========================

	Result_ = Text_->SetValue(0, PerformanceManager::Instance()->GetFPS());
	if (!Result_) { return false; }

	Result_ = Text_->SetValue(1, PerformanceManager::Instance()->GetUsage());
	if (!Result_) { return false; }

	Result_ = Text_->SetValue(2, (int)Camera::Instance()->GetTransform()->GetX());
	if (!Result_) { return false; }

	Result_ = Text_->SetValue(3, (int)Camera::Instance()->GetTransform()->GetY());
	if (!Result_) { return false; }

	Result_ = Text_->SetValue(4, (int)Camera::Instance()->GetTransform()->GetZ());
	if (!Result_) { return false; }

	Result_ = Text_->SetValue(5, (int)Camera::Instance()->GetTransform()->GetPitch());
	if (!Result_) { return false; }

	Result_ = Text_->SetValue(6, (int)Camera::Instance()->GetTransform()->GetYaw());
	if (!Result_) { return false; }

	Result_ = Text_->SetValue(7, (int)Camera::Instance()->GetTransform()->GetRoll());
	if (!Result_) { return false; }

	Result_ = Text_->SetValue(8, World_->GetBuildTime());
	if (!Result_) { return false; }

	return true;
}

bool MinecraftScene::HandleInput()
{
	//=================
	// Go Back to Menu
	//=================

	if (InputManager::Instance() -> GetKeyDown(VK_ESCAPE))
	{
		ApplicationManager::Instance()->SetScene(SceneState::MAINMENU);
	}

	//=======================
	// Toggle Wireframe Mode
	//=======================

	if (InputManager::Instance() -> GetKeyDown(VK_9))
	{
		// Switch to the opposite state for wireframe mode
		DirectXManager::Instance() -> ToggleWireframe(!DirectXManager::Instance() -> GetWireframeStatus());
	}

	//=============
	// Toggle Time
	//=============

	if (InputManager::Instance() -> GetKeyDown(VK_0))
	{
		// Switch to the opposite state
		NightTimeMode_ = !NightTimeMode_;

		// Toggle objects
		Light::Instance() -> ToggleTime(NightTimeMode_);
		SkySphere_ -> ToggleTime(NightTimeMode_);
		Ocean_ -> ToggleTime(NightTimeMode_);
		WhiteOut_ = !WhiteOut_;
	}

	return true;
}