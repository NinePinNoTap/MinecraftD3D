#include "Material.h"

Material::Material()
{
	// Initialise pointers
	BaseTexture_ = 0;
	NormalTexture_ = 0;
	AlphaTexture_ = 0;
	NoiseTexture_ = 0;
	DistortionTexture_ = 0;
	TextureArray_ = 0;
	Tint_ = Colour(1.0f, 1.0f, 1.0f, 1.0f);
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
	if (TextureArray_)
	{
		TextureArray_->Shutdown();
		delete TextureArray_;
		TextureArray_ = 0;
	}
}

// Setters
void Material::SetColour(Colour tint)
{
	Tint_ = tint;
}

bool Material::SetBaseTexture(WCHAR* textureFilename)
{
	// Create the texture
	BaseTexture_ = CreateTexture(textureFilename);

	if (!BaseTexture_)
	{
		return false;
	}

	return true;
}

bool Material::SetNormalTexture(WCHAR* textureFilename)
{
	// Create the texture
	NormalTexture_ = CreateTexture(textureFilename);

	if (!NormalTexture_)
	{
		return false;
	}

	return true;
}

bool Material::SetAlphaTexture(WCHAR* textureFilename)
{
	// Create the texture
	AlphaTexture_ = CreateTexture(textureFilename);

	if (!AlphaTexture_)
	{
		return false;
	}

	return true;
}

bool Material::SetNoiseTexture(WCHAR* textureFilename)
{
	// Create the texture
	NoiseTexture_ = CreateTexture(textureFilename);

	if (!NoiseTexture_)
	{
		return false;
	}

	return true;
}

bool Material::SetDistortionTexture(WCHAR* textureFilename)
{
	// Create the texture
	DistortionTexture_ = CreateTexture(textureFilename);

	if (!DistortionTexture_)
	{
		return false;
	}

	return true;
}

bool Material::SetTextureArray(vector<wstring> textureFilenames)
{
	// Create the object
	TextureArray_ = new TextureArray;
	if (!TextureArray_)
	{
		return false;
	}

	// Create the textures
	return TextureArray_->Initialise(textureFilenames);
}

Texture* Material::CreateTexture(WCHAR* textureFilename)
{
	Texture* NewTexture;

	// Create the object
	NewTexture = new Texture;
	if (!NewTexture)
	{
		return false;
	}

	// Load the texture
	NewTexture->Initialise(textureFilename);

	return NewTexture;
}

// Getters
Colour Material::GetTint()
{
	return Tint_;
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

ID3D11ShaderResourceView* Material::GetTextureArray()
{
	if (!TextureArray_)
		return 0;

	return TextureArray_->GetTextures();
}