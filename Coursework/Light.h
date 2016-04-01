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
	bool Initialise();

	// Setters
	void SetAmbientColor(float r, float g, float b, float a);
	void SetDiffuseColor(float r, float g, float b, float a);
	void SetDirection(float x, float y, float z);
	void SetSpecularColor(float r, float g, float b, float a);

	// Setter
	void ToggleTime(bool Flag);

	// Getters
	D3DXVECTOR4 GetAmbientColor();
	D3DXVECTOR4 GetDiffuseColor();
	D3DXVECTOR3 GetDirection();
	D3DXVECTOR4 GetSpecularColor();

private:
	// Properties
	D3DXVECTOR4 AmbientColour_;
	D3DXVECTOR4 DiffuseColor_;
	D3DXVECTOR3 Direction_;
	D3DXVECTOR4 SpecularColour_;
};


