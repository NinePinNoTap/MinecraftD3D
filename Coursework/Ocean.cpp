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

	newMaterial->SetFloat("WaterHeight", waterResolution.depth);
	newMaterial->SetVector2("NormalMapTiling", D3DXVECTOR2(0.01f, 0.02f));
	newMaterial->SetFloat("WaterTranslation", 0.0f);
	newMaterial->SetFloat("ReflectRefractScale", 0.03f);
	newMaterial->SetVector4("RefractionTint", D3DXVECTOR4(0.0f, 0.8f, 1.0f, 1.0f));
	newMaterial->SetFloat("WaveHeight", 1.5f);
	newMaterial->SetFloat("TessellationAmount", 58.0f);

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

	Frame_ = 0.0f;
	WaveSpeed_ = 0.025f;
	WaterTranslation_ = 0.0f;

	IsReflective_ = RenderMode::Off;
	UseCulling_ = RenderMode::Off;
	UseDepth_ = RenderMode::On;
	IsPostProcessed_ = RenderMode::On;
	BlendMode_ = BlendMode::NoBlending;

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

	// Update Material
	Model_->GetMaterial()->SetFloat("Frame", Frame_);
	Model_->GetMaterial()->SetFloat("WaterTranslation", WaterTranslation_);

	return true;
}

bool Ocean::Render()
{
	if (!IsActive_ || !Shader_ || !Model_)
		return true;

	// Define how we want the model to be rendered
	SetRenderModes();

	// Define how we want to see the model
	Shader_->SetRenderMode(ProjectionMode::Perspective, ViewMode::View);

	// Render to Render Texture
	if (IsPostProcessed_ == RenderMode::On)
	{
		// Get and set render texture
		Texture* renderTexture;
		AssetManager::Instance()->LoadTexture(&renderTexture, "RenderTexture");
		renderTexture->SetRenderTarget();

		// Render the model
		RenderMeshes();

		// Reset Render Target
		DirectXManager::Instance()->SetBackBufferRenderTarget();
		DirectXManager::Instance()->ResetViewport();

		// Clean Up
		renderTexture = 0;
	}

	// Render Mesh
	RenderMeshes();

	// Reset Pipeline Settings
	ResetRenderModes();

	return true;
}

bool Ocean::RenderMeshes()
{
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

	return true;
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
		WaveSpeed_ = 0.06f;
		Model_->GetMaterial()->SetFloat("WaveHeight", 2.0f);
	}
	else
	{
		Model_->GetMaterial()->SetFloat("WaveHeight", 1.5f);
		WaveSpeed_ = 0.025f;
	}
}