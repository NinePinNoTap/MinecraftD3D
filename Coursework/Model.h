#pragma once

#include <vector>

#include "Mesh3D.h"
#include "Material.h"

class Model
{
public:
	Model();
	Model(const Model& model);
	~Model();

	void Shutdown();

	void AddMesh(Mesh3D* mesh);
	void AddMaterial(Material* material);

	void UpdateMesh(int index, Mesh3D* mesh);
	void UpdateMaterial(int index, Material* material);

	void ClearMeshes();
	void ClearMaterials();

	Mesh3D* GetMesh(int index = 0);
	Material* GetMaterial(int index = 0);

	vector<Mesh3D*> GetAllMeshes() { return Meshes_; }
	vector<Material*> GetAllMaterials() { return Materials_; }

	int GetMeshCount();

protected:
	std::vector<Mesh3D*> Meshes_;
	std::vector<Material*> Materials_;
};

