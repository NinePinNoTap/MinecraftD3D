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

	bool LoadModel(const char* modelFilename, Model& model);

private:
	bool BuildModel(Model* model, std::string textureFilename, vector<D3DXVECTOR3> vertexData, 
		vector<D3DXVECTOR2> textureData, vector<D3DXVECTOR3> normalData, vector<D3DXVECTOR3> faceData);
	bool Result_;
};