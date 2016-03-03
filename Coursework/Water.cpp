#include "Water.h"
#include "DirectXManager.h"
#include "PrimitiveMesh.h"

Water::Water() : Primitive()
{
	// Initialise the pointers to null
	VertexBuffer_ = 0;
	IndexBuffer_ = 0;
}

Water::Water(const Water& other)
{
}

Water::~Water()
{
}

// Initialising
bool Water::Initialise(WCHAR* textureFilename, Rect3D WaterResolution)
{
	// Initialise water variables
	WaterHeight_ = WaterResolution.depth;
	NormalMapTiling_ = D3DXVECTOR2(0.01f, 0.02f);
	Frame_ = 0.0f;
	WaterTimer_ = 0.0f;
	ReflectRefractScale_ = 0.03f;
	RefractionTint_ = D3DXVECTOR4(0.0f, 0.8f, 1.0f, 1.0f);
	WaterShininess_ = 50.0f;
	WaveHeight_ = 1.5f;
	WaveSpeed_ = 0.025f;
	Tessellation_ = 58.0f;

	// Create the water plane
	Mesh_ = new PrimitiveMesh;
	if (!Mesh_)
	{
		return false;
	}
	Result_ = Mesh_->CreatePlane(WaterResolution, Rect3D(1, 1), 1);
	if (!Result_)
	{
		return false;
	}
	Result_ = Mesh_->CreateMaterial(NULL, textureFilename);
	if (!Result_)
	{
		return false;
	}

	// Return the buffers as we use a different render method
	VertexBuffer_ = Mesh_->GetVertexBuffer();
	IndexBuffer_ = Mesh_->GetIndexBuffer();

	// Create the transform
	Transform_ = new Transform;
	if (!Transform_)
	{
		return false;
	}

	return true;
}

// Shutdown
void Water::Shutdown()
{
	// Release the texture object
	if (Mesh_)
	{
		Mesh_->Shutdown();
		delete Mesh_;
		Mesh_ = 0;
	}

	if (Transform_)
	{
		delete Transform_;
		Transform_ = 0;
	}

	return;
}

// Frame
bool Water::Frame()
{
	// Update the position of the water to simulate motion
	Frame_ += 0.003f;
	Wrap(Frame_, 0, 1.0f);

	// Update the waater time to simulate waves
	WaterTimer_ += WaveSpeed_;

	return true;
}

// Game Settings
void Water::ToggleTime(bool NightTimeMode)
{
	// Toggle between good weather and bad weather
	if (NightTimeMode)
	{
		WaveHeight_ = 2.0f;
		WaveSpeed_ = 0.06f;
	}
	else
	{
		WaveHeight_ = 1.5f;
		WaveSpeed_ = 0.025f;
	}
}

// Getters
float Water::GetTessellation()
{
	return Tessellation_;
}

float Water::GetWaterHeight()
{
	return WaterHeight_;
}

float Water::GetWaveHeight()
{
	return WaveHeight_;
}

D3DXVECTOR2 Water::GetNormalMapTiling()
{
	return NormalMapTiling_;
}

float Water::GetWaterTimer()
{
	return WaterTimer_;
}

float Water::GetReflectRefractScale()
{
	return ReflectRefractScale_;
}

D3DXVECTOR4 Water::GetRefractionTint()
{
	return RefractionTint_;
}

float Water::GetSpecularShininess()
{
	return WaterShininess_;
}