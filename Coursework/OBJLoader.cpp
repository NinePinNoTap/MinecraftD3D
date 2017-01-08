#include "OBJloader.h"

OBJloader::OBJloader()
{
}


OBJloader::~OBJloader()
{
}

bool OBJloader::loadModel(const char* modelFilename, Model& model)
{
	ifstream fin;
	char linebuffer[256];

	// Current Model Data
	vector<D3DXVECTOR3> vertexData;
	vector<D3DXVECTOR2> textureData;
	vector<D3DXVECTOR3> normalData;
	vector<D3DXVECTOR3> faceData;

	// Temporary file reading
	D3DXVECTOR3 tempVertex;
	D3DXVECTOR2 tempTextureUV;
	D3DXVECTOR3 tempFace[3];
	std::string textureFilename;

	//===================
	// onload the OBJ File
	//===================

	// Open the file
	fin.open(Directory::Model + modelFilename);

	// Check if file couldnt be opened
	if (fin.fail())
	{
		return false;
	}
	
	// Loop through the file lines 
	while (!fin.eof())
	{
		// get the current line
		fin.getline(linebuffer, 256);

		if (string(linebuffer).find("v ") == 0) 
		{
			//Read the tempVertex line and push the data into the vector
			sscanf_s(string(linebuffer).c_str(), "v %f %f %f", &tempVertex.x, &tempVertex.y, &tempVertex.z);
			vertexData.push_back(tempVertex);
		}
		else if (string(linebuffer).find("vt ") == 0) 
		{
			//Read the texture coord line and push the data into the vector
			sscanf_s(string(linebuffer).c_str(), "vt %f %f", &tempTextureUV.x, &tempTextureUV.y);
			textureData.push_back(tempTextureUV);
		}
		else if (string(linebuffer).find("vn ") == 0) 
		{
			//Read the normal line and push the data into the vector
			sscanf_s(string(linebuffer).c_str(), "vn %f %f %f", &tempVertex.x, &tempVertex.y, &tempVertex.z);
			normalData.push_back(tempVertex);
		}
		else if (string(linebuffer).find("f ") == 0) 
		{
			//Read the face line
			sscanf_s(string(linebuffer).c_str(), "f %f/%f/%f %f/%f/%f %f/%f/%f",
				&tempFace[0].x, &tempFace[0].y, &tempFace[0].z,
				&tempFace[1].x, &tempFace[1].y, &tempFace[1].z,
				&tempFace[2].x, &tempFace[2].y, &tempFace[2].z);

			// Add the faceData into the vector
			faceData.push_back(tempFace[2]);
			faceData.push_back(tempFace[1]);
			faceData.push_back(tempFace[0]);
		}
		else if (string(linebuffer).find("usemtl") == 0) 
		{
			// Check if we have face data to process
			if (faceData.size() > 0)
			{
				m_result = buildModel(&model, textureFilename, vertexData, textureData, normalData, faceData);
				if (!m_result)
				{
					return false;
				}

				// Clear Data
				faceData.clear();
			}


			// Read in the texture name
			std::string str(linebuffer);
			std::vector<std::string> tokens;
			std::istringstream split(str);
			for (std::string temp; std::getline(split, temp, ' '); tokens.push_back(temp));

			textureFilename = tokens[1];
		}
	}

	// Check for any remaining models to be created
	if (faceData.size() > 0)
	{
		m_result = buildModel(&model, textureFilename, vertexData, textureData, normalData, faceData);
		if (!m_result)
		{
			return false;
		}
	}

	//==========
	// Clean Up
	//==========

	vertexData.clear();
	normalData.clear();
	textureData.clear();
	faceData.clear();

	// Close the file
	fin.close();

	return true;
}

bool OBJloader::buildModel(Model* model, std::string textureFilename, vector<D3DXVECTOR3> vertexData, vector<D3DXVECTOR2> textureData, vector<D3DXVECTOR3> normalData, vector<D3DXVECTOR3> faceData)
{
	float vertexID, textureID, normalID;
	int indexCount, vertexCount;

	//====================
	// Construct the Mesh
	//====================

	VertexData* ObjMesh;
	unsigned long* Indices;

	// Store how many indices and vertices we will have
	indexCount = vertexCount = faceData.size();

	// Construct data storage
	ObjMesh = new VertexData[vertexCount];
	if (!ObjMesh)
	{
		return false;
	}

	Indices = new unsigned long[indexCount];
	if (!Indices)
	{
		return false;
	}

	// Loop through face data
	for (unsigned int Index = 0; Index < faceData.size(); Index++)
	{
		// Read in Face Data
		vertexID = faceData[Index].x - 1;
		textureID = faceData[Index].y - 1;
		normalID = faceData[Index].z - 1;

		// Grab data from the vectors and apply them to the model
		ObjMesh[Index].position.x = vertexData[vertexID].x;
		ObjMesh[Index].position.y = vertexData[vertexID].y;
		ObjMesh[Index].position.z = vertexData[vertexID].z * -1; //Flip for RH system

		ObjMesh[Index].texture.x = textureData[textureID].x;
		ObjMesh[Index].texture.y = 1 - textureData[textureID].y; //Flip for RH system

		ObjMesh[Index].normal.x = normalData[normalID].x;
		ObjMesh[Index].normal.y = normalData[normalID].y;
		ObjMesh[Index].normal.z = normalData[normalID].z * -1; //Flip for RH system

		Indices[Index] = Index;
	}

	//=============
	// create Mesh
	//=============

	// create Mesh
	Mesh3D* newMesh = new Mesh3D;
	newMesh->setIndexCount(indexCount);
	newMesh->setVertexCount(vertexCount);
	newMesh->setMesh(ObjMesh, Indices);
	m_result = newMesh->build();
	if (!m_result)
	{
		return false;
	}

	// Pass to model
	model->addMesh(newMesh);

	//=================
	// create Material
	//=================

	if (textureFilename.length() > 0)
	{
		// Add texture extension
		textureFilename += ".dds";

		// create Material
		Material* newMaterial = new Material;
		m_result = newMaterial->setBaseTexture(textureFilename);
		if (!m_result)
		{
			return false;
		}

		model->addMaterial(newMaterial);
	}

	return true;
}