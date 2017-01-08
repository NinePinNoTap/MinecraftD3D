#pragma once

#include <d3dx11.h>
#include <d3dx10math.h>

#include "GameObject.h"
#include "Singleton.h"

class Light : public GameObject, public Singleton<Light>
{
public:
	Light();
	Light(const Light&);
	~Light();

	// Initialising
	bool initialise();

	// setters
	void setAmbientColor(float r, float g, float b, float a);
	void setDiffuseColor(float r, float g, float b, float a);
	void setDirection(float x, float y, float z);
	void setSpecularColor(float r, float g, float b, float a);
	void setSpecularPower(float specularPower);

	// setter
	void toggleTime(bool Flag);

	// getters
	D3DXVECTOR4 getAmbientColor();
	D3DXVECTOR4 getDiffuseColor();
	D3DXVECTOR3 getDirection();
	D3DXVECTOR4 getSpecularColor();
	float getSpecularPower();

private:
	// Properties
	D3DXVECTOR4 m_ambientColour;
	D3DXVECTOR4 m_diffuseColour;
	D3DXVECTOR3 m_direction;
	D3DXVECTOR4 m_specularColour;
	float m_specularPower;
};


