#pragma once

#include <d3dx11.h>
#include <d3dx10math.h>

#include "Constants.h"
#include "Mesh.h"
#include "Transform.h"
#include "Utilities.h"

class GameObject
{
public:
	GameObject();
	~GameObject();

	// Shutdown
	virtual void Shutdown();

	// Frames
	virtual bool Frame();

	// Rendering
	void SetReflectable(bool Flag);
	void SetActive(bool Flag);

	// Getters
	virtual Mesh* GetMesh();
	virtual Material* GetMaterial();
	Transform* GetTransform();

	bool IsReflectable();
	bool IsActive();
	float GetFrame();

protected:
	// Model
	Mesh* Mesh_;

	// Transform
	Transform* Transform_;

	// Animations
	float Frame_;
	
	// Flags
	bool Result_;
	bool IsReflectable_;
	bool IsActive_;
};


