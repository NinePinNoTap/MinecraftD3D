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
#include "InstancedTextureShader.h"
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
	bool initialise(HWND hwnd);

	// terminate
	void terminate();
	
	// Shaders
	bool ParticleRender(ParticleSystem* particles);
	
	inline GameShader* getShader(string keyName)
	{
		return m_shaders[keyName];
	}

private:
	bool m_result;
	
	// Shaders
	CloudShader* m_cloudShader;
	ColourShader* m_colourShader;
	FireShader* m_fireShader;
	FontShader* m_fontShader;
	InstancedLightShader* m_instancedLightShader;
	InstancedTextureShader* m_instancedTextureShader;
	LightShader* m_lightShader;
	OceanShader* m_oceanShader;
	SkyShader* m_skySphereShader;
	TerrainShader* m_terrainShader;
	TerrainReflectionShader* m_terrainReflectionShader;
	TextureShader* m_textureShader;
	TintShader* m_TintShader;

	// Shader Database
	std::map<string, GameShader*> m_shaders;
};


