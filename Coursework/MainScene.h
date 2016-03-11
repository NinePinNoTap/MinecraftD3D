#pragma once

#include "DirectXManager.h"
#include "AssetManager.h"
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
#include "Water.h"
#include "Model.h"

class MainScene : public Scene3D
{
public:
	MainScene();
	~MainScene();

	bool Initialise(HWND hwnd, Rect2D WindowResolution);
	void Shutdown();

	bool Frame();
	void Reset();

	Texture* GetSceneRender();

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

	// Misc
	Clouds* Clouds_;
	Fire* Fire_;
	Model* CampFire_;
	ParticleSystem* ParticleSystem_;
	Texture* RefractionTexture_;
	Texture* ReflectionTexture_;
	Sprite* Sprite_;
	SkySphere* SkySphere_;
	Terrain* Terrain_;
	Water* Ocean_;
	AudioClip* AmbientSound_;
	AudioClip* AmbientSound2_;

	// Flags
	bool IsUnderwater_;
	bool NightTimeMode_;
};


