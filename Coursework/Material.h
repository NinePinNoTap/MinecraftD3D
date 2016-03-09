#pragma once

#include <d3dx11.h>
#include <d3dx10math.h>

#include "Colour.h"
#include "Texture.h"
#include "TextureArray.h"

class Material
{
public:
	Material();
	~Material();

	// Shutdown
	void Shutdown();

	// Setters
	void SetColour(Colour tint);
	bool SetBaseTexture(WCHAR* textureFilename);
	bool SetNormalTexture(WCHAR* textureFilename);
	bool SetAlphaTexture(WCHAR* textureFilename);
	bool SetNoiseTexture(WCHAR* textureFilename);
	bool SetDistortionTexture(WCHAR* textureFilename);
	bool SetTextureArray(vector<wstring> textureFilenames);

	// Getters
	Colour GetTint();
	ID3D11ShaderResourceView* GetBaseTexture();
	ID3D11ShaderResourceView* GetNormalTexture();
	ID3D11ShaderResourceView* GetAlphaTexture();
	ID3D11ShaderResourceView* GetNoiseTexture();
	ID3D11ShaderResourceView* GetDistortionTexture();
	ID3D11ShaderResourceView* GetTextureArray();

private:
	// Texture Loader
	Texture* CreateTexture(WCHAR* textureFilename);

	// Base
	Colour Tint_;

	// Textures
	Texture* BaseTexture_;
	Texture* NormalTexture_;
	Texture* AlphaTexture_;
	Texture* NoiseTexture_;
	Texture* DistortionTexture_;
	TextureArray* TextureArray_;
};


