#pragma once

#include <map>

#include "DirectXManager.h"
#include "Singleton.h"
#include "ParticleSystem.h"
#include "Camera.h"
#include "Clouds.h"
#include "Fire.h"
#include "Light.h"
#include "GameObject.h"
#include "Texture.h"
#include "Sprite.h"
#include "SkySphere.h"
#include "Terrain.h"
#include "Text.h"
#include "Ocean.h"

#include "GameShader.h"
#include "CloudShader.h"
#include "ColourShader.h"
#include "FireShader.h"
#include "FontShader.h"
#include "InstancedLightShader.h"
#include "LightShader.h"
#include "OceanShader.h"
#include "SkyShader.h"
#include "TerrainReflectionShader.h"
#include "TerrainShader.h"
#include "TextureShader.h"
#include "TintShader.h"

class ShaderManager : public Singleton<ShaderManager>
{
public:
	ShaderManager();
	ShaderManager(const ShaderManager&);
	~ShaderManager();

	// Initialising
	bool Initialise(HWND hwnd);

	// Shutdown
	void Shutdown();
	
	// Shaders
	bool TextureRender(ParticleSystem* particles);
	
	GameShader* GetShader(string keyName)
	{
		return ShaderDatabase_[keyName];
	}

private:
	bool Result_;
	
	// Shaders
	CloudShader* CloudShader_;
	ColourShader* ColourShader_;
	FireShader* FireShader_;
	FontShader* FontShader_;
	InstancedLightShader* InstancedLightShader_;
	LightShader* LightShader_;
	OceanShader* OceanShader_;
	SkyShader* SkySphereShader_;
	TerrainShader* TerrainShader_;
	TerrainReflectionShader* TerrainReflectionShader_;
	TextureShader* TextureShader_;
	TintShader* TintShader_;

	// Shader Database
	std::map<string, GameShader*> ShaderDatabase_;
};


