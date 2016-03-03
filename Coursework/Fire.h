#pragma once

#include <d3dx11.h>
#include <d3dx10math.h>

#include "Model.h"

class Fire : public Model
{
public:
	Fire();
	~Fire();

	// Initialising
	bool Initialise(char* modelFilename, WCHAR* textureFilename, WCHAR* noiseFilename, WCHAR* alphaFilename);

	// Shutdown
	void Shutdown();

	// Frame
	bool Frame();

	// Camera
	void SetCameraPosition(D3DXVECTOR3 CameraPosition);

	// Property Getters
	D3DXVECTOR3 GetTiling();
	D3DXVECTOR3 GetSpeed();
	float GetDistortionAmount();
	float GetDistortionBias();
	void GetDistortion(D3DXVECTOR2& DistortionA, D3DXVECTOR2& DistortionB, D3DXVECTOR2& DistortionC);
	
private:
	// Billboarding
	D3DXVECTOR3 CameraPosition_;

	// Properties
	D3DXVECTOR3 TranslationSpeed_;
	D3DXVECTOR3 Tiling_;
	D3DXVECTOR2 DistortionA_;
	D3DXVECTOR2 DistortionB_;
	D3DXVECTOR2 DistortionC_;
	float DistortionAmount_;
	float DistortionBias_;
};