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

	void terminate();

	void addMesh(Mesh3D* mesh);
	void addMaterial(Material* material);

	void updateMesh(int index, Mesh3D* mesh);
	void updateMaterial(int index, Material* material);

	void clearMeshes();
	void clearMaterials();

	Mesh3D* getMesh(int index = 0);
	Material* getMaterial(int index = 0);

	vector<Mesh3D*> getAllMeshes() { return m_meshes; }
	vector<Material*> getAllMaterials() { return m_materials; }

	int getMeshCount();

protected:
	std::vector<Mesh3D*> m_meshes;
	std::vector<Material*> m_materials;
};