#pragma once

#include <d3dx11.h>
#include <d3dx10math.h>

#include "Camera.h"
#include "GameObject.h"
#include "TXTLoader.h"

class Fire : public GameObject
{
public:
	Fire();
	~Fire();

	// Initialising
	bool Initialise(char* modelFilename, WCHAR* textureFilename, WCHAR* noiseFilename, WCHAR* alphaFilename);

	// Frame
	bool Frame();

	// Property Getters
	D3DXVECTOR3 GetTiling();
	D3DXVECTOR3 GetSpeed();
	float GetDistortionAmount();
	float GetDistortionBias();
	void GetDistortion(D3DXVECTOR2& DistortionA, D3DXVECTOR2& DistortionB, D3DXVECTOR2& DistortionC);
	
private:
	// Properties
	D3DXVECTOR3 TranslationSpeed_;
	D3DXVECTOR3 Tiling_;
	D3DXVECTOR2 DistortionA_;
	D3DXVECTOR2 DistortionB_;
	D3DXVECTOR2 DistortionC_;
	float DistortionAmount_;
	float DistortionBias_;
};