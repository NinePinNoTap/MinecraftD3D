#include "Fire.h"

Fire::Fire() : GameObject()
{
}

Fire::~Fire()
{
}

// Initialising
bool Fire::Initialise(char* modelFilename, WCHAR* textureFilename, WCHAR* noiseFilename, WCHAR* alphaFilename)
{
	TXTLoader txtLoader;

	//========================
	// Set up Fire Properties
	//========================

	// Define how fast to offset each noise texture
	TranslationSpeed_ = D3DXVECTOR3(1.3f, 2.1f, 2.3f);

	// Define how much to tile each noise texture
	Tiling_ = D3DXVECTOR3(1.0f, 2.0f, 3.0f);

	// Define the distortion factor for each texture
	DistortionA_ = D3DXVECTOR2(0.1f, 0.2f);
	DistortionB_ = D3DXVECTOR2(0.1f, 0.3f);
	DistortionC_ = D3DXVECTOR2(0.1f, 0.1f);

	// Define how much to distort the perturb texture
	DistortionAmount_ = 0.8f;

	// Define where the fire needs to be more solid
	DistortionBias_ = 0.5f;

	//==============
	// Create Model
	//==============

	Model_ = new Model;
	if (!Model_)
	{
		return false;
	}

	Result_ = Model_->Initialise();
	if (!Result_)
	{
		return false;
	}

	// Load Model
	Result_ = txtLoader.LoadModel(modelFilename, *Model_);
	if (!Result_)
	{
		return false;
	}

	//=================
	// Create Material
	//=================

	Model_->GetMaterial()->SetColour(WHITE);

	Result_ = Model_->GetMaterial()->SetBaseTexture(textureFilename);
	if (!Result_)
	{
		return false;
	}

	Result_ = Model_->GetMaterial()->SetAlphaTexture(alphaFilename);
	if (!Result_)
	{
		return false;
	}

	Result_ = Model_->GetMaterial()->SetNoiseTexture(noiseFilename);
	if (!Result_)
	{
		return false;
	}

	//==================
	// Create Transform
	//==================

	Transform_ = new Transform;
	if (!Transform_)
	{
		return false;
	}

	//=================
	// Initialise Vars
	//=================

	Frame_ = 0;
	IsReflectable_ = false;
	IsActive_ = true;
	
	return true;
}

// Frame
bool Fire::Frame()
{
	float angle;

	//==============
	// Animate Fire
	//==============
	
	Frame_ += 0.01f;
	Wrap(Frame_, 0, 1000);

	//===========
	// Billboard
	//===========

	// Calculate the angle between the model and the camera
	angle = atan2(Transform_->GetX() - Camera::Instance()->GetTransform()->GetPosition().x,
				  Transform_->GetZ() - Camera::Instance()->GetTransform()->GetPosition().z);

	// Convert to Degrees
	angle = D3DXToDegree(angle);

	// Apply the rotation to the model
	Transform_->SetRotationY(angle);

	return true;
}

// Property Getters
D3DXVECTOR3 Fire::GetSpeed()
{
	return TranslationSpeed_;
}

D3DXVECTOR3 Fire::GetTiling()
{
	return Tiling_;
}

void Fire::GetDistortion(D3DXVECTOR2& DistortionA, D3DXVECTOR2& DistortionB, D3DXVECTOR2& DistortionC)
{
	DistortionA = DistortionA_;
	DistortionB = DistortionB_;
	DistortionC = DistortionC_;
}

float Fire::GetDistortionAmount()
{
	return DistortionAmount_;
}

float Fire::GetDistortionBias()
{
	return DistortionBias_;
}