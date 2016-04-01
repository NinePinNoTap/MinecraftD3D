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

	// Add Data
	void SetColour(string keyName, Colour value);
	void SetFloat(string keyName, float value);
	bool SetTexture(string keyName, string value);
	void SetTexture(string keyName, Texture* value);
	void SetVector2(string keyName, D3DXVECTOR2 value);
	void SetVector3(string keyName, D3DXVECTOR3 value);
	void SetVector4(string keyName, D3DXVECTOR4 value);

	// Get Data
	Colour GetColour(string keyName);
	float GetFloat(string keyName);
	Texture* GetTexture(string keyName);
	D3DXVECTOR2 GetVector2(string keyName);
	D3DXVECTOR3 GetVector3(string keyName);
	D3DXVECTOR4 GetVector4(string keyName);

private:
	std::map<string, float> FloatDatabase_;
	std::map<string, D3DXVECTOR2> Vector2Database_;
	std::map<string, D3DXVECTOR3> Vector3Database_;
	std::map<string, D3DXVECTOR4> Vector4Database_;
	std::map<string, Texture*> TextureDatabase_;
	std::map<string, Colour> ColourDatabase_;
};


