#pragma once

#include <d3dx11.h>
#include <d3dx10math.h>
#include <string>
#include <map>


#include "Texture.h"

class Material
{
public:
	Material();
	Material(const Material& material);
	~Material();

	// terminate
	void terminate();

	// Colours
	void setColour(D3DXVECTOR4 tint);
	void setSpecular(float amount);

	// Textures
	bool setBaseTexture(std::string textureFilename);
	bool setNormalTexture(std::string textureFilename);
	bool setAlphaTexture(std::string textureFilename);
	bool setNoiseTexture(std::string textureFilename);
	bool setDistortionTexture(std::string textureFilename);
	bool setPerturbTexture(std::string textureFilename);

	// getters
	D3DXVECTOR4 getTint();
	float getSpecular();
	ID3D11ShaderResourceView* getBaseTexture();
	ID3D11ShaderResourceView* getNormalTexture();
	ID3D11ShaderResourceView* getAlphaTexture();
	ID3D11ShaderResourceView* getNoiseTexture();
	ID3D11ShaderResourceView* getDistortionTexture();
	ID3D11ShaderResourceView* getPerturbTexture();

	// setters
	void setFloat(string keyName, float value);
	bool setTexture(string keyName, string value);
	void setTexture(string keyName, Texture* value);
	void setVector2(string keyName, D3DXVECTOR2 value);
	void setVector3(string keyName, D3DXVECTOR3 value);
	void setVector4(string keyName, D3DXVECTOR4 value);

	// getters
	float getFloat(string keyName);
	Texture* getTexture(string keyName);
	D3DXVECTOR2 getVector2(string keyName);
	D3DXVECTOR3 getVector3(string keyName);
	D3DXVECTOR4 getVector4(string keyName);

private:
	// Colour
	D3DXVECTOR4 m_tint;
	float m_specularAmount;

	// Textures
	Texture* m_baseTexture;
	Texture* m_normalTexture;
	Texture* m_alphaTexture;
	Texture* m_noiseTexture;
	Texture* m_distortionTexture;
	Texture* m_perturbTexture;

	Texture* m_renderTexture;
	Texture* m_reflectionRenderTexture;
	Texture* m_refractionRenderTexture;

	// Databases
	std::map<string, float> m_floatProperties;
	std::map<string, D3DXVECTOR2> m_vector2Properties;
	std::map<string, D3DXVECTOR3> m_vector3Properties;
	std::map<string, D3DXVECTOR4> m_vector4Properties;
	std::map<string, Texture*> m_textureProperties;
};


