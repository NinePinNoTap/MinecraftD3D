#include "Clouds.h"
#include "DirectXManager.h"

Clouds::Clouds()
{
	// Initialise the pointers to null
	Transform_ = 0;
	Mesh_ = 0;
}

Clouds::Clouds(const Clouds& other)
{
}

Clouds::~Clouds()
{
}

#include "PrimitiveMesh.h"

// Set up
bool Clouds::Initialise(WCHAR* cloudTextureFilename, WCHAR* perturbTextureFilename)
{
	// Set Properties
	QuadCount_ = 100;
	PlaneWidth_ = 100.0f;
	MaxHeight_ = 0.5f;
	TextureRepeat_ = 20;
	ScaleFactor_ = 0.3f;
	Brightness_ = 0.5f;
	Translation_ = 0.0f;
	
	Mesh_ = new PrimitiveMesh;
	Result_ = Mesh_->CreateSkyPlane(QuadCount_, PlaneWidth_, MaxHeight_, TextureRepeat_);
	if (!Result_)
	{
		return false;
	}	

	// Load textures
	Result_ = LoadTextures(cloudTextureFilename, perturbTextureFilename);
	if (!Result_)	{ return false; }

	// Create the transform
	Transform_ = new Transform;
	if (!Transform_)
	{
		return false;
	}

	return true;
}

void Clouds::Shutdown()
{
	// Release the model
	if (Mesh_)
	{
		Mesh_->Shutdown();
		Mesh_ = 0;
	}

	// Release the vertex buffer
	if (Transform_)
	{
		delete Transform_;
		Transform_ = 0;
	}

	return;
}

// Textures
bool Clouds::LoadTextures(WCHAR* textureFilename1, WCHAR* textureFilename2)
{
	// Load our textures
	vector<wstring> TextureFilenames;
	TextureFilenames.push_back(textureFilename1);
	TextureFilenames.push_back(textureFilename2);

	// Create the material
	Result_ = Mesh_->CreateMaterial(TextureFilenames);
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
