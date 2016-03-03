#pragma once

#include "GameObject.h"
#include "ModelMesh.h"

class Model : public GameObject
{
public:
	Model();
	~Model();

	void SetMesh(ModelMesh* mesh);
	ModelMesh* GetMesh();

protected:
	ModelMesh* Mesh_;
};

