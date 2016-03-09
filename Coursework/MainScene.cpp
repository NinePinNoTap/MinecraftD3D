#include "MainScene.h"
#include "ApplicationManager.h"
#include "PerformanceManager.h"

MainScene::MainScene() : Scene3D()
{
	// Initialise pointers
	Clouds_ = 0;
	Fire_ = 0;
	CampFire_ = 0;
	ParticleSystem_ = 0;
	RefractionTexture_ = 0;
	ReflectionTexture_ = 0;
	Sprite_ = 0;
	SkySphere_ = 0;
	Terrain_ = 0;
	Ocean_ = 0;
}

MainScene::~MainScene()
{
}

bool MainScene::Initialise(HWND hwnd, Rect2D WindowResolution)
{
	//=======================
	// Initialise the Camera
	//=======================

	Camera::Instance()->Get2DViewMatrix(BaseViewMatrix_);
	Camera::Instance()->SetStartPosition(447, 24, 163);
	Camera::Instance()->SetStartRotation(6, 28, 0);
	Camera::Instance()->AllowFlying(true);
	Camera::Instance()->SetSpeed(0.5f);

	//=======================
	// Initialise the Clouds
	//=======================

	Clouds_ = new Clouds;
	if (!Clouds_) { return false; }
	Result_ = Clouds_ -> Initialise(L"Data/Textures/Cloud.dds", L"Data/Textures/CloudPerturb.dds");
	if (!Result_)
	{
		MessageBox(hwnd, L"Could not initialise the sky plane object.", L"Error", MB_OK);
		return false;
	}

	//======================
	// Initialise the Light
	//======================

	Light::Instance()->GetTransform()->SetPosition(-3500.0f, 9900.0f, 2100.0f);
	Light::Instance()->SetAmbientColor(0.5f, 0.5f, 0.5f, 1.0f);
	Light::Instance()->SetDiffuseColor(0.5f, 0.5f, 0.5f, 1.0f);
	Light::Instance()->SetDirection(0.5f, -0.75f, 0.25f);
	Light::Instance()->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	Light::Instance()->SetSpecularPower(32.0f);

	//======================
	// Initialise the Ocean 
	//======================

	Ocean_ = new Water;
	if (!Ocean_) { return false; }
	Result_ = Ocean_->Initialise(L"Data/Textures/WaterNormal.dds", Rect3D(512.0f, 512.0f, 17.5f));
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
	if (!ParticleSystem_) { return false; }
	Result_ = ParticleSystem_ -> Initialise(L"Data/Textures/rain.dds");
	if (!Result_)
	{
		MessageBox(hwnd, L"Could not initialise the particle WindowManager.", L"Error", MB_OK);
		return false;
	}

	//===================================
	// Initialise the Reflection Texture
	//===================================

	ReflectionTexture_ = new Texture;
	if (!ReflectionTexture_) { return false; }
	Result_ = ReflectionTexture_->Initialise(WindowResolution);
	if (!Result_)
	{
		MessageBox(hwnd, L"Could not initialise the reflection render to texture object.", L"Error", MB_OK);
		return false;
	}

	//===================================
	// Initialise the Refraction Texture
	//===================================

	RefractionTexture_ = new Texture;
	if (!RefractionTexture_) { return false; }
	Result_ = RefractionTexture_->Initialise(WindowResolution);
	if (!Result_)
	{
		MessageBox(hwnd, L"Could not initialise the refraction render to texture object.", L"Error", MB_OK);
		return false;
	}

	//===============================
	// Initialise the Scene Textures
	//===============================

	SceneTexture_ = new Texture;
	if (!SceneTexture_) { return false; }
	Result_ = SceneTexture_->Initialise(WindowResolution);
	if (!Result_)
	{
		MessageBox(hwnd, L"Could not initialise the scene texture object.", L"Error", MB_OK);
		return false;
	}

	Sprite_ = new Sprite;
	if (!Sprite_) { return false; }
	Result_ = Sprite_->Initialise(Rect3D(WindowResolution.width, WindowResolution.height));
	if (!Result_)
	{
		MessageBox(hwnd, L"Could not initialise the screen window object.", L"Error", MB_OK);
		return false;
	}

	//===========================
	// Initialise the Sky Sphere
	//===========================

	SkySphere_ = new SkySphere;
	if (!SkySphere_) { return false; }
	SkySphere_ -> Initialise("Data/Models/Sphere.txt");
	if (!Result_)
	{
		MessageBox(hwnd, L"Could not initialise the sky sphere object.", L"Error", MB_OK);
		return false;
	}

	//===================
	// Initialise Sounds
	//===================

	AmbientSound_ = new AudioClip;
	AmbientSound_->LoadFile("Data/Sounds/water.wav", false);

	//========================
	// Initialise the Terrain
	//========================

	Terrain_ = new Terrain;
	if (!Terrain_) { return false; }
	Result_ = Terrain_->Initialise(Rect3D(256, 256), TerrainType::PERLIN, L"Data/Textures/sand.dds", L"Data/Textures/sand_normal.dds", Vector2(50, 50), 1.0f);
	if (!Result_)
	{
		MessageBox(hwnd, L"Could not initialise the terrain object.", L"Error", MB_OK);
		return false;
	}
	Terrain_->GetTransform()->SetY(50);

	//=================
	// Initialise Text
	//=================

	Text_ = new Text;
	if (!Text_) { return false; }
	Result_ = Text_->Initialise(hwnd, WindowResolution);
	if (!Result_)
	{
		MessageBox(hwnd, L"Could not initialise the text object.", L"Error", MB_OK);
		return false;
	}
	Text_->CreateText("", Vector2(10, 10), YELLOW); // FPS
	Text_->CreateText("", Vector2(10, 30), YELLOW); // CPU
	Text_->CreateText("", Vector2(10, 50), YELLOW); // CameraX
	Text_->CreateText("", Vector2(10, 70), YELLOW); // CameraY
	Text_->CreateText("", Vector2(10, 90), YELLOW); // CameraZ
	Text_->CreateText("", Vector2(10, 110), YELLOW); // RotationX
	Text_->CreateText("", Vector2(10, 130), YELLOW); // RotationY
	Text_->CreateText("", Vector2(10, 150), YELLOW); // Rotation Z

	Text_->CreateText("CONTROLS", Vector2(WindowResolution.width - 10, 10), WHITE, RIGHT);
	Text_->CreateText("[1] - Toggle Wireframe", Vector2(WindowResolution.width - 10, 30), WHITE, RIGHT);
	Text_->CreateText("[2] - Toggle Time", Vector2(WindowResolution.width - 10, 50), WHITE, RIGHT);
	Text_->CreateText("[BACKSPACE] - Reset Camera", Vector2(WindowResolution.width - 10, 70), WHITE, RIGHT);
	Text_->CreateText("[ESC] - Quit", Vector2(WindowResolution.width - 10, 110), WHITE, RIGHT);

	//==================
	// Initialise flags
	//==================

	IsUnderwater_ = false;
	NightTimeMode_ = false;
	Result_ = false;

	return true;
}

void MainScene::Shutdown()
{
	//===================
	// Shut down objects
	//===================

	ShutdownGameObjects();

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

	if (RefractionTexture_)
	{
		RefractionTexture_ -> Shutdown();
		delete RefractionTexture_;
		RefractionTexture_ = 0;
	}

	if (ReflectionTexture_)
	{
		ReflectionTexture_ -> Shutdown();
		delete ReflectionTexture_;
		ReflectionTexture_ = 0;
	}

	if (SceneTexture_)
	{
		SceneTexture_ -> Shutdown();
		delete SceneTexture_;
		SceneTexture_ = 0;
	}

	if (Sprite_)
	{
		Sprite_ -> Shutdown();
		delete Sprite_;
		Sprite_ = 0;
	}

	if (SkySphere_)
	{
		SkySphere_ -> Shutdown();
		delete SkySphere_;
		SkySphere_ = 0;
	}

	if (Terrain_)
	{
		Terrain_ -> Shutdown();
		delete Terrain_;
		Terrain_ = 0;
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

void MainScene::Reset()
{
	//=========================================
	// Reset objects back to their start state
	//=========================================

	Camera::Instance()->Get2DViewMatrix(BaseViewMatrix_);
	Camera::Instance()->SetStartPosition(447, 24, 163);
	Camera::Instance()->SetStartRotation(6, 28, 0);
	Camera::Instance()->AllowFlying(true);
	Camera::Instance()->SetSpeed(0.5f);

	Light::Instance()->GetTransform()->SetPosition(-3500.0f, 9900.0f, 2100.0f);
	Light::Instance()->SetAmbientColor(0.5f, 0.5f, 0.5f, 1.0f);
	Light::Instance()->SetDiffuseColor(0.5f, 0.5f, 0.5f, 1.0f);
	Light::Instance()->SetDirection(0.5f, -0.75f, 0.25f);
	Light::Instance()->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	Light::Instance()->SetSpecularPower(32.0f);

	InputManager::Instance()->Initialise();
	LockMouseToCenter();

	//============
	// Play Sound
	//============

	AmbientSound_->Play(true);
}

bool MainScene::Frame()
{
	// Handle user InputManager
	Result_ = UserInputManager();
	if (!Result_) { return false; }

	// Handle frame updates
	Result_ = UpdateObjects();
	if (!Result_) { return false; }

	// Render the scene
	Result_ = Render();
	if (!Result_) { return false; }

	return true;
}

bool MainScene::UpdateObjects()
{
	//=====================
	// Set Camera Position
	//=====================
	
	//ParticleSystem_->GetTransform()->SetPosition(Camera::Instance()->GetTransform()->GetPosition() + (Camera::Instance()->GetTransform()->GetForwardVector() * 1.25));

	//================
	// Update Objects
	//================

	Camera::Instance() -> Frame();
	Clouds_ -> Frame();
	Ocean_ -> Frame();

	//Result_ = ParticleSystem_ -> Frame(PerformanceManager::Instance()->GetTime());
	//if (!Result_) { return false; }

	//===========================
	// Update WindowManager Information
	//===========================

	Result_ = UpdateText();
	if (!Result_) { return false; }

	//=========================
	// Check Underwater Status
	//=========================

/*	if (Camera::Instance()->GetTransform()->GetY() < Ocean_->GetWaterHeight())
	{
		IsUnderwater_ = true;
	}
	else
	{
		IsUnderwater_ = false;
	}*/

	return true;
}

bool MainScene::UpdateText()
{
	//===========================
	// Update WindowManager Information
	//===========================

	Result_ = Text_->SetText(0, "FPS : ", PerformanceManager::Instance()->GetFPS());
	if (!Result_) { return false; }
	Result_ = Text_->SetText(1, "CPU : ", PerformanceManager::Instance()->GetUsage());
	if (!Result_) { return false; }
	Result_ = Text_->SetText(2, "Camera X : ", (int)Camera::Instance()->GetTransform()->GetX());
	if (!Result_) { return false; }
	Result_ = Text_->SetText(3, "Camera Y : ", (int)Camera::Instance()->GetTransform()->GetY());
	if (!Result_) { return false; }
	Result_ = Text_->SetText(4, "Camera Z : ", (int)Camera::Instance()->GetTransform()->GetZ());
	if (!Result_) { return false; }
	Result_ = Text_->SetText(5, "Rotation X : ", (int)Camera::Instance()->GetTransform()->GetPitch());
	if (!Result_) { return false; }
	Result_ = Text_->SetText(6, "Rotation Y : ", (int)Camera::Instance()->GetTransform()->GetYaw());
	if (!Result_) { return false; }
	Result_ = Text_->SetText(7, "Rotation Z : ", (int)Camera::Instance()->GetTransform()->GetRoll());
	if (!Result_) { return false; }

	return true;
}

bool MainScene::UserInputManager()
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
		ApplicationManager::Instance()->SetScene(MAIN);
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
		Fire_ -> SetActive(NightTimeMode_);
	}

	return true;
}

bool MainScene::Render()
{
	//==============================
	// Render Refraction To Texture
	//==============================

	Result_ = RenderRefraction();
	if (!Result_) { return false; }

	//==============================
	// Render Reflection To Texture
	//==============================

	Result_ = RenderReflection();
	if (!Result_) { return false; }

	//=============================
	// Render Refraction To Screen
	//=============================

	if (IsUnderwater_)
	{
		// Render with post processing to the screen
		Result_ = RenderWithPostProcessing();
		if (!Result_) { return false; }
	}
	else
	{
		bool WireframeStatus = DirectXManager::Instance() -> GetWireframeStatus();

		// Render the scene normally to the screen
		Result_ = RenderScene(!WireframeStatus);
		if (!Result_) { return false; }
	}

	return true;
}

bool MainScene::RenderScene(bool ShowText)
{
	// Begin rendering
	DirectXManager::Instance() -> BeginScene();

	//==============================================
	// Generate Matrices and Send To Shader Manager
	//==============================================

	GenerateMatrices();
	SetShaderManagerVars();

	//================
	// Render the Sky 
	//================

	// Set the position of the sky and clouds to the camera
	Clouds_->GetTransform()->SetPosition(Camera::Instance()->GetTransform()->GetPosition());
	SkySphere_->GetTransform()->SetPosition(Camera::Instance()->GetTransform()->GetPosition());

	// Disable culling and the z buffer
	DirectXManager::Instance() -> ToggleCulling(false);
	DirectXManager::Instance() -> ToggleZBuffer(false);

	// Render the sky dome
	ShaderManager::Instance() -> SkyRender(SkySphere_);

	// Turn back face culling on
	DirectXManager::Instance() -> ToggleCulling(true);

	// Enable blending so the clouds can be blended with the sky
	DirectXManager::Instance() -> EnableSecondBlendState();

	// Render the clouds
	ShaderManager::Instance() -> CloudRender(Clouds_);

	// Enable culling and the z buffer
	DirectXManager::Instance() -> ToggleAlphaBlending(false);
	DirectXManager::Instance() -> ToggleZBuffer(true);

	//==============================================
	// Generate Matrices and Send To Shader Manager
	//==============================================

	GenerateMatrices();
	SetShaderManagerVars();

	//====================
	// Render the Terrain
	//====================

	Result_ = ShaderManager::Instance()->TerrainRender(Terrain_);
	if (!Result_) { return false; }

	//==================
	// Render the Ocean 
	//==================

	Result_ = ShaderManager::Instance()->WaterRender(Ocean_, RefractionTexture_, ReflectionTexture_);
	if (!Result_) { return false; }

	//===================
	// Render the Models 
	//===================

	// Turn off back face culling
	DirectXManager::Instance() -> ToggleCulling(false);

	// Render Static Models
	for each (GameObject* obj in Objects_)
	{
		if (obj->IsActive())
		{
			Result_ = ShaderManager::Instance()->LightRender(obj, MODEL_SPECULAR);
			if (!Result_)
			{
				return false;
			}
		}
	}

	//==============================================
	// Generate Matrices and Send To Shader Manager
	//==============================================

	GenerateMatrices();

	SetShaderManagerVars();

	//=============
	// Render Rain
	//=============

	// Only rain on the screen if is night time and we arent underwater
	if (NightTimeMode_ && !IsUnderwater_)
	{
		// Turn on alpha blending
		DirectXManager::Instance() -> ToggleAlphaBlending(true);

		// Render the particle WindowManager
		Result_ = ShaderManager::Instance()->ParticleRender(ParticleSystem_);
		if (!Result_) { return false; }

		// Turn off alpha blending.
		DirectXManager::Instance() -> ToggleAlphaBlending(false);
	}

	// Check if we are underwater, if not don't render text
	if (!IsUnderwater_ && ShowText)
	{
		Result_ = RenderText();
		if (!Result_) { return false; }
	}

	// End rendering
	DirectXManager::Instance() -> EndScene();

	return true;
}

bool MainScene::RenderWithPostProcessing()
{
	// Render the scene to the scene texture
	Result_ = RenderToTexture();
	if (!Result_) { return false; }

	// Begin rendering
	DirectXManager::Instance() -> BeginScene();

	// Turn off the Z buffer
	DirectXManager::Instance() -> ToggleZBuffer(false);

	//==============================================
	// Generate Matrices and Send To Shader Manager
	//==============================================

	GenerateMatrices();

	SetShaderManager2DVars();

	//===================================
	// Render Scene with Post Processing
	//===================================

	// Check if wireframe mode is active
	bool WireframeStatus = DirectXManager::Instance() -> GetWireframeStatus();
	if (WireframeStatus)
	{
		// Temporarily disable it so can render wireframe mode underwater
		DirectXManager::Instance() -> ToggleWireframe(false);
	}

	// Reset wireframe status back to previous setting
	DirectXManager::Instance() -> ToggleWireframe(WireframeStatus);

	// Render the screens text
	Result_ = RenderText();
	if (!Result_) { return false; }

	// Stop rendering
	DirectXManager::Instance() -> EndScene();

	return true;
}

bool MainScene::RenderRefraction()
{
	// Render to the refraction texture
	RefractionTexture_ -> SetRenderTarget();
	RefractionTexture_ -> ClearRenderTarget(BLACK);

	//==============================================
	// Generate Matrices and Send To Shader Manager
	//==============================================

	GenerateMatrices();
	SetShaderManagerVars();

	//====================
	// Render the Terrain
	//====================

	// Clip everything above the maximum wave height water
	// We use max wave height here otherwise the refraction renders incorrectly
	ClipPlane_ = D3DXVECTOR4(0.0f, -1.0f, 0.0f, Ocean_ -> GetWaterHeight() + (Ocean_ -> GetWaveHeight() * 2));

	// Render the terrain using reflection shader
	//Result_ = ShaderManager::Instance()->TerrainRender(Terrain_, ClipPlane_);
	if (!Result_) { return false; }

	// Reset rendering back to normal
	DirectXManager::Instance() -> SetBackBufferRenderTarget();
	DirectXManager::Instance() -> ResetViewport();

	return true;
}

bool MainScene::RenderReflection()
{
	D3DXVECTOR3 ReflectedCameraPosition;

	// Render to the reflection texture
	ReflectionTexture_ -> SetRenderTarget();
	ReflectionTexture_ -> ClearRenderTarget(BLACK);

	//===================
	// Generate Matrices
	//===================

	GenerateMatrices();
	SetShaderManagerReflectionVars();

	//================
	// Render the sky
	//================

	// Create a camera reflected in the Y axis
	ReflectedCameraPosition = Camera::Instance()->GetTransform()->GetPosition();
	ReflectedCameraPosition.y = -Camera::Instance()->GetTransform()->GetY() + (Ocean_->GetWaterHeight() * 2.0f);

	// Update Camera Position
	SkySphere_->GetTransform()->SetPosition(ReflectedCameraPosition);
	Clouds_->GetTransform()->SetPosition(ReflectedCameraPosition);

	// Disable back face culling and z buffer
	DirectXManager::Instance() -> ToggleCulling(false);
	DirectXManager::Instance() -> ToggleZBuffer(false);

	// Render the sky sphere
	Result_ = ShaderManager::Instance()->SkyRender(SkySphere_);
	if (!Result_) { return false; }

	// Re-enable culling
	DirectXManager::Instance() -> ToggleCulling(true);

	// Enable blending so the clouds can be blended with the sky
	DirectXManager::Instance() -> EnableSecondBlendState();

	// Render the clouds
	Result_ = ShaderManager::Instance()->CloudRender(Clouds_);
	if (!Result_) { return false; }

	// Disable blending and re-enable the z buffer
	DirectXManager::Instance() -> ToggleAlphaBlending(false);
	DirectXManager::Instance() -> ToggleZBuffer(true);

	//==============================================
	// Generate Matrices and Send To Shader Manager
	//==============================================

	GenerateMatrices();
	SetShaderManagerReflectionVars();

	//===================
	// Render the Models
	//===================

	// Turn off culling
	DirectXManager::Instance() -> ToggleCulling(false);

	// Render Models
	for each (GameObject* obj in Objects_)
	{
		if (obj->IsReflectable())
		{
			Result_ = ShaderManager::Instance()->LightRender(obj, MODEL_SPECULAR);
			if (!Result_)
			{
				return false;
			}
		}
	}

	//====================
	// Render the Terrain
	//====================

	// Clip everything above the maximum wave height water
	// We use max wave height here otherwise the refraction renders incorrectly
	ClipPlane_ = D3DXVECTOR4(0.0f, 1.0f, 0.0f, -Ocean_ -> GetWaterHeight() + (Ocean_ -> GetWaveHeight() * 2));

	//Result_ = ShaderManager::Instance()->TerrainRender(Terrain_, ClipPlane_);
	if (!Result_) { return false; }

	// Re-enable culling
	DirectXManager::Instance() -> ToggleCulling(false);

	// Reset rendering back to normal
	DirectXManager::Instance() -> SetBackBufferRenderTarget();
	DirectXManager::Instance() -> ResetViewport();

	return true;
}

bool MainScene::RenderToTexture(bool ShowText)
{
	// Render to the scene texture
	SceneTexture_ -> SetRenderTarget();
	SceneTexture_ -> ClearRenderTarget(BLACK);

	// Render the scene
	Result_ = RenderScene(ShowText);
	if (!Result_) { return false; }

	// Reset rendering back to default
	DirectXManager::Instance() -> SetBackBufferRenderTarget();

	return true;
}

bool MainScene::RenderText()
{
	//==============================================
	// Generate Matrices and Send To Shader Manager
	//==============================================

	GenerateMatrices();

	SetShaderManager2DVars();

	//=======================
	// Render Text to Screen
	//=======================

	// Turn on alpha blending
	DirectXManager::Instance() -> ToggleAlphaBlending(true);

	// Render the text
	Result_ = Text_ -> Render();
	if (!Result_) { return false; }

	// Disable blending
	DirectXManager::Instance() -> ToggleAlphaBlending(false);

	// Re-enable the z buffer
	DirectXManager::Instance() -> ToggleZBuffer(true);

	return true;
}

void MainScene::GenerateMatrices()
{
	//===============================================
	// Generate World/Ortho/Projection/View Matrices
	//===============================================

	Camera::Instance() -> Render();
	Camera::Instance() -> RenderReflection(Ocean_ -> GetWaterHeight());
	Camera::Instance() -> GetViewMatrix(ViewMatrix_);
	Camera::Instance() -> Get2DViewMatrix(BaseViewMatrix_);
	Camera::Instance() -> GetReflectionMatrix(ReflectionViewMatrix_);

	DirectXManager::Instance() -> GetWorldMatrix(WorldMatrix_);
	DirectXManager::Instance() -> GetProjectionMatrix(ProjectionMatrix_);
	DirectXManager::Instance() -> GetOrthoMatrix(OrthoMatrix_);
}

void MainScene::SetShaderManagerVars()
{
	//==================================================
	// Send Normal Rendering Matrices to Shader Manager
	//==================================================

	ShaderManager::Instance()->SetWorldMatrix(WorldMatrix_);
	ShaderManager::Instance()->SetProjectionMatrix(ProjectionMatrix_);
	ShaderManager::Instance()->SetViewMatrix(ViewMatrix_);
	ShaderManager::Instance()->SetReflectionViewMatrix(ReflectionViewMatrix_);
}

void MainScene::SetShaderManagerReflectionVars()
{
	//======================================================
	// Send Reflection Rendering Matrices to Shader Manager
	//======================================================

	ShaderManager::Instance()->SetWorldMatrix(WorldMatrix_);
	ShaderManager::Instance()->SetProjectionMatrix(ProjectionMatrix_);
	ShaderManager::Instance()->SetViewMatrix(ReflectionViewMatrix_);
	ShaderManager::Instance()->SetReflectionViewMatrix(ReflectionViewMatrix_);
}

Texture* MainScene::GetSceneRender()
{
	return SceneTexture_;
}