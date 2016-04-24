#pragma once

#include "DirectXManager.h"

#include "Camera.h"
#include "Clouds.h"
#include "Fire.h"
#include "Light.h"
#include "GameObject.h"
#include "InputManager.h"
#include "Texture.h"
#include "ShaderManager.h"
#include "GameScene.h"
#include "AudioClip.h"
#include "Sprite.h"
#include "SkySphere.h"
#include "Text.h"
#include "Ocean.h"
#include "Model.h"
#include "VoxelWorld.h"
#include "Toolbar.h"

using namespace Config;

class MinecraftScene : public GameScene
{
public:
	MinecraftScene();
	~MinecraftScene();

	// Initialise
	bool Initialise(HWND hwnd);

	// Shutdown
	void Shutdown();

	// Frame
	bool Frame();
	
	// Scene Change
	void Load();
	void Unload();

private:
	bool HandleObjects();
	bool HandleText();
	bool HandleInput();

	bool Render();
	bool RenderScene();

	// Rendering
	D3DXVECTOR4 ClipPlane_;

	// Objects
	AudioClip* AmbientSound_;
	Clouds* Clouds_;
	Fire* Fire_;
	Model* CampFire_;
	Ocean* Ocean_;
	Sprite* Cursor_;
	Sprite* WindowSprite_;
	SkySphere* SkySphere_;
	Text* Text_;
	Texture* RenderTexture_;
	Texture* RefractionTexture_;
	Texture* ReflectionTexture_;
	Toolbar* ToolbarUI_;
	VoxelWorld* World_;

	// Flags
	bool IsUnderwater_;
	bool NightTimeMode_;
};