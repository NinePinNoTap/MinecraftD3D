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

bool Light::Initialise()
{
	// Create a transform
	Transform_ = new Transform;
	if (!Transform_)
	{
		return false;
	}

	// Set Light Position
	Transform_->SetPosition(-3500.0f, 9900.0f, 2100.0f);

	// Initialise the default lighting effects
	SetAmbientColor(0.5f, 0.5f, 0.5f, 1.0f);
	SetDiffuseColor(0.5f, 0.5f, 0.5f, 1.0f);
	SetDirection(0.5f, -0.75f, 0.25f);
	SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	SetSpecularPower(32.0f);

	return true;
}

// Setters
void Light::SetAmbientColor(float red, float green, float blue, float alpha)
{
	AmbientColour_ = D3DXVECTOR4(red, green, blue, alpha);
	return;
}

void Light::SetDiffuseColor(float red, float green, float blue, float alpha)
{
	DiffuseColor_ = D3DXVECTOR4(red, green, blue, alpha);
	return;
}

void Light::SetDirection(float x, float y, float z)
{
	Direction_ = D3DXVECTOR3(x, y, z);
	return;
}

void Light::SetSpecularColor(float red, float green, float blue, float alpha)
{
	SpecularColour_ = D3DXVECTOR4(red, green, blue, alpha);
	return;
}

void Light::SetSpecularPower(float power)
{
	SpecularPower_ = power;
	return;
}

// Scene Updating
void Light::ToggleTime(bool NightTimeMode)
{
	// Toggle between good weather and bad weather
	if (NightTimeMode)
	{
		SetAmbientColor(0.25f, 0.25f, 0.25f, 1.0f);
		SetDiffuseColor(0.25f, 0.25f, 0.25f, 1.0f);
	}
	else
	{
		SetAmbientColor(0.75f, 0.75f, 0.75f, 1.0f);
		SetDiffuseColor(0.75f, 0.75f, 0.75f, 1.0f);
	}
}

// Returns
D3DXVECTOR4 Light::GetAmbientColor()
{
	return AmbientColour_;
}

D3DXVECTOR4 Light::GetDiffuseColor()
{
	return DiffuseColor_;
}

D3DXVECTOR3 Light::GetDirection()
{
	return Direction_;
}

D3DXVECTOR4 Light::GetSpecularColor()
{
	return SpecularColour_;
}

float Light::GetSpecularPower()
{
	return SpecularPower_;
}