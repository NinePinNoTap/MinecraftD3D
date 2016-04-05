#include "Clouds.h"
#include "DirectXManager.h"
#include "ShaderManager.h"

Clouds::Clouds() : GameObject()
{
}

Clouds::Clouds(const Clouds& other)
{
}

Clouds::~Clouds()
{
}

// Set up
bool Clouds::Initialise(string cloudTextureFilename, string perturbTextureFilename)
{
	PrimitiveFactory primitiveFactory;

	// Set Properties
	ScaleFactor_ = 0.3f;
	Brightness_ = 0.5f;

	//==============
	// Create Model
	//==============

	Model_ = new Model;
	if (!Model_)
	{
		return false;
	}

	// Load Model
	Result_ = primitiveFactory.CreateSkyPlane(100, 100.0f, 0.5f, 20, *Model_);
	if (!Result_)
	{
		return false;
	}

	// Create the material
	Material* newMaterial = new Material;
	Result_ = newMaterial->SetTexture("BaseTexture", cloudTextureFilename);
	if (!Result_)
	{
		return false;
	}
	Result_ = newMaterial->SetTexture("PerturbTexture", perturbTextureFilename);
	if (!Result_)
	{
		return false;
	}

	// Set material properties
	newMaterial->SetFloat("TextureScale", ScaleFactor_);
	newMaterial->SetFloat("TextureBrightness", Brightness_);
	Model_->AddMaterial(newMaterial);

	//============
	// Set Shader
	//============

	Shader_ = ShaderManager::Instance()->GetShader("cloud");
	if (!Shader_)
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
bool Clouds::Frame()
{
	//================
	// Animate Clouds
	//================

	Frame_ += 0.0001f;
	if (Frame_ > 1.0f)
	{
		Frame_ -= 1.0f;
	}

	Model_->GetMaterial()->SetFloat("Frame", Frame_);

	return true;
}

// Getters
float Clouds::GetScale()
{
	return ScaleFactor_;
}

float Clouds::GetBrightness()
{
	return Brightness_;
}
