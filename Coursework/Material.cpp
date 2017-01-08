#include "Material.h"
#include "AssetManager.h"

Material::Material()
{
	// initialise pointers
	m_baseTexture = 0;
	m_normalTexture = 0;
	m_alphaTexture = 0;
	m_noiseTexture = 0;
	m_distortionTexture = 0;
	m_perturbTexture = 0;

	// initialise colours
	m_tint = Colour::White;
	m_specularAmount = 32.0f;

	// Add Default Data
	setVector4("BaseColour", D3DXVECTOR4(1,1,1,1));
}

Material::Material(const Material& material)
{
	*this = material; // copy the object
}

Material::~Material()
{

}

// terminate
void Material::terminate()
{
	
}

// setters
void Material::setColour(D3DXVECTOR4 tint)
{
	m_tint = tint;
}

void Material::setSpecular(float amount)
{
	m_specularAmount = amount;
}

bool Material::setBaseTexture(std::string textureFilename)
{
	// create the texture
	AssetManager::getInstance()->loadTexture(&m_baseTexture, textureFilename);

	if (!m_baseTexture)
	{
		return false;
	}

	return true;
}

bool Material::setNormalTexture(std::string textureFilename)
{
	// create the texture
	AssetManager::getInstance()->loadTexture(&m_normalTexture, textureFilename);

	if (!m_normalTexture)
	{
		return false;
	}

	return true;
}

bool Material::setAlphaTexture(std::string textureFilename)
{
	// create the texture
	AssetManager::getInstance()->loadTexture(&m_alphaTexture, textureFilename);

	if (!m_alphaTexture)
	{
		return false;
	}

	return true;
}

bool Material::setNoiseTexture(std::string textureFilename)
{
	// create the texture
	AssetManager::getInstance()->loadTexture(&m_noiseTexture, textureFilename);

	if (!m_noiseTexture)
	{
		return false;
	}

	return true;
}

bool Material::setDistortionTexture(std::string textureFilename)
{
	// create the texture
	AssetManager::getInstance()->loadTexture(&m_distortionTexture, textureFilename);

	if (!m_distortionTexture)
	{
		return false;
	}

	return true;
}

bool Material::setPerturbTexture(std::string textureFilename)
{
	// create the texture
	AssetManager::getInstance()->loadTexture(&m_perturbTexture, textureFilename);

	if (!m_perturbTexture)
	{
		return false;
	}

	return true;
}

// getters
D3DXVECTOR4 Material::getTint()
{
	return m_tint;
}

float Material::getSpecular()
{
	return m_specularAmount;
}

ID3D11ShaderResourceView* Material::getBaseTexture()
{
	if (!m_baseTexture)
		return 0;

	return m_baseTexture->getTexture();
}

ID3D11ShaderResourceView* Material::getNormalTexture()
{
	if (!m_normalTexture)
		return 0;

	return m_normalTexture->getTexture();
}

ID3D11ShaderResourceView* Material::getAlphaTexture()
{
	if (!m_alphaTexture)
		return 0;

	return m_alphaTexture->getTexture();
}

ID3D11ShaderResourceView* Material::getNoiseTexture()
{
	if (!m_noiseTexture)
		return 0;

	return m_noiseTexture->getTexture();
}

ID3D11ShaderResourceView* Material::getDistortionTexture()
{
	if (!m_distortionTexture)
		return 0;

	return m_distortionTexture->getTexture();
}

ID3D11ShaderResourceView* Material::getPerturbTexture()
{
	if (!m_perturbTexture)
		return 0;

	return m_perturbTexture->getTexture();
}

//========================================================================
// NEW STUFF
//========================================================================

// setters
void Material::setFloat(string keyName, float value)
{
	m_floatProperties[keyName] = value;
}

bool Material::setTexture(string keyName, string value)
{
	if (m_textureProperties.count(keyName))
	{
		return true;
	}

	Texture* newTexture = 0;

	// onload Texture
	AssetManager::getInstance()->loadTexture(&newTexture, value);

	if (!newTexture)
	{
		return false;
	}

	m_textureProperties[keyName] = newTexture;

	return true;
}

void Material::setTexture(string keyName, Texture* value)
{
	m_textureProperties[keyName] = value;
}

void Material::setVector2(string keyName, D3DXVECTOR2 value)
{
	m_vector2Properties[keyName] = value;
}

void Material::setVector3(string keyName, D3DXVECTOR3 value)
{
	m_vector3Properties[keyName] = value;
}

void Material::setVector4(string keyName, D3DXVECTOR4 value)
{
	m_vector4Properties[keyName] = value;
}

// getters
float Material::getFloat(string keyName)
{
	if (m_floatProperties.count(keyName))
	{
		return m_floatProperties[keyName];
	}

	return 0;
}

Texture* Material::getTexture(string keyName)
{
	if (m_textureProperties.count(keyName))
	{
		return m_textureProperties[keyName];
	}

	return 0;
}

D3DXVECTOR2 Material::getVector2(string keyName)
{
	if (m_vector2Properties.count(keyName))
	{
		return m_vector2Properties[keyName];
	}

	return D3DXVECTOR2(0.0f, 0.0f);
}

D3DXVECTOR3 Material::getVector3(string keyName)
{
	if (m_vector3Properties.count(keyName))
	{
		return m_vector3Properties[keyName];
	}

	return D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

D3DXVECTOR4 Material::getVector4(string keyName)
{
	if (m_vector4Properties.count(keyName))
	{
		return m_vector4Properties[keyName];
	}

	return D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f);
}