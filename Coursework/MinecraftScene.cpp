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
	ParticleSystem_ = 0;
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

	AssetManager::Instance()->LoadTexture(&RenderTexture_, "RenderTexture", Rect2D(windowWidth, windowHeight));
	AssetManager::Instance()->LoadTexture(&ReflectionTexture_, "ReflectionTexture", Rect2D(windowWidth, windowHeight));
	AssetManager::Instance()->LoadTexture(&RefractionTexture_, "RefractionTexture", Rect2D(windowWidth, windowHeight));

	//=======================
	// Initialise the Clouds
	//=======================

	Clouds_ = new Clouds;
	if (!Clouds_)
	{
		return false;
	}
	Result_ = Clouds_ -> Initialise("cloud.dds", "cloudperturb.dds");
	if (!Result_)
	{
		MessageBox(hwnd, L"Could not initialise the sky plane object.", L"Error", MB_OK);
		return false;
	}

	//======================
	// Initialise the Ocean 
	//======================

	Ocean_ = new Ocean;
	if (!Ocean_)
	{
		return false;
	}
	Result_ = Ocean_->Initialise("water_normal.dds", Rect3D(512.0f, 512.0f, 17.5f));
	if (!Result_)
	{
		MessageBox(hwnd, L"Could not initialise the ocean object.", L"Error", MB_OK);
		return false;
	}
	Ocean_->GetTransform()->SetPosition(256, Ocean_->GetWaterHeight(), 256);

	//================================
	// Initialise the Particle System
	//================================

	ParticleSystem_ = new ParticleSystem;
	if (!ParticleSystem_)
	{
		return false;
	}
	Result_ = ParticleSystem_ -> Initialise("rain.dds");
	if (!Result_)
	{
		MessageBox(hwnd, L"Could not initialise the particle WindowManager.", L"Error", MB_OK);
		return false;
	}

	//=======================
	// Initialise 2D Bitmaps
	//=======================

	ActionBar_ = new Sprite;
	if (!ActionBar_)
	{
		return false;
	}
	Result_ = ActionBar_->Initialise(Rect3D(728, 88));
	if (!Result_)
	{
		MessageBox(hwnd, L"Could not initialise the action bar object.", L"Error", MB_OK);
		return false;
	}
	ActionBar_->GetTransform()->SetY(-(windowHeight * 0.8) / 2);
	ActionBar_->SetTexture("ui_toolbar.dds");
	ActionBar_->SetShader("texture");

	WindowSprite_ = new Sprite;
	if (!WindowSprite_)
	{
		return false;
	}
	Result_ = WindowSprite_->Initialise(Rect3D(windowWidth, windowHeight));
	if (!Result_)
	{
		MessageBox(hwnd, L"Could not initialise the screen window object.", L"Error", MB_OK);
		return false;
	}

	//===========================
	// Initialise the Sky Sphere
	//===========================

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

	AssetManager::Instance()->LoadAudio(&AmbientSound_, "ambience");
	AmbientSound_->SetVolume(1.0f);

	//=================
	// Initialise Text
	//=================

	Text_ = new Text;
	if (!Text_)
	{
		return false;
	}
	Result_ = Text_->Initialise(hwnd, "shruti", "shruti.dds", 95);
	if (!Result_)
	{
		MessageBox(hwnd, L"Could not initialise the text object.", L"Error", MB_OK);
		return false;
	}
	Text_->CreateText("FPS:", Vector2(10, 10), WHITE); // FPS
	Text_->CreateText("CPU:", Vector2(10, 30), BLACK); // CPU
	Text_->CreateText("Camera X :", Vector2(10, 50), BLACK); // CameraX
	Text_->CreateText("Camera Y :", Vector2(10, 70), BLACK); // CameraY
	Text_->CreateText("Camera Z :", Vector2(10, 90), BLACK); // CameraZ
	Text_->CreateText("Rotation X :", Vector2(10, 110), BLACK); // RotationX
	Text_->CreateText("Rotation Y :", Vector2(10, 130), BLACK); // RotationY
	Text_->CreateText("Rotation Z :", Vector2(10, 150), BLACK); // Rotation Z

	Text_->CreateText("CONTROLS", Vector2(windowWidth - 10, 10), BLACK, RIGHT);
	Text_->CreateText("Toggle Wireframe [1]", Vector2(windowWidth - 10, 30), BLACK, RIGHT);
	Text_->CreateText("Toggle Time [2]", Vector2(windowWidth - 10, 50), BLACK, RIGHT);
	Text_->CreateText("Reset Camera [BACKSPACE]", Vector2(windowWidth - 10, 70), BLACK, RIGHT);
	Text_->CreateText("Quit [ESC]", Vector2(windowWidth - 10, 110), BLACK, RIGHT);

	//==============
	// Create World
	//==============

	World_ = new VoxelWorld;
	World_->Initialise();

	//==================
	// Initialise flags
	//==================

	IsUnderwater_ = false;
	NightTimeMode_ = false;
	Result_ = false;
	
	return true;
}

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

	if (ParticleSystem_)
	{
		ParticleSystem_ -> Shutdown();
		delete ParticleSystem_;
		ParticleSystem_ = 0;
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

void MinecraftScene::Reset()
{
	//=========================================
	// Reset objects back to their start state
	//=========================================

	Camera::Instance()->SetStartPosition(0, 100, 0);
	Camera::Instance()->SetStartRotation(-89, 0, 0);
	Camera::Instance()->AllowFlying(true);
	Camera::Instance()->SetSpeed(4.137f);

	Light::Instance()->GetTransform()->SetPosition(-3500.0f, 9900.0f, 2100.0f);
	Light::Instance()->SetAmbientColor(0.5f, 0.5f, 0.5f, 1.0f);
	Light::Instance()->SetDiffuseColor(0.5f, 0.5f, 0.5f, 1.0f);
	Light::Instance()->SetDirection(0.5f, -0.75f, 0.25f);
	Light::Instance()->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);

	InputManager::Instance()->Initialise();
	LockMouseToCenter();

	//============
	// Play Sound
	//============

	AmbientSound_->Play(true);
}

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

bool MinecraftScene::HandleObjects()
{
	//=====================
	// Set Camera Position
	//=====================
	
	ParticleSystem_->GetTransform()->SetPosition(Camera::Instance()->GetTransform()->GetPosition() + (Camera::Instance()->GetTransform()->GetForwardVector() * 1.25));

	//================
	// Update Objects
	//================

	Camera::Instance() -> Frame();
	PerformanceManager::Instance()->Frame();
	ViewFrustumManager::Instance()->Frame();


	Clouds_ -> Frame();
	Ocean_->Frame();
	SkySphere_->Frame();
	World_->Frame();

	if (NightTimeMode_)
	{
		Result_ = ParticleSystem_->Frame();
		if (!Result_)
		{
			return false;
		}
	}
	
	//===========================
	// Update WindowManager Information
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

	return true;
}

bool MinecraftScene::HandleInput()
{
	//====================
	// Escape ApplicationManager
	//====================

	if (InputManager::Instance() -> GetKeyDown(VK_ESCAPE))
	{
		MessageBox(NULL, L"Shutting down the ApplicationManager.", L"Warning", MB_OK);
		return false;
	}

	//=================
	// Scene Switching
	//=================

	if (InputManager::Instance() -> GetKeyDown(VK_HOME))
	{
		ApplicationManager::Instance()->SetScene(GAME);
		AmbientSound_->Stop();
	}

	//==============
	// Reset Camera
	//==============

	if (InputManager::Instance() -> GetKeyDown(VK_BACK))
	{
		Camera::Instance() -> Reset();
	}

	//=======================
	// Toggle Wireframe Mode
	//=======================

	if (InputManager::Instance() -> GetKeyDown(VK_1))
	{
		// Switch to the opposite state for wireframe mode
		DirectXManager::Instance() -> ToggleWireframe(!DirectXManager::Instance() -> GetWireframeStatus());
	}

	//==================
	// Toggle Time Mode
	//==================

	if (InputManager::Instance() -> GetKeyDown(VK_2))
	{
		// Switch to the opposite state
		NightTimeMode_ = !NightTimeMode_;

		// Toggle objects
		Light::Instance() -> ToggleTime(NightTimeMode_);
		SkySphere_ -> ToggleTime(NightTimeMode_);
		Ocean_ -> ToggleTime(NightTimeMode_);
	}

	return true;
}

bool MinecraftScene::Render()
{
	//==============================
	// Render Refraction To Texture
	//==============================

	//Result_ = RenderRefraction();
	//if (!Result_) { return false; }

	////==============================
	//// Render Reflection To Texture
	////==============================

	//Result_ = RenderReflection();
	//if (!Result_) { return false; }

	//==============
	// Render Scene
	//==============

	// Render the scene normally to the screen
	Result_ = RenderScene(!DirectXManager::Instance()->GetWireframeStatus());
	if (!Result_)
	{
		return false;
	}

	return true;
}

bool MinecraftScene::RenderScene(bool ShowText)
{
	// Begin rendering
	DirectXManager::Instance() -> BeginScene();

	//================
	// Render the Sky 
	//================

	// Render the sky dome
	SkySphere_->Render();

	// Render the clouds
	Clouds_->Render();

	//==================
	// Render the Ocean 
	//==================

	/*Result_ = Ocean_->Render();
	if (!Result_)
	{
		return false;
	}*/


	//==================
	// Render the World 
	//==================

	World_->Render();

	//=============
	// Render Rain
	//=============

	// Only rain on the screen if is night time and we arent underwater
	if (NightTimeMode_)
	{
		// Turn on alpha blending
		DirectXManager::Instance() -> SetAlphaBlendingOn(true);

		// Render the particle WindowManager
		Result_ = ShaderManager::Instance()->TextureRender(ParticleSystem_);
		if (!Result_)
		{
			return false;
		}

		// Turn off alpha blending.
		DirectXManager::Instance() -> SetAlphaBlendingOn(false);
	}

	Result_ = RenderText();
	if (!Result_)
	{
		return false;
	}

	// End rendering
	DirectXManager::Instance() -> EndScene();

	return true;
}

bool MinecraftScene::RenderRefraction()
{
	// Render to the refraction texture
	RefractionTexture_ -> SetRenderTarget();
	RefractionTexture_ -> ClearRenderTarget(BLACK);

	//====================
	// Render the Terrain
	//====================

	// Clip everything above the maximum wave height water
	// We use max wave height here otherwise the refraction renders incorrectly
	ClipPlane_ = D3DXVECTOR4(0.0f, -1.0f, 0.0f, Ocean_ -> GetWaterHeight() + (Ocean_ -> GetWaveHeight() * 2));

	// Render the terrain using reflection shader
	//Result_ = ShaderManager::Instance()->TerrainRender(Terrain_, ClipPlane_);
	//if (!Result_) { return false; }

	// Reset rendering back to normal
	DirectXManager::Instance() -> SetBackBufferRenderTarget();
	DirectXManager::Instance() -> ResetViewport();

	return true;
}

bool MinecraftScene::RenderReflection()
{
	D3DXVECTOR3 ReflectedCameraPosition;

	// Render to the reflection texture
	ReflectionTexture_ -> SetRenderTarget();
	ReflectionTexture_ -> ClearRenderTarget(BLACK);

	//================
	// Render the sky
	//================

	// Create a camera reflected in the Y axis
	ReflectedCameraPosition = Camera::Instance()->GetTransform()->GetPosition();
	ReflectedCameraPosition.y = -Camera::Instance()->GetTransform()->GetY() + (Ocean_->GetWaterHeight() * 2.0f);

	// Update Camera Position
	SkySphere_->GetTransform()->SetPosition(ReflectedCameraPosition);
	Clouds_->GetTransform()->SetPosition(ReflectedCameraPosition);
	
	// Render the sky sphere
	SkySphere_->Render();
	if (!Result_)
	{
		return false;
	}
	
	// Render the clouds
	Clouds_->Render();
	if (!Result_)
	{
		return false;
	}
	
	World_->Render();

	//====================
	// Render the Terrain
	//====================

	// Clip everything above the maximum wave height water
	// We use max wave height here otherwise the refraction renders incorrectly
	ClipPlane_ = D3DXVECTOR4(0.0f, 1.0f, 0.0f, -Ocean_ -> GetWaterHeight() + (Ocean_ -> GetWaveHeight() * 2));

	// Reset rendering back to normal
	DirectXManager::Instance() -> SetBackBufferRenderTarget();
	DirectXManager::Instance() -> ResetViewport();

	return true;
}

bool MinecraftScene::RenderToTexture(bool ShowText)
{
	// Render to the scene texture
	RenderTexture_ -> SetRenderTarget();
	RenderTexture_ -> ClearRenderTarget(BLACK);

	// Render the scene
	Result_ = RenderScene(ShowText);
	if (!Result_)
	{
		return false;
	}

	// Reset rendering back to default
	DirectXManager::Instance() -> SetBackBufferRenderTarget();

	return true;
}

bool MinecraftScene::RenderText()
{
	//=======================
	// Render Text to Screen
	//=======================

	// Turn on alpha blending
	DirectXManager::Instance()->SetAlphaBlendingOn(true);

	// Render the text
	Result_ = Text_->Render();
	if (!Result_)
	{
		return false;
	}

	Result_ = ActionBar_->Render();
	if (!Result_)
	{
		return false;
	}

	// Disable blending
	DirectXManager::Instance() -> SetAlphaBlendingOn(false);

	// Re-enable the z buffer
	DirectXManager::Instance() -> SetDepthBufferOn(true);

	return true;
}