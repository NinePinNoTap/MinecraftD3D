#pragma once

#include "DirectXManager.h"
#include "Singleton.h"

#include "Block.h"
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

	// Matrix Setters
	void SetWorldMatrix(D3DXMATRIX world);
	void SetViewMatrix(D3DXMATRIX view);
	void SetProjectionMatrix(D3DXMATRIX projection);
	void SetReflectionViewMatrix(D3DXMATRIX reflection);

	// Matrix Getters
	MatrixBuffer GetMatrixBuffer() { return MatrixBuffer_; }
	
	// Shaders
	bool CloudShader(Clouds* cloud);

	bool ColourShader(GameObject* obj);

	bool FireShader(Fire* fire);

	bool FontShader(Text::SentenceType* sentence, ID3D11ShaderResourceView* texture);

	bool LightShader(GameObject* model);

	bool OceanShader(Water* ocean, Texture* refraction, Texture* reflection);

	bool SkyShader(GameObject* sky);

	bool TerrainShader(Terrain* terrain);
	bool TerrainShader(Terrain* terrain, D3DXVECTOR4 clip);

	bool TextureShader(GameObject* model);
	bool TextureShader(ParticleSystem* particles);
	
private:
	bool Result_;

	// Matrices
	MatrixBuffer MatrixBuffer_;
	
	// Shaders
	GameShader* CloudShader_;
	GameShader* ColourShader_;
	GameShader* FireShader_;
	GameShader* FontShader_;
	GameShader* LightShader_;
	GameShader* OceanShader_;
	GameShader* SkySphereShader_;
	GameShader* TerrainShader_;
	GameShader* TerrainReflectionShader_;
	GameShader* TextureShader_;
};


