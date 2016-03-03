#include "GameObject.h"

GameObject::GameObject()
{
	// Initialise the pointers
	Mesh_ = 0;

	// Initialise default transforms
	Transform_ = 0;

	// Initialise default flags
	IsActive_ = true;
}

GameObject::~GameObject()
{
}

// Shutdown
void GameObject::Shutdown()
{
	// Release the model
	if (Mesh_)
	{
		Mesh_->Shutdown();
		delete Mesh_;
		Mesh_ = 0;
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

// Rendering
void GameObject::SetReflectable(bool Flag)
{
	IsReflectable_ = Flag;
}

void GameObject::SetActive(bool Flag)
{
	IsActive_ = Flag;
}

// Model Getters
Mesh* GameObject::GetMesh()
{
	// Check if we have a mesh to return
	if (!Mesh_)
	{
		// We don't so create it
		Mesh_ = new Mesh;
	}

	return Mesh_;
}

// Material Getter
Material* GameObject::GetMaterial()
{
	return Mesh_->GetMaterial();
}

// Transform Getter
Transform* GameObject::GetTransform()
{
	// Check if we have a transform to return
	if (!Transform_)
	{
		// We don't so create it
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