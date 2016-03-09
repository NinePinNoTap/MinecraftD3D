#include "Model.h"

Model::Model()
{
	Material_ = 0;
	Mesh_ = 0;
}

Model::~Model()
{

}

bool Model::Initialise()
{
	// Create Mesh3D
	Mesh_ = new Mesh3D;
	if (!Mesh_)
	{
		return false;
	}

	// Create Material
	Material_ = new Material;
	if (!Material_)
	{
		return false;
	}

	return true;
}

void Model::Shutdown()
{
	// Shutdown Mesh3D
	if (Mesh_)
	{
		Mesh_->Shutdown();
		delete Mesh_;
		Mesh_ = 0;
	}
	
	// Shutdown Material
	if (Material_)
	{
		Material_->Shutdown();
		delete Material_;
		Material_ = 0;
	}
}


Mesh3D* Model::GetMesh()
{
	return Mesh_;
}

Material* Model::GetMaterial()
{
	return Material_;
}