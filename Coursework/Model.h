#pragma once

#include "Mesh3D.h"
#include "Material.h"

class Model
{
public:
	Model();
	~Model();

	bool Initialise();
	void Shutdown();

	Mesh3D* GetMesh();
	Material* GetMaterial();

protected:
	Mesh3D* Mesh_;
	Material* Material_;
};

