#pragma once

#include <vector>

#include "Mesh3D.h"
#include "Material.h"

class Model
{
public:
	Model();
	~Model();

	void Shutdown();

	void AddMesh(Mesh3D* mesh);
	void AddMaterial(Material* material);

	void UpdateMesh(int index, Mesh3D* mesh);
	void UpdateMaterial(int index, Material* material);

	Mesh3D* GetMesh(int index);
	Material* GetMaterial(int index);

	int GetMeshCount();

protected:
	std::vector<Mesh3D*> Meshes_;
	std::vector<Material*> Materials_;
};

