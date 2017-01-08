#include "Fire.h"

Fire::Fire() : GameObject()
{
}

Fire::~Fire()
{
}

// Initialising
bool Fire::initialise(char* modelFilename, std::string textureFilename, std::string noiseFilename, std::string alphaFilename)
{
	TXTloader txtloader;

	//========================
	// set up Fire Properties
	//========================

	// Define how fast to offset each noise texture
	m_translationSpeed = D3DXVECTOR3(1.3f, 2.1f, 2.3f);

	// Define how much to tile each noise texture
	m_tiling = D3DXVECTOR3(1.0f, 2.0f, 3.0f);

	// Define the distortion factor for each texture
	m_distortionA = D3DXVECTOR2(0.1f, 0.2f);
	m_distortionB = D3DXVECTOR2(0.1f, 0.3f);
	m_distortionC = D3DXVECTOR2(0.1f, 0.1f);

	// Define how much to distort the perturb texture
	m_distortionAmount = 0.8f;

	// Define where the fire needs to be more solid
	m_distortionBias = 0.5f;

	//==============
	// create Model
	//==============

	m_model = new Model;
	if (!m_model)
	{
		return false;
	}

	// onload Model
	m_result = txtloader.loadModel(modelFilename, *m_model);
	if (!m_result)
	{
		return false;
	}

	//=================
	// create Material
	//=================

	Material* newMaterial = new Material;
	m_result = newMaterial->setBaseTexture(textureFilename);
	if (!m_result)
	{
		return false;
	}

	m_result = newMaterial->setAlphaTexture(alphaFilename);
	if (!m_result)
	{
		return false;
	}

	m_result = newMaterial->setNoiseTexture(noiseFilename);
	if (!m_result)
	{
		return false;
	}
	m_model->addMaterial(newMaterial);

	//==================
	// create Transform
	//==================

	m_transform = new Transform;
	if (!m_transform)
	{
		return false;
	}

	//=================
	// initialise Vars
	//=================

	m_frame = 0;
	
	m_reflective = renderMode::Off;
	m_culled = renderMode::Off;
	m_depth = renderMode::On;
	m_blendMode = BlendMode::AlphaMasked;

	m_isActive = true;
	
	return true;
}

// update
bool Fire::update()
{
	float angle;

	//==============
	// Animate Fire
	//==============
	
	m_frame += 0.01f;
	wrap(m_frame, 0, 1000);

	//===========
	// Billboard
	//===========

	// Calculate the angle between the model and the camera
	angle = atan2(m_transform->getX() - Camera::getInstance()->getTransform()->getPosition().x,
				  m_transform->getZ() - Camera::getInstance()->getTransform()->getPosition().z);

	// Convert to Degrees
	angle = D3DXToDegree(angle);

	// Apply the rotation to the model
	m_transform->setRotationY(angle);

	return true;
}

// Property getters
D3DXVECTOR3 Fire::getSpeed()
{
	return m_translationSpeed;
}

D3DXVECTOR3 Fire::getTiling()
{
	return m_tiling;
}

void Fire::getDistortion(D3DXVECTOR2& DistortionA, D3DXVECTOR2& DistortionB, D3DXVECTOR2& DistortionC)
{
	DistortionA = m_distortionA;
	DistortionB = m_distortionB;
	DistortionC = m_distortionC;
}

float Fire::getDistortionAmount()
{
	return m_distortionAmount;
}

float Fire::getDistortionBias()
{
	return m_distortionBias;
}