#include "Material.h"
#include "AssetManager.h"

Material::Material()
{
	// Add Default Data
	SetColour("BaseColour", WHITE);
	SetFloat("SpecularPower", 32.0f);
}

Material::~Material()
{

}

// Setters
void Material::SetColour(string keyName, Colour value)
{
	ColourDatabase_[keyName] = value;
}

void Material::SetFloat(string keyName, float value)
{
	FloatDatabase_[keyName] = value;
}

bool Material::SetTexture(string keyName, string value)
{
	Texture* newTexture = 0;

	// Load Texture
	AssetManager::Instance()->LoadTexture(&newTexture, value);

	if (!newTexture)
	{
		return false;
	}

	TextureDatabase_[keyName] = newTexture;

	return true;
}

void Material::SetTexture(string keyName, Texture* value)
{
	TextureDatabase_[keyName] = value;
}

void Material::SetVector2(string keyName, D3DXVECTOR2 value)
{
	Vector2Database_[keyName] = value;
}

void Material::SetVector3(string keyName, D3DXVECTOR3 value)
{
	Vector3Database_[keyName] = value;
}

void Material::SetVector4(string keyName, D3DXVECTOR4 value)
{
	Vector4Database_[keyName] = value;
}

// Getters
Colour Material::GetColour(string keyName)
{
	if (ColourDatabase_.count(keyName))
	{
		return ColourDatabase_[keyName];
	}

	return WHITE;
}

float Material::GetFloat(string keyName)
{
	if (FloatDatabase_.count(keyName))
	{
		return FloatDatabase_[keyName];
	}

	return 0;
}

Texture* Material::GetTexture(string keyName)
{
	if (TextureDatabase_.count(keyName))
	{
		return TextureDatabase_[keyName];
	}

	return 0;
}

D3DXVECTOR2 Material::GetVector2(string keyName)
{
	if (Vector2Database_.count(keyName))
	{
		return Vector2Database_[keyName];
	}

	return D3DXVECTOR2(0.0f, 0.0f);
}

D3DXVECTOR3 Material::GetVector3(string keyName)
{
	if (Vector3Database_.count(keyName))
	{
		return Vector3Database_[keyName];
	}

	return D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

D3DXVECTOR4 Material::GetVector4(string keyName)
{
	if (Vector4Database_.count(keyName))
	{
		return Vector4Database_[keyName];
	}

	return D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f);
}