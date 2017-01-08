#include "Model.h"

Model::Model()
{
}

Model::Model(const Model& model)
{
	*this = model; // copy the value
}

Model::~Model()
{
}

void Model::terminate()
{
	// terminate meshes
	if (m_meshes.size() > 0)
	{
		for (unsigned int i = 0; i < m_meshes.size(); i++)
		{
			if (m_meshes[i])
			{
				m_meshes[i]->terminate();
				delete m_meshes[i];
				m_meshes[i] = 0;
			}
		}

		m_meshes.clear();
	}

	// terminate materials
	if (m_materials.size() > 0)
	{
		for (unsigned int i = 0; i < m_materials.size(); i++)
		{
			if (m_materials[i])
			{
				m_materials[i]->terminate();
				delete m_materials[i];
				m_materials[i] = 0;
			}
		}

		m_materials.clear();
	}
}

void Model::addMesh(Mesh3D* mesh)
{
	m_meshes.push_back(mesh);
}

void Model::addMaterial(Material* material)
{
	m_materials.push_back(material);
}

void Model::updateMesh(int index, Mesh3D* mesh)
{
	// Check if we can access the element
	if (index >= m_meshes.size())
	{
		m_meshes.push_back(mesh);
		return;
	}

	// Delete Old Mesh
	m_meshes[index]->terminate();
	m_meshes[index] = 0;

	// Add New Mesh
	m_meshes[index] = mesh;
}

void Model::updateMaterial(int index, Material* material)
{
	// Make sure we have materials to update
	if (m_materials.empty())
	{
		// Add material
		m_materials.push_back(material);
		return;
	}

	// Make sure the index isn't out of range
	if (index >= m_materials.size())
	{
		// Add material
		m_materials.push_back(material);
		return;
	}

	// Delete Old Mesh
	delete m_materials[index];
	m_materials[index] = 0;

	// Add New Mesh
	m_materials[index] = material;
}

void Model::clearMeshes()
{
	// Clean Up
	for (unsigned int i = 0; i < m_meshes.size(); i++)
	{
		m_meshes[i]->terminate();
		delete m_meshes[i];
		m_meshes[i] = 0;
	}

	m_meshes.clear();
}

void Model::clearMaterials()
{
	// Clean Up
	for (unsigned int i = 0; i < m_meshes.size(); i++)
	{
		delete m_materials[i];
		m_materials[i] = 0;
	}

	m_materials.clear();
}

Mesh3D* Model::getMesh(int index)
{
	return m_meshes[index];
}

Material* Model::getMaterial(int index)
{
	clamp(index, 0, m_materials.size() - 1);

	return m_materials[index];
}

int Model::getMeshCount()
{
	return m_meshes.size();
}