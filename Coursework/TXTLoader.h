#pragma once

#include <d3dx11.h>
#include <d3dx10math.h>

#include <algorithm>
#include <fstream>
#include <string>
#include <sstream>
#include <iterator>
#include <vector>

#include "Model.h"

class TXTLoader
{
public:
	TXTLoader();
	~TXTLoader();

	bool LoadModel(const char* filename, Model& model)
	{
		ifstream fin;
		char input;
		int i, vertexCount, indexCount;
		VertexData* mesh;
		unsigned long* Indices;

		//===============
		// Read TXT File
		//===============

		// Open the model file.
		fin.open(filename);

		// If it could not open the file then exit
		if (fin.fail()) { return false; }

		// Read up to the value of vertex count
		fin.get(input);
		while (input != ':')
		{
			fin.get(input);
		}

		// Read in the vertex count
		fin >> vertexCount;

		// Set the number of indices to be the same as the vertex count
		indexCount = vertexCount;

		// Create the model using the vertex count that was read in
		mesh = new VertexData[vertexCount];
		if (!mesh)
		{
			return false;
		}

		Indices = new unsigned long[indexCount];
		if (!Indices)
		{
			return false;
		}

		// Read up to the beginning of the data
		fin.get(input);
		while (input != ':')
		{
			fin.get(input);
		}
		fin.get(input);
		fin.get(input);

		//==========================
		// Construct Mesh3D from File
		//==========================

		// Read in the vertex data.
		for (i = 0; i<vertexCount; i++)
		{
			fin >> mesh[i].position.x	>> mesh[i].position.y	>> mesh[i].position.z;
			fin >> mesh[i].texture.x	>> mesh[i].texture.y;
			fin >> mesh[i].normal.x		>> mesh[i].normal.y		>> mesh[i].normal.z;

			mesh[i].texture.z = 0;
			mesh[i].binormal = D3DXVECTOR3(0, 0, 0);
			mesh[i].tangent = D3DXVECTOR3(0, 0, 0);

			Indices[i] = i;
		}

		// Close the model file.
		fin.close();

		//================
		// Finalise Model
		//================

		model.GetMesh()->SetMesh(mesh, Indices);
		model.GetMesh()->SetVertexCount(vertexCount);
		model.GetMesh()->SetIndexCount(indexCount);
		Result_ = model.GetMesh()->Build();
		if (!Result_)
		{
			return false;
		}

		return true;
	}

private:
	bool Result_;
};