#pragma once

#include <fstream>
#include <string>
#include <sstream>
#include <iterator>
#include <vector>

#include "Constants.h"
#include "Model.h"

class OBJLoader
{
public:
	OBJLoader();
	~OBJLoader();

	bool LoadModel(const char* ModelFilename, Model& model);

private:

	bool LoadMaterials(vector<wstring> TextureFilenames)
	{
		// TODO
	}
};