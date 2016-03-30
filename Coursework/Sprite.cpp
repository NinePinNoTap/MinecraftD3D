#include "Sprite.h"

Sprite::Sprite() : GameObject()
{
}

Sprite::Sprite(const Sprite& other)
{
}

Sprite::~Sprite()
{
}

bool Sprite::Initialise(Rect3D Dimensions)
{
	PrimitiveFactory primitiveFactory;

	//==============
	// Create Model
	//==============

	Model_ = new Model;
	if (!Model_)
	{
		return false;
	}

	// Load Model
	Result_ = primitiveFactory.Create2DBox(Dimensions, *Model_);
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

bool Sprite::SetTexture(string textureFilename)
{
	Material* newMaterial = new Material;
	newMaterial->SetBaseTexture(textureFilename);

	Model_->AddMaterial(newMaterial);

	return true;
}