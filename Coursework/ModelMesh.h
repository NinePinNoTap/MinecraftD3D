#pragma once

#include "Mesh.h"

class ModelMesh : public Mesh
{
public:
	ModelMesh();
	~ModelMesh();

	bool LoadModel(std::string Directory, const char* ModelFilename);
	bool LoadModel(char* ModelFilename, wstring TextureFilename = L"");

private:
	bool LoadModelFromObj(string Directory, const char* ModelFilename);
	bool LoadModelFromTxt(char* ModelFilename);
};