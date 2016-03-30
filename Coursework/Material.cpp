#include "Material.h"
#include "AssetManager.h"

Material::Material()
{
	// Initialise pointers
	BaseTexture_ = 0;
	NormalTexture_ = 0;
	AlphaTexture_ = 0;
	NoiseTexture_ = 0;
	DistortionTexture_ = 0;
	PerturbTexture_ = 0;

	// Initialise colours
	Tint_ = WHITE;
	SpecularAmount_ = 1.0f;
}

Material::~Material()
{

}

// Shutdown
void Material::Shutdown()
{
	if (BaseTexture_)
	{
		BaseTexture_->Shutdown();
		delete BaseTexture_;
		BaseTexture_ = 0;
	}
	if (NormalTexture_)
	{
		NormalTexture_->Shutdown();
		delete NormalTexture_;
		NormalTexture_ = 0;
	}
	if (AlphaTexture_)
	{
		AlphaTexture_->Shutdown();
		delete AlphaTexture_;
		AlphaTexture_ = 0;
	}
	if (NoiseTexture_)
	{
		NoiseTexture_->Shutdown();
		delete NoiseTexture_;
		NoiseTexture_ = 0;
	}
	if (DistortionTexture_)
	{
		DistortionTexture_->Shutdown();
		delete DistortionTexture_;
		DistortionTexture_ = 0;
	}
	if (PerturbTexture_)
	{
		PerturbTexture_->Shutdown();
		delete PerturbTexture_;
		PerturbTexture_ = 0;
	}
}

// Setters
void Material::SetColour(Colour tint)
{
	Tint_ = tint;
}

void Material::SetSpecular(float amount)
{
	SpecularAmount_ = amount;
}

bool Material::SetBaseTexture(std::string textureFilename)
{
	// Create the texture
	AssetManager::Instance()->LoadTexture(&BaseTexture_, textureFilename);

	if (!BaseTexture_)
	{
		return false;
	}

	return true;
}

bool Material::SetNormalTexture(std::string textureFilename)
{
	// Create the texture
	AssetManager::Instance()->LoadTexture(&NormalTexture_, textureFilename);

	if (!NormalTexture_)
	{
		return false;
	}

	return true;
}

bool Material::SetAlphaTexture(std::string textureFilename)
{
	// Create the texture
	AssetManager::Instance()->LoadTexture(&AlphaTexture_, textureFilename);

	if (!AlphaTexture_)
	{
		return false;
	}

	return true;
}

bool Material::SetNoiseTexture(std::string textureFilename)
{
	// Create the texture
	AssetManager::Instance()->LoadTexture(&NoiseTexture_, textureFilename);

	if (!NoiseTexture_)
	{
		return false;
	}

	return true;
}

bool Material::SetDistortionTexture(std::string textureFilename)
{
	// Create the texture
	AssetManager::Instance()->LoadTexture(&DistortionTexture_, textureFilename);

	if (!DistortionTexture_)
	{
		return false;
	}

	return true;
}

bool Material::SetPerturbTexture(std::string textureFilename)
{
	// Create the texture
	AssetManager::Instance()->LoadTexture(&PerturbTexture_, textureFilename);

	if (!PerturbTexture_)
	{
		return false;
	}

	return true;
}

// Getters
Colour Material::GetTint()
{
	return Tint_;
}

float Material::GetSpecular()
{
	return SpecularAmount_;
}

ID3D11ShaderResourceView* Material::GetBaseTexture()
{
	if (!BaseTexture_)
		return 0;

	return BaseTexture_->GetTexture();
}

ID3D11ShaderResourceView* Material::GetNormalTexture()
{
	if (!NormalTexture_)
		return 0;

	return NormalTexture_->GetTexture();
}

ID3D11ShaderResourceView* Material::GetAlphaTexture()
{
	if (!AlphaTexture_)
		return 0;

	return AlphaTexture_->GetTexture();
}

ID3D11ShaderResourceView* Material::GetNoiseTexture()
{
	if (!NoiseTexture_)
		return 0;

	return NoiseTexture_->GetTexture();
}

ID3D11ShaderResourceView* Material::GetDistortionTexture()
{
	if (!DistortionTexture_)
		return 0;

	return DistortionTexture_->GetTexture();
}

ID3D11ShaderResourceView* Material::GetPerturbTexture()
{
	if (!PerturbTexture_)
		return 0;

	return PerturbTexture_->GetTexture();
}