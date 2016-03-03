#include "Fire.h"

Fire::Fire() : Model()
{
}

Fire::~Fire()
{
}

// Initialising
bool Fire::Initialise(char* modelFilename, WCHAR* textureFilename, WCHAR* noiseFilename, WCHAR* alphaFilename)
{
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

	//=================
	// Set up the Fire
	//=================

	Mesh_ = new ModelMesh;
	Result_ = Mesh_->LoadModel(modelFilename);
	if (!Result_)
	{
		return false;
	}

	// Create fire textures
	Result_ = Mesh_->CreateMaterial(textureFilename, 0, alphaFilename, noiseFilename);
	if (!Result_)
	{
		return false;
	}

	// Initialise the animation frame counter
	Frame_ = 0;

	// Create the transform
	Transform_ = new Transform;
	if (!Transform_)
	{
		return false;
	}

	// Initialise flags
	IsActive_ = false;
	
	return true;
}

// Shutdown
void Fire::Shutdown()
{
	// Shutdown Model
	if ( Mesh_ )
	{
		Mesh_ -> Shutdown();
		delete Mesh_;
		Mesh_ = 0;
	}
}

// Frame
bool Fire::Frame()
{
	//==============
	// Animate Fire
	//==============
	
	Frame_ += 0.01f;
	Wrap(Frame_, 0, 1000);

	//===========
	// Billboard
	//===========

	// Calculate the angle between the model and the camera
	float angle = atan2(Transform_->GetX() - CameraPosition_.x, Transform_->GetZ() - CameraPosition_.z);
	angle = D3DXToDegree(angle);

	// Apply the rotation to the model
	Transform_->SetRotationY(angle);

	return true;
}

// Billboarding
void Fire::SetCameraPosition(D3DXVECTOR3 CameraPosition)
{
	CameraPosition_ = CameraPosition;
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