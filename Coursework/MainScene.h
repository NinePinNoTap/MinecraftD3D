#pragma once

#include "DirectXManager.h"

#include "Camera.h"
#include "Clouds.h"
#include "Fire.h"
#include "Light.h"
#include "GameObject.h"
#include "InputManager.h"
#include "ParticleSystem.h"
#include "Texture.h"
#include "ShaderManager.h"
#include "Scene3D.h"
#include "AudioClip.h"
#include "Sprite.h"
#include "SkySphere.h"
#include "Terrain.h"
#include "Text.h"
#include "Ocean.h"
#include "Model.h"
#include "VoxelTerrain.h"

class MainScene : public Scene3D
{
public:
	MainScene();
	~MainScene();

	bool Initialise(HWND hwnd);
	void Shutdown();

	bool Frame();
	void Reset();

private:
	bool UpdateObjects();
	bool UpdateText();
	bool UserInputManager();

	bool Render();
	bool RenderRefraction();
	bool RenderReflection();
	bool RenderToTexture(bool ShowText = true);
	bool RenderWithPostProcessing();
	bool RenderScene(bool ShowText = true);
	bool RenderText();

	void GenerateMatrices();
	void SetShaderManagerVars();
	void SetShaderManagerReflectionVars();

	// Rendering
	D3DXVECTOR4 ClipPlane_;

	// Objects
	AudioClip* AmbientSound_;
	Clouds* Clouds_;
	Fire* Fire_;
	Model* CampFire_;
	Ocean* Ocean_;
	ParticleSystem* ParticleSystem_;
	Sprite* Sprite_;
	SkySphere* SkySphere_;
	Terrain* Terrain_;
	Texture* RenderTexture_;
	Texture* RefractionTexture_;
	Texture* ReflectionTexture_;
	VoxelTerrain* Chunk_;

	// Flags
	bool IsUnderwater_;
	bool NightTimeMode_;
};