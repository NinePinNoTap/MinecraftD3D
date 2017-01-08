#pragma once

#include <d3dx11.h>
#include <d3dx10math.h>

#include "Camera.h"
#include "GameObject.h"
#include "TXTloader.h"

class Fire : public GameObject
{
public:
	Fire();
	~Fire();

	// Initialising
	bool initialise(char* modelFilename, std::string textureFilename, std::string noiseFilename, std::string alphaFilename);

	// update
	bool update();

	// Property getters
	D3DXVECTOR3 getTiling();
	D3DXVECTOR3 getSpeed();
	float getDistortionAmount();
	float getDistortionBias();
	void getDistortion(D3DXVECTOR2& DistortionA, D3DXVECTOR2& DistortionB, D3DXVECTOR2& DistortionC);
	
private:
	// Properties
	D3DXVECTOR3 m_translationSpeed;
	D3DXVECTOR3 m_tiling;
	D3DXVECTOR2 m_distortionA;
	D3DXVECTOR2 m_distortionB;
	D3DXVECTOR2 m_distortionC;
	float m_distortionAmount;
	float m_distortionBias;
};