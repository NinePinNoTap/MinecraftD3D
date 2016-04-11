#include "Ocean.h"
#include "AssetManager.h"

Ocean::Ocean() : GameObject()
{
}

Ocean::Ocean(const Ocean& other)
{
}

Ocean::~Ocean()
{
}

// Initialising
bool Ocean::Initialise(string textureFilename, Rect3D waterResolution)
{
	PrimitiveFactory primitiveFactory;
	Texture* refractionTexture;
	Texture* reflectionTexture;

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

	// Retrieve refraction and reflection texture pointers
	AssetManager::Instance()->LoadTexture(&refractionTexture, "RefractionTexture");
	AssetManager::Instance()->LoadTexture(&reflectionTexture, "ReflectionTexture");

	// Create material
	Material* newMaterial = new Material;
	Result_ = newMaterial->SetNormalTexture(textureFilename);
	if (!Result_)
	{
		return false;
	}
	newMaterial->SetTexture("RefractionTexture", refractionTexture);
	newMaterial->SetTexture("ReflectionTexture", reflectionTexture);
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
	SetShader("ocean");

	// Clean Up
	newMaterial = 0;
	refractionTexture = 0;
	reflectionTexture = 0;

	return true;
}

// Shutdown
void Ocean::Shutdown()
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
bool Ocean::Frame()
{
	// Update the position of the water to simulate motion
	WaterTranslation_ += 0.003f;
	Wrap(WaterTranslation_, 0, 1.0f);

	// Update the waater time to simulate waves
	Frame_ += WaveSpeed_;

	return true;
}

bool Ocean::Render()
{
	// Make sure the object is active
	if (!IsActive_)
		return true;

	// Make sure we have a shader to use
	if (!Shader_)
		return true;

	if (!Model_)
		return true;

	// Render the model
	for (int i = 0; i < Model_->GetMeshCount(); i++)
	{
		// Make sure the mesh is active for it to be rendered
		if (Model_->GetMesh(i)->IsActive())
		{
			// Send model to pipeline
			SendModelToPipeline(Model_->GetMesh(i));

			// Send material to shader
			Shader_->Prepare(Model_->GetMaterial(i), Transform_);

			// Render Object
			Shader_->Render(Model_->GetMesh(i)->GetIndexCount());
		}
	}
}

bool Ocean::SendModelToPipeline(Mesh3D* objMesh)
{
	unsigned int stride;
	unsigned int offset;
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;

	// Set vertex buffer stride and offset
	stride = sizeof(VertexData);
	offset = 0;

	// Get Mesh Buffers
	vertexBuffer = objMesh->GetVertexBuffer();
	indexBuffer = objMesh->GetIndexBuffer();

	// Set the vertex buffer to active in the InputManager assembler so it can be rendered
	DirectXManager::Instance()->GetDeviceContext()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the InputManager assembler so it can be rendered
	DirectXManager::Instance()->GetDeviceContext()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer
	DirectXManager::Instance()->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST);

	return true;
}

// Game Settings
void Ocean::ToggleTime(bool NightTimeMode)
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
float Ocean::GetTessellation()
{
	return Tessellation_;
}

float Ocean::GetWaterHeight()
{
	return WaterHeight_;
}

float Ocean::GetWaveHeight()
{
	return WaveHeight_;
}

D3DXVECTOR2 Ocean::GetNormalMapTiling()
{
	return NormalMapTiling_;
}

float Ocean::GetWaterTranslation()
{
	return WaterTranslation_;
}

float Ocean::GetReflectRefractScale()
{
	return ReflectRefractScale_;
}

D3DXVECTOR4 Ocean::GetRefractionTint()
{
	return RefractionTint_;
}

float Ocean::GetSpecularShininess()
{
	return WaterShininess_;
}