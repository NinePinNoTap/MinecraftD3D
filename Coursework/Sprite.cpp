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

	IsReflective_ = RenderMode::Off;
	UseCulling_ = RenderMode::Off;
	UseDepth_ = RenderMode::Off;
	BlendMode_ = BlendMode::NoBlending;

	IsActive_ = true;

	return true;
}

bool Sprite::Render()
{
	if (!IsActive_ || !Shader_ || !Model_)
		return true;

	// Define how we want to see the model
	Shader_->SetRenderMode(ProjectionMode::Orthographic, ViewMode::BaseView);

	// Define how we want the model to be rendered
	SetRenderModes();

	// Render Mesh
	RenderMeshes();

	// Reset Pipeline Settings
	ResetRenderModes();
}

bool Sprite::SetTexture(string textureFilename)
{
	Material* newMaterial = new Material;
	newMaterial->SetBaseTexture(textureFilename);

	Model_->AddMaterial(newMaterial);

	return true;
}