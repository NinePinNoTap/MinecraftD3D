#include "Model.h"

Model::Model()
{
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
			Meshes_[i]->Shutdown();
			delete Meshes_[i];
			Meshes_[i] = 0;
		}

		Meshes_.clear();
	}

	// Shutdown materials
	if (Materials_.size() > 0)
	{
		for (int i = 0; i < Materials_.size(); i++)
		{
			Materials_[i]->Shutdown();
			delete Materials_[i];
			Materials_[i] = 0;
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

Mesh3D* Model::GetMesh(int index)
{
	return Meshes_[index];
}

Material* Model::GetMaterial(int index)
{
	return Materials_[index];
}

int Model::GetMeshCount()
{
	return Meshes_.size();
}