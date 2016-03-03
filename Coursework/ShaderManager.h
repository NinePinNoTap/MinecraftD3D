#pragma once

#include "DirectXManager.h"
#include "Singleton.h"

#include "Camera.h"
#include "Clouds.h"
#include "Fire.h"
#include "Light.h"
#include "GameObject.h"
#include "ParticleSystem.h"
#include "Texture.h"
#include "Sprite.h"
#include "SkySphere.h"
#include "Terrain.h"
#include "Text.h"
#include "Water.h"

#include "GameShader.h"

class ShaderManager : public Singleton<ShaderManager>
{
public:
	ShaderManager();
	ShaderManager(const ShaderManager&);
	~ShaderManager();

	// Initialising
	bool Initialise(HWND);

	// Shutdown
	void Shutdown();

	// Object Setters
	void SetCamera(Camera* camera);
	void SetLight(Light* light);

	// Matrix Setters
	void SetWorldMatrix(D3DXMATRIX world);
	void SetViewMatrix(D3DXMATRIX view);
	void SetProjectionMatrix(D3DXMATRIX projection);
	void SetReflectionViewMatrix(D3DXMATRIX reflection);

	// Render Using Cloud Shader
	bool CloudRender(Clouds* cloud);

	// Render Using Fire Shader
	bool FireRender(Fire* fire);

	// Render Using Font Shader
	bool FontRender(Text::SentenceType* sentence, ID3D11ShaderResourceView* texture);

	// Render Using Light Shader
	bool LightRender(GameObject* model, float Specular);

	// Render Using Water Shader
	bool WaterRender(Water* ocean, Texture* refraction, Texture* reflection);

	// Render Using Particle Shader
	bool ParticleRender(ParticleSystem* particles);

	// Render Using Sky Shader
	bool SkyRender(SkySphere* sky);

	// Render Using Terrain Shader
	bool TerrainRender(Terrain* terrain);

	// Render Using Terrain Reflection Shader
	bool TerrainRender(Terrain* terrain, D3DXVECTOR4 clip);

	// Render Using Texture Shader
	bool TextureRender(GameObject* model);
	bool TextureRender(Sprite* window, Texture* render);

private:
	bool Result_;

	// Game Objects
	Camera* Camera_;
	Light* Light_;

	// Matrices
	MatrixCBuffer MatrixBuffer_;
	
	// Shaders
	GameShader* CloudShader_;
	GameShader* FireShader_;
	GameShader* FontShader_;
	GameShader* LightShader_;
	GameShader* OceanShader_;
	GameShader* ParticleShader_;
	GameShader* SkySphereShader_;
	GameShader* TerrainShader_;
	GameShader* TerrainReflectionShader_;
	GameShader* TextureShader_;
};


