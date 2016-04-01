#include "SkySphere.h"

SkySphere::SkySphere() : GameObject()
{
	
}

SkySphere::SkySphere(const SkySphere& other)
{
}

SkySphere::~SkySphere()
{
}

bool SkySphere::Initialise(const char* filename)
{
	TXTLoader txtLoader;

	//==============
	// Create Model
	//==============

	// Load Model
	AssetManager::Instance()->LoadModel(&Model_, filename);
	if (!Model_)
	{
		return false;
	}

	// Create Material
	Model_->AddMaterial(new Material);

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

	// Initialise sky color for daytime
	Colors_[0][1] = D3DXVECTOR4(0.65f, 0.77f, 0.85f, 1.0f); //Top
	Colors_[0][0] = D3DXVECTOR4(0.18f, 0.35f, 0.54f, 1.0f); //Center

	// Initialise the sky color for nighttime
	Colors_[1][0] = D3DXVECTOR4(0.0f, 0.2f, 0.2f, 1.0f); //Top
	Colors_[1][1] = D3DXVECTOR4(0.2f, 0.4f, 0.4f, 1.0f); //Center

	// Initialise the current color for daytime
	Model_->GetMaterial()->SetVector4("TopColour", Colors_[0][0]);
	Model_->GetMaterial()->SetVector4("CenterColour", Colors_[0][1]);

	return true;
}

void SkySphere::ToggleTime(bool timeMode)
{
	int ID = (int)timeMode;

	// Update Material
	Model_->GetMaterial()->SetVector4("TopColour", Colors_[ID][0]);
	Model_->GetMaterial()->SetVector4("CenterColour", Colors_[ID][1]);
}