#include "Light.h"

Light::Light()
{
}

Light::Light(const Light& other)
{
}

Light::~Light()
{
}

bool Light::initialise()
{
	// create a transform
	m_transform = new Transform;
	if (!m_transform)
	{
		return false;
	}

	// set Light Position
	m_transform->setPosition(-3500.0f, 9900.0f, 2100.0f);

	// initialise the default lighting effects
	setAmbientColor(0.5f, 0.5f, 0.5f, 1.0f);
	setDiffuseColor(0.5f, 0.5f, 0.5f, 1.0f);
	setDirection(0.5f, -0.75f, 0.25f);
	setSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	setSpecularPower(32.0f);

	return true;
}

// setters
void Light::setAmbientColor(float red, float green, float blue, float alpha)
{
	m_ambientColour = D3DXVECTOR4(red, green, blue, alpha);
}

void Light::setDiffuseColor(float red, float green, float blue, float alpha)
{
	m_diffuseColour = D3DXVECTOR4(red, green, blue, alpha);
}

void Light::setDirection(float x, float y, float z)
{
	m_direction = D3DXVECTOR3(x, y, z);
}

void Light::setSpecularColor(float red, float green, float blue, float alpha)
{
	m_specularColour = D3DXVECTOR4(red, green, blue, alpha);
}

void Light::setSpecularPower(float specularPower)
{
	m_specularPower = specularPower;
}

// Scene Updating
void Light::toggleTime(bool NightTimeMode)
{
	// Toggle between good weather and bad weather
	if (NightTimeMode)
	{
		setAmbientColor(0.25f, 0.25f, 0.25f, 1.0f);
		setDiffuseColor(0.25f, 0.25f, 0.25f, 1.0f);
	}
	else
	{
		setAmbientColor(0.75f, 0.75f, 0.75f, 1.0f);
		setDiffuseColor(0.75f, 0.75f, 0.75f, 1.0f);
	}
}

// Returns
D3DXVECTOR4 Light::getAmbientColor()
{
	return m_ambientColour;
}

D3DXVECTOR4 Light::getDiffuseColor()
{
	return m_diffuseColour;
}

D3DXVECTOR3 Light::getDirection()
{
	return m_direction;
}

D3DXVECTOR4 Light::getSpecularColor()
{
	return m_specularColour;
}

float Light::getSpecularPower()
{
	return m_specularPower;
}