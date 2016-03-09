#include "Clouds.h"
#include "DirectXManager.h"

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
bool Clouds::Initialise(WCHAR* cloudTextureFilename, WCHAR* perturbTextureFilename)
{
	PrimitiveFactory primitiveFactory;

	// Set Properties
	QuadCount_ = 100;
	PlaneWidth_ = 100.0f;
	MaxHeight_ = 0.5f;
	TextureRepeat_ = 20;
	ScaleFactor_ = 0.3f;
	Brightness_ = 0.5f;
	Translation_ = 0.0f;

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
	Result_ = primitiveFactory.CreateSkyPlane(QuadCount_, PlaneWidth_, MaxHeight_, TextureRepeat_, *Model_);
	if (!Result_)
	{
		return false;
	}

	// Load Textures
	Result_ = LoadTextures(cloudTextureFilename, perturbTextureFilename);
	if (!Result_)	{ return false; }

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

// Textures
bool Clouds::LoadTextures(WCHAR* textureFilename1, WCHAR* textureFilename2)
{
	// Load our textures
	vector<wstring> TextureFilenames;
	TextureFilenames.push_back(textureFilename1);
	TextureFilenames.push_back(textureFilename2);

	// Create the material
	Result_ = Model_->GetMaterial()->SetTextureArray(TextureFilenames);
	if (!Result_)
	{
		return false;
	}

	// Clean up
	TextureFilenames.clear();

	return true;
}

// Frame
bool Clouds::Frame()
{
	//================
	// Animate Clouds
	//================

	Translation_ += 0.0001f;
	if (Translation_ > 1.0f)
	{
		Translation_ -= 1.0f;
	}

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

float Clouds::GetTranslation()
{
	return Translation_;
}
