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

void Model::Shutdown()
{
	// Shutdown meshes
	if (Meshes_.size() > 0)
	{
		for (int i = 0; i < Meshes_.size(); i++)
		{
			if (Meshes_[i])
			{
				Meshes_[i]->Shutdown();
				delete Meshes_[i];
				Meshes_[i] = 0;
			}
		}

		Meshes_.clear();
	}

	// Shutdown materials
	if (Materials_.size() > 0)
	{
		for (int i = 0; i < Materials_.size(); i++)
		{
			if (Materials_[i])
			{
				Materials_[i]->Shutdown();
				delete Materials_[i];
				Materials_[i] = 0;
			}
		}

		Materials_.clear();
	}
}

void Model::AddMesh(Mesh3D* mesh)
{
	Meshes_.push_back(mesh);
}

void Model::AddMaterial(Material* material)
{
	Materials_.push_back(material);
}

void Model::UpdateMesh(int index, Mesh3D* mesh)
{
	// Check if we can access the element
	if (index >= Meshes_.size())
	{
		Meshes_.push_back(mesh);
		return;
	}

	// Delete Old Mesh
	Meshes_[index]->Shutdown();
	delete Meshes_[index];
	Meshes_[index] = 0;

	// Add New Mesh
	Meshes_[index] = mesh;
}

void Model::UpdateMaterial(int index, Material* material)
{
	// Make sure we have materials to update
	if (Materials_.empty())
	{
		// Add material
		Materials_.push_back(material);
		return;
	}

	// Make sure the index isn't out of range
	if (index >= Materials_.size())
	{
		// Add material
		Materials_.push_back(material);
		return;
	}

	// Delete Old Mesh
	delete Materials_[index];
	Materials_[index] = 0;

	// Add New Mesh
	Materials_[index] = material;
}

void Model::ClearMeshes()
{
	// Clean Up
	for (int i = 0; i < Meshes_.size(); i++)
	{
		Meshes_[i]->Shutdown();
		delete Meshes_[i];
		Meshes_[i] = 0;
	}

	Meshes_.clear();
}

void Model::ClearMaterials()
{
	// Clean Up
	for (int i = 0; i < Meshes_.size(); i++)
	{
		delete Materials_[i];
		Materials_[i] = 0;
	}

	Materials_.clear();
}

Mesh3D* Model::GetMesh(int index)
{
	return Meshes_[index];
}

Material* Model::GetMaterial(int index)
{
	Clamp(index, 0, Materials_.size() - 1);

	return Materials_[index];
}

int Model::GetMeshCount()
{
	return Meshes_.size();
}