#pragma once

#include <fstream>
#include <string>
#include <sstream>
#include <iterator>
#include <vector>

#include "Model.h"

using namespace Config;

class OBJloader
{
public:
	OBJloader();
	~OBJloader();

	bool loadModel(const char* modelFilename, Model& model);

private:
	bool buildModel(Model* model, std::string textureFilename, vector<D3DXVECTOR3> vertexData, 
		vector<D3DXVECTOR2> textureData, vector<D3DXVECTOR3> normalData, vector<D3DXVECTOR3> faceData);
	bool m_result;
};