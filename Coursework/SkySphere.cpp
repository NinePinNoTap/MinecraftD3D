#include "SkySphere.h"
#include "ShaderManager.h"

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

	//============
	// Set Shader
	//============

	Shader_ = ShaderManager::Instance()->GetShader("skysphere");

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

	IsReflective_ = RenderMode::On;
	UseCulling_ = RenderMode::Off;
	UseDepth_ = RenderMode::Off;
	BlendMode_ = BlendMode::NoBlending;

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

bool SkySphere::Frame()
{
	//==============
	// Track Camera
	//==============

	Transform_->SetPosition(Camera::Instance()->GetTransform()->GetPosition());

	return true;
}

bool SkySphere::Render()
{
	if (!IsActive_ || !Shader_ || !Model_)
		return true;

	// Define how we want the model to be rendered
	SetRenderModes();

	// Render Reflection
	if (IsReflective_ == RenderMode::On)
	{
		float currentY = Transform_->GetY();
		D3DXVECTOR3 reflectedCamera = Camera::Instance()->GetTransform()->GetPosition();
		reflectedCamera.y *= -1;

		Transform_->SetY(reflectedCamera.y);

		Shader_->SetRenderMode(ProjectionMode::Perspective, ViewMode::Reflection);

		Texture* reflectionTexture;
		AssetManager::Instance()->LoadTexture(&reflectionTexture, "ReflectionTexture");
		reflectionTexture->SetRenderTarget();

		RenderMeshes();

		DirectXManager::Instance()->SetBackBufferRenderTarget();
		DirectXManager::Instance()->ResetViewport();

		Transform_->SetY(currentY);
	}

	// Define how we want to see the model
	Shader_->SetRenderMode(ProjectionMode::Perspective, ViewMode::View);

	// Render Mesh
	RenderMeshes();

	// Reset Pipeline Settings
	ResetRenderModes();
}

void SkySphere::ToggleTime(bool timeMode)
{
	int ID = (int)timeMode;

	// Update Material
	Model_->GetMaterial()->SetVector4("TopColour", Colors_[ID][0]);
	Model_->GetMaterial()->SetVector4("CenterColour", Colors_[ID][1]);
}