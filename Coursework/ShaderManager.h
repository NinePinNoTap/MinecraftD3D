#pragma once

#include <map>

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
#include "Ocean.h"

#include "GameShader.h"
#include "CloudShader.h"
#include "ColourShader.h"
#include "FireShader.h"
#include "LightShader.h"
#include "SkyShader.h"
#include "TerrainReflectionShader.h"
#include "TerrainShader.h"
#include "TextureShader.h"

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

	// Matrix Setters
	void SetWorldMatrix(D3DXMATRIX world);
	void SetViewMatrix(D3DXMATRIX view);
	void SetProjectionMatrix(D3DXMATRIX projection);
	void SetReflectionViewMatrix(D3DXMATRIX reflection);

	// Matrix Getters
	MatrixBuffer GetMatrixBuffer() { return MatrixBuffer_; }
	
	// Shaders
	//bool CloudRender(Clouds* cloud);
	//bool ColourRender(GameObject* obj);
	//bool FireRender(Fire* fire);
	bool FontRender(Text::SentenceType* sentence, ID3D11ShaderResourceView* texture);
	//bool LightRender(GameObject* model);
	bool OceanRender(Ocean* ocean, Texture* refraction, Texture* reflection);
	//bool SkyRender(GameObject* sky);

	//bool TerrainRender(Terrain* terrain);
	//bool TerrainRender(Terrain* terrain, D3DXVECTOR4 clip);

	bool TextureRender(ParticleSystem* particles);
	
	GameShader* GetShader(string keyName)
	{
		return ShaderDatabase_[keyName];
	}

private:
	bool Result_;

	// Matrices
	MatrixBuffer MatrixBuffer_;
	
	// Shaders
	CloudShader* CloudShader_;
	ColourShader* ColourShader_;
	FireShader* FireShader_;
	GameShader* FontShader_;
	LightShader* LightShader_;
	GameShader* OceanShader_;
	SkyShader* SkySphereShader_;
	TerrainShader* TerrainShader_;
	TerrainReflectionShader* TerrainReflectionShader_;
	TextureShader* TextureShader_;

	// Shader Database
	std::map<string, GameShader*> ShaderDatabase_;
};


