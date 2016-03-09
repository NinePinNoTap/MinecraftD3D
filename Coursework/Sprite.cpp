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

	Result_ = Model_->Initialise();
	if (!Result_)
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

bool Sprite::SetTexture(WCHAR* filename)
{
	// Create a texture
	Result_ = Model_->GetMaterial()->SetTextureArray(vector<wstring>(1, filename));
	if (!Result_)
	{
		return false;
	}

	return true;
}