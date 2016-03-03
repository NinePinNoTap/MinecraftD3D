#include "Model.h"

Model::Model()
{
	Mesh_ = 0;
}

Model::~Model()
{

}

void Model::SetMesh(ModelMesh* mesh)
{
	Mesh_ = mesh;
}

ModelMesh* Model::GetMesh()
{
	if (!Mesh_)
	{
		Mesh_ = new ModelMesh;
	}

	return Mesh_;
}