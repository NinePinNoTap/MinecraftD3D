#pragma once
#include "GameObject.h"
#include "PrimitiveMesh.h"

class Primitive : public GameObject
{
public:
	Primitive();
	~Primitive();

	PrimitiveMesh* GetMesh();

protected:
	PrimitiveMesh* Mesh_;
};

