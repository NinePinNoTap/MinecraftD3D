#include "OBJLoader.h"

OBJLoader::OBJLoader()
{
}


OBJLoader::~OBJLoader()
{
}

bool OBJLoader::LoadModel(const char* ModelFilename, Model& model)
{
	ifstream fin;
	char linebuffer[256];
	vector<D3DXVECTOR3> vertexPositions;
	vector<D3DXVECTOR2> vertexTextureCoords;
	vector<D3DXVECTOR3> faceNormals;
	vector<D3DXVECTOR3> faceIndices;
	vector<wstring> textureFilenames;
	vector<int> textureIDs;
	D3DXVECTOR3 tempVertex;
	D3DXVECTOR2 tempTexCoord;
	D3DXVECTOR3 TempFace[3];
	float vertexID, textureID, normalID;
	int indexCount, vertexCount;
	VertexData* objMesh;
	unsigned long* Indices;
	bool Result;

	//===================
	// Load the OBJ File
	//===================

	// Open the file
	fin.open(MODEL_DIR + ModelFilename);

	// Check if file couldnt be opened
	if (fin.fail()) { return false; }

	// Loop through the file lines 
	while (!fin.eof())
	{
		// Get the current line
		fin.getline(linebuffer, 256);

		if (string(linebuffer).find("v ") == 0) // Line defines a tempVertex position
		{
			//Read the tempVertex line and push the data into the vector
			sscanf_s(string(linebuffer).c_str(), "v %f %f %f", &tempVertex.x, &tempVertex.y, &tempVertex.z);
			vertexPositions.push_back(tempVertex);
		}
		else if (string(linebuffer).find("vt ") == 0) // Line defines a tempVertex texture
		{
			//Read the texture coord line and push the data into the vector
			sscanf_s(string(linebuffer).c_str(), "vt %f %f", &tempTexCoord.x, &tempTexCoord.y);
			vertexTextureCoords.push_back(tempTexCoord);
		}
		else if (string(linebuffer).find("vn ") == 0) // Line defines a tempVertex normal
		{
			//Read the normal line and push the data into the vector
			sscanf_s(string(linebuffer).c_str(), "vn %f %f %f", &tempVertex.x, &tempVertex.y, &tempVertex.z);
			faceNormals.push_back(tempVertex);
		}
		else if (string(linebuffer).find("f ") == 0) // Line defines a triangle (faceIndices)
		{
			//Read the face line
			sscanf_s(string(linebuffer).c_str(), "f %f/%f/%f %f/%f/%f %f/%f/%f",
				&TempFace[0].x, &TempFace[0].y, &TempFace[0].z,
				&TempFace[1].x, &TempFace[1].y, &TempFace[1].z,
				&TempFace[2].x, &TempFace[2].y, &TempFace[2].z);

			// Add the faceIndices into the vector
			faceIndices.push_back(TempFace[0]);
			faceIndices.push_back(TempFace[1]);
			faceIndices.push_back(TempFace[2]);

			// Add the texture IDs for the three faceIndices
			textureIDs.push_back(textureFilenames.size() - 1);
			textureIDs.push_back(textureFilenames.size() - 1);
			textureIDs.push_back(textureFilenames.size() - 1);
		}
		else if (string(linebuffer).find("usemtl ") == 0) // Line is a texture filename 
		{
			// Split the string up as we only need the second part (actual file name)
			istringstream ss(linebuffer);
			istream_iterator<string> begin(ss), end;
			vector<string> stringParts(begin, end);

			// Compile the full filename
			string filename = TEXTURE_DIR + stringParts[1] + ".dds";

			// Check if the file is already on the list
			if (std::find(textureFilenames.begin(), textureFilenames.end(), wstring(filename.begin(), filename.end())) == textureFilenames.end())
			{
				// Add the filename to the vector
				textureFilenames.push_back(wstring(filename.begin(), filename.end()));
			}
		}
	}

	// Close the file
	fin.close();

	//====================
	// Construct the Mesh3D
	//====================

	// Create the model using the vertex count that was read in
	indexCount = vertexCount = faceIndices.size();
	objMesh = new VertexData[vertexCount];
	if (!objMesh) { return false; }

	Indices = new unsigned long[indexCount];
	if (!Indices) { return false; }

	for (unsigned int Index = 0; Index < faceIndices.size(); Index++)
	{
		// Read in Face Data
		vertexID = faceIndices[Index].x - 1;
		textureID = faceIndices[Index].y - 1;
		normalID = faceIndices[Index].z - 1;

		// Grab data from the vectors and apply them to the model
		objMesh[Index].position.x = vertexPositions[vertexID].x;
		objMesh[Index].position.y = vertexPositions[vertexID].y;
		objMesh[Index].position.z = vertexPositions[vertexID].z * -1; //Flip for RH WindowManager

		objMesh[Index].texture.x = vertexTextureCoords[textureID].x;
		objMesh[Index].texture.y = 1 - vertexTextureCoords[textureID].y; //Flip for RH WindowManager
		objMesh[Index].texture.z = textureIDs[Index];

		objMesh[Index].normal.x = faceNormals[normalID].x;
		objMesh[Index].normal.y = faceNormals[normalID].y;
		objMesh[Index].normal.z = faceNormals[normalID].z * -1; //Flip for RH WindowManager

		Indices[Index] = Index;
	}

	//================
	// Finalise Model
	//================

	model.GetMesh()->SetMesh(objMesh, Indices);
	model.GetMesh()->SetIndexCount(indexCount);
	model.GetMesh()->SetVertexCount(vertexCount);
	model.GetMaterial()->SetTextureArray(textureFilenames);

	//==========
	// Clean up
	//==========

	vertexPositions.clear();
	vertexTextureCoords.clear();
	faceNormals.clear();
	faceIndices.clear();
	textureFilenames.clear();
	textureIDs.clear();

	return model.GetMesh()->Build();
}