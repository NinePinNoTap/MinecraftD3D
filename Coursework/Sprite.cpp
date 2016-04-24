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

bool Sprite::Initialise(Rect3D spriteResolution, string textureFilename)
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
	Result_ = primitiveFactory.Create2DBox(spriteResolution, *Model_);
	if (!Result_)
	{
		return false;
	}

	//=================
	// Create Material
	//=================

	Material* newMaterial = new Material;
	newMaterial -> SetBaseTexture(textureFilename);
	Model_ -> AddMaterial(newMaterial);

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
	
	// Render Modes
	IsReflective_ = RenderMode::Off;
	UseCulling_ = RenderMode::Off;
	UseDepth_ = RenderMode::Off;
	BlendMode_ = BlendMode::NoBlending;

	// Animations
	Frame_ = 0;

	// Flags
	IsActive_ = true;

	// Define Base Shader
	SetShader("texture");

	// Clean Up
	newMaterial = 0;

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

	return true;
}