#include "GameObject.h"

GameObject::GameObject()
{
	// Initialise the pointers
	Model_ = 0;

	// Initialise default transforms
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

	Model_ = new Model;
	if (!Model_)
	{
		return false;
	}

	// Load Model
	Result_ = objLoader.LoadModel(filename, *Model_);
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
	// Nothing to do here

	return true;
}

bool GameObject::Render()
{
	unsigned int stride;
	unsigned int offset;
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
	int meshCount;

	// Set vertex buffer stride and offset
	stride = sizeof(VertexData);
	offset = 0;

	// Get number of meshes
	meshCount = Model_->GetMeshCount();

	// Loop through and render the mesh parts
	//for (int i = 0; i < meshCount; i++)
	//{
		// Get Mesh Buffers
		vertexBuffer = Model_->GetMesh()->GetVertexBuffer();
		indexBuffer = Model_->GetMesh()->GetIndexBuffer();

		// Set the vertex buffer to active in the InputManager assembler so it can be rendered
		DirectXManager::Instance()->GetDeviceContext()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

		// Set the index buffer to active in the InputManager assembler so it can be rendered
		DirectXManager::Instance()->GetDeviceContext()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

		// Set the type of primitive that should be rendered from this vertex buffer
		DirectXManager::Instance()->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//}

	return true;
}

// Rendering
void GameObject::SetReflectable(bool Flag)
{
	IsReflectable_ = Flag;
}

void GameObject::SetActive(bool Flag)
{
	IsActive_ = Flag;
}

// Setters
void GameObject::SetModel(Model* model)
{
	Model_ = model;
}

// Model Getters
Model* GameObject::GetModel()
{
	return Model_;
}

// Transform Getter
Transform* GameObject::GetTransform()
{	
	if (!Transform_)
	{
		Transform_ = new Transform;
	}
	return Transform_;
}

// Rendering Getter
bool GameObject::IsReflectable()
{
	return IsReflectable_;
}

bool GameObject::IsActive()
{
	return IsActive_;
}

// Animation Getter
float GameObject::GetFrame()
{
	return Frame_;
}