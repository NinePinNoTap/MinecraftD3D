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
	IsReflectable_ = false;
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
	IsReflectable_ = false;
	IsActive_ = true;
}

// Shutdown
void GameObject::Shutdown()
{
	// Release the model
	if (Model_)
	{
		Model_->Shutdown();
		delete Model_;
		Model_ = 0;
	}

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

// Setters
void GameObject::SetShader(string shaderName)
{
	Shader_ = ShaderManager::Instance()->GetShader(shaderName);

	if (!Shader_)
	{
		OutputToDebug("Could not set shader");
	}
}

void GameObject::SetReflectable(bool Flag)
{
	IsReflectable_ = Flag;
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

bool GameObject::IsReflectable()
{
	return IsReflectable_;
}

bool GameObject::IsActive()
{
	return IsActive_;
}

float GameObject::GetFrame()
{
	return Frame_;
}