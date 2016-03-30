#pragma once

#include <d3dx11.h>
#include <d3dx10math.h>
#include <string>

#include "Colour.h"
#include "Constants.h"
#include "Texture.h"

class Material
{
public:
	Material();
	~Material();

	// Shutdown
	void Shutdown();

	// Colours
	void SetColour(Colour tint);
	void SetSpecular(float amount);

	// Textures
	bool SetBaseTexture(std::string textureFilename);
	bool SetNormalTexture(std::string textureFilename);
	bool SetAlphaTexture(std::string textureFilename);
	bool SetNoiseTexture(std::string textureFilename);
	bool SetDistortionTexture(std::string textureFilename);
	bool SetPerturbTexture(std::string textureFilename);

	// Getters
	Colour GetTint();
	float GetSpecular();
	ID3D11ShaderResourceView* GetBaseTexture();
	ID3D11ShaderResourceView* GetNormalTexture();
	ID3D11ShaderResourceView* GetAlphaTexture();
	ID3D11ShaderResourceView* GetNoiseTexture();
	ID3D11ShaderResourceView* GetDistortionTexture();
	ID3D11ShaderResourceView* GetPerturbTexture();

private:
	// Colour
	Colour Tint_;
	float SpecularAmount_;

	// Textures
	Texture* BaseTexture_;
	Texture* NormalTexture_;
	Texture* AlphaTexture_;
	Texture* NoiseTexture_;
	Texture* DistortionTexture_;
	Texture* PerturbTexture_;
};


