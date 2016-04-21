#include "GameObject.h"
#include "ShaderManager.h"

GameObject::GameObject()
{
	// Initialise the pointers
	Model_ = 0;
	Shader_ = 0;
	Transform_ = 0;

	// Initialise default flags
	IsActive_ = true;
}

GameObject::~GameObject()
{
}

bool GameObject::Initialise()
{
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
	UseDepth_ = RenderMode::On;
	IsPostProcessed_ = RenderMode::On;
	BlendMode_ = BlendMode::NoBlending;

	IsActive_ = true;
}

bool GameObject::Initialise(const char* filename)
{
	OBJLoader objLoader;

	//==============
	// Create Model
	//==============

	AssetManager::Instance()->LoadModel(&Model_, filename);
	if (!Model_)
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
	UseDepth_ = RenderMode::On;
	IsPostProcessed_ = RenderMode::On;
	BlendMode_ = BlendMode::NoBlending;

	IsActive_ = true;
}

// Shutdown
void GameObject::Shutdown()
{
	// Delete the transform
	if (Transform_)
	{
		delete Transform_;
		Transform_ = 0;
	}
}

// Frame
bool GameObject::Frame()
{
	return true;
}

bool GameObject::Render()
{
	if (!IsActive_ || !Shader_ || !Model_)
		return true;

	// Define how we want the model to be rendered
	SetRenderModes();

	// Render Reflection
	if (IsReflective_ == RenderMode::On)
	{
		// Define how we want to see the model
		Shader_->SetRenderMode(ProjectionMode::Perspective, ViewMode::Reflection);

		// Get and set the reflection texture
		Texture* reflectionTexture;
		AssetManager::Instance()->LoadTexture(&reflectionTexture, "ReflectionTexture");
		reflectionTexture->SetRenderTarget();

		// Render Model
		RenderMeshes();

		DirectXManager::Instance()->SetBackBufferRenderTarget();
		DirectXManager::Instance()->ResetViewport();

		// Clean Up
		reflectionTexture = 0;
	}

	// Define how we want to see the model
	Shader_->SetRenderMode(ProjectionMode::Perspective, ViewMode::View);
	
	if (IsPostProcessed_ == RenderMode::On)
	{
		// Get and set the reflection texture
		Texture* renderTexture;
		AssetManager::Instance()->LoadTexture(&renderTexture, "RenderTexture");
		renderTexture->SetRenderTarget();

		RenderMeshes();

		DirectXManager::Instance()->SetBackBufferRenderTarget();
		DirectXManager::Instance()->ResetViewport();

		// Clean Up
		renderTexture = 0;
	}

	// Render Mesh
	RenderMeshes();

	// Reset Pipeline Settings
	ResetRenderModes();
}

bool GameObject::RenderMeshes()
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

bool GameObject::SendModelToPipeline(Mesh3D* objMesh)
{
	unsigned int stride;
	unsigned int offset;
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;

	// Set vertex buffer stride and offset
	stride = sizeof(VertexData);
	offset = 0;

	// Get mesh buffers
	vertexBuffer = objMesh->GetVertexBuffer();
	indexBuffer = objMesh->GetIndexBuffer();

	// Set the vertex buffer to active in the input assembler so it can be rendered
	DirectXManager::Instance()->GetDeviceContext()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered
	DirectXManager::Instance()->GetDeviceContext()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer
	DirectXManager::Instance()->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	vertexBuffer = 0;
	indexBuffer = 0;

	return true;
}

void GameObject::SetRenderModes()
{
	// Backface Culling
	if (UseCulling_ == RenderMode::On)
	{
		DirectXManager::Instance()->SetBackfaceCullingOn(true);
	}
	else
	{
		DirectXManager::Instance()->SetBackfaceCullingOn(false);
	}

	// Z Buffer
	if (UseDepth_ == RenderMode::On)
	{
		DirectXManager::Instance()->SetDepthBufferOn(true);
	}
	else
	{
		DirectXManager::Instance()->SetDepthBufferOn(false);
	}
	
	// Blending
	switch (BlendMode_)
	{
		case BlendMode::NoBlending:
			DirectXManager::Instance()->SetAlphaBlendingOn(false);
			break;

		case BlendMode::AlphaBlending:
			DirectXManager::Instance()->SetAlphaBlendingOn(true);
			break;

		case BlendMode::CloudBlending:
			DirectXManager::Instance()->SetCloudBlendingOn();
			break;

		case BlendMode::FireBlending:
			DirectXManager::Instance()->SetFireBlendingOn();
			break;
	}
}

void GameObject::ResetRenderModes()
{
	DirectXManager::Instance()->SetAlphaBlendingOn(false);
	DirectXManager::Instance()->SetDepthBufferOn(true);
	DirectXManager::Instance()->SetBackfaceCullingOn(false);
}

// Setters
void GameObject::SetShader(string shaderName)
{
	Shader_ = ShaderManager::Instance()->GetShader(shaderName);

	if (!Shader_)
	{
		OutputToDebug("Could not set shader");
	}
}

void GameObject::SetRenderModes(RenderMode canReflect, RenderMode useCulling, RenderMode useDepth, BlendMode blendMode)
{
	IsReflective_ = canReflect;
	UseCulling_ = useCulling;
	UseDepth_ = useDepth;
	BlendMode_ = blendMode;
}

void GameObject::SetActive(bool Flag)
{
	IsActive_ = Flag;
}

void GameObject::SetModel(Model* model)
{
	Model_ = model;
}

// Getters
Model* GameObject::GetModel()
{
	return Model_;
}

Transform* GameObject::GetTransform()
{	
	return Transform_;
}

bool GameObject::IsActive()
{
	return IsActive_;
}

float GameObject::GetFrame()
{
	return Frame_;
}