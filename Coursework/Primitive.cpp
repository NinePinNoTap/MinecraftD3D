#include "Primitive.h"

Primitive::Primitive()
{
}


Primitive::~Primitive()
{
}

PrimitiveMesh* Primitive::GetMesh()
{
	if (!Mesh_)
	{
		Mesh_ = new PrimitiveMesh;
	}

	return Mesh_;
}