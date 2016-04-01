#pragma once

#include <d3dx11.h>
#include <d3dx10math.h>
#include <string>
#include <map>

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


	// Setters
	void SetFloat(string keyName, float value);
	bool SetTexture(string keyName, string value);
	void SetTexture(string keyName, Texture* value);
	void SetVector2(string keyName, D3DXVECTOR2 value);
	void SetVector3(string keyName, D3DXVECTOR3 value);
	void SetVector4(string keyName, D3DXVECTOR4 value);

	// Getters
	float GetFloat(string keyName);
	Texture* GetTexture(string keyName);
	D3DXVECTOR2 GetVector2(string keyName);
	D3DXVECTOR3 GetVector3(string keyName);
	D3DXVECTOR4 GetVector4(string keyName);

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

	// Databases
	std::map<string, float> FloatDatabase_;
	std::map<string, D3DXVECTOR2> Vector2Database_;
	std::map<string, D3DXVECTOR3> Vector3Database_;
	std::map<string, D3DXVECTOR4> Vector4Database_;
	std::map<string, Texture*> TextureDatabase_;
	std::map<string, Colour> ColourDatabase_;
};


