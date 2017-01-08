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

	// initialise
	bool initialise(HWND hwnd);

	// terminate
	void terminate();

	// update
	bool update();
	
	// Scene Change
	void onLoad();
	void onUnload();

private:
	bool handleObjects();
	bool handleText();
	bool handleInput();

	bool render();
	bool renderScene();

	// rendering
	D3DXVECTOR4 m_clipPlane;

	// Objects
	AudioClip* m_ambientSound;
	Clouds* m_clouds;
	Fire* m_fire;
	Model* m_campFire;
	Ocean* Ocean_;
	Sprite* m_cursor;
	Sprite* m_windowSprite;
	SkySphere* m_skySphere;
	Text* m_text;
	Texture* m_renderTexture;
	Texture* m_refractionTexture;
	Texture* m_reflectionTexture;
	Toolbar* m_toolbarInterface;
	VoxelWorld* m_voxelWorld;

	// Flags
	bool m_whiteOut;
	bool m_nightTime;
};