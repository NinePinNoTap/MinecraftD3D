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
	Result_ = newMaterial->SetTexture("NormalTexture", textureFilename);
	if (!Result_)
	{
		return false;
	}

	// Add values
	newMaterial->SetFloat("FrameTime", 0.0f);
	newMaterial->SetFloat("WaveHeight", 1.5f);
	newMaterial->SetFloat("TextureOffset", 0.0f);
	newMaterial->SetFloat("SpecularPower", 50.0f);
	newMaterial->SetFloat("ReflectRefractScale", 0.03f);
	newMaterial->SetFloat("TessellationAmount", 58.0f);
	newMaterial->SetVector2("NormalMapTiling", D3DXVECTOR2(0.01f, 0.02f));
	newMaterial->SetVector4("RefractionTint", D3DXVECTOR4(0.0f, 0.8f, 1.0f, 1.0f));

	// Add material to model
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
	WaveSpeed_ = 0.025f;
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
	float textureOffset;

	// Get current texture offset
	textureOffset = Model_->GetMaterial()->GetFloat("TextureOffset");

	// Update the position of the water to simulate motion
	textureOffset += 0.003f;
	Wrap(textureOffset, 0, 1.0f);

	Model_->GetMaterial()->SetFloat("TextureOffset", textureOffset);

	// Update the water time to simulate waves
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
		Model_->GetMaterial()->SetFloat("WaveHeight", 2.0f);
		WaveSpeed_ = 0.06f;
	}
	else
	{
		Model_->GetMaterial()->SetFloat("WaveHeight", 1.5f);
		WaveSpeed_ = 0.025f;
	}
}