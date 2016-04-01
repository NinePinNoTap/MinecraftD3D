#include "Water.h"

Water::Water() : GameObject()
{
}

Water::Water(const Water& other)
{
}

Water::~Water()
{
}

// Initialising
bool Water::Initialise(string textureFilename, Rect3D waterResolution)
{
	PrimitiveFactory primitiveFactory;

	// Initialise water variables
	WaterHeight_ = waterResolution.depth;
	NormalMapTiling_ = D3DXVECTOR2(0.01f, 0.02f);
	Frame_ = 0.0f;
	WaterTranslation_ = 0.0f;
	ReflectRefractScale_ = 0.03f;
	RefractionTint_ = D3DXVECTOR4(0.0f, 0.8f, 1.0f, 1.0f);
	WaterShininess_ = 50.0f;
	WaveHeight_ = 1.5f;
	WaveSpeed_ = 0.025f;
	Tessellation_ = 58.0f;
	
	//==============
	// Create Model
	//==============

	Model_ = new Model;
	if (!Model_)
	{
		return false;
	}

	// Load Model
	Result_ = primitiveFactory.CreatePlane(waterResolution, Rect3D(1, 1), 1.0f, *Model_);
	if (!Result_)
	{
		return false;
	}

	//=================
	// Create Material
	//=================

	Material* newMaterial = new Material;
	Result_ = newMaterial->SetNormalTexture(textureFilename);
	if (!Result_)
	{
		return false;
	}
	Model_->AddMaterial(newMaterial);

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

// Shutdown
void Water::Shutdown()
{
	// Release the texture object
	if (Model_)
	{
		Model_->Shutdown();
		delete Model_;
		Model_ = 0;
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
	WaterTranslation_ += 0.003f;
	Wrap(WaterTranslation_, 0, 1.0f);

	// Update the waater time to simulate waves
	Frame_ += WaveSpeed_;

	return true;
}

bool Water::Render()
{
	unsigned int stride;
	unsigned int offset;
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;

	// Set vertex buffer stride and offset
	stride = sizeof(VertexData);
	offset = 0;

	// Get Mesh Buffers
	vertexBuffer = Model_->GetMesh()->GetVertexBuffer();
	indexBuffer = Model_->GetMesh()->GetIndexBuffer();

	// Set the vertex buffer to active in the InputManager assembler so it can be rendered
	DirectXManager::Instance()->GetDeviceContext()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the InputManager assembler so it can be rendered
	DirectXManager::Instance()->GetDeviceContext()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer
	DirectXManager::Instance()->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST);

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

float Water::GetWaterTranslation()
{
	return WaterTranslation_;
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