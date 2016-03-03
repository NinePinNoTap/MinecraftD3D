#include "ModelMesh.h"

ModelMesh::ModelMesh()
{
}

ModelMesh::~ModelMesh()
{
}

bool ModelMesh::LoadModel(std::string Directory, const char* ModelFilename)
{
	// Load in the model
	Result_ = LoadModelFromObj(Directory, ModelFilename);
	if (!Result_)
	{
		return false;
	}

	// Calculate model binormals, normals and tangents
	CalculateTangentBinormals();

	// Calculate collision
	CalculateBoundingBox();

	// Initialise the vertex and index buffer
	Result_ = InitialiseBuffers();
	if (!Result_)
	{
		return false;
	}

	return true;
}

bool ModelMesh::LoadModel(char* ModelFilename, wstring textureFilename)
{
	// Load in the model
	Result_ = LoadModelFromTxt(ModelFilename);
	if (!Result_)
	{
		return false;
	}

	// Calculate model binormals and tangents
	CalculateTangentBinormals();

	// Calculate collision
	CalculateBoundingBox();

	// Initialise the vertex and index buffer
	Result_ = InitialiseBuffers();
	if (!Result_)
	{
		return false;
	}

	// Load a texture if required
	if (!textureFilename.empty())
	{
		Result_ = CreateMaterial(vector<wstring>(1, textureFilename));
		if (!Result_)
		{
			return false;
		}
	}

	return true;
}

bool ModelMesh::LoadModelFromTxt(char* ModelFilename)
{
	ifstream fin;
	char InputManager;
	int i;

	//===============
	// Read TXT File
	//===============

	// Open the model file.
	fin.open(ModelFilename);

	// If it could not open the file then exit
	if (fin.fail()) { return false; }

	// Read up to the value of vertex count
	fin.get(InputManager);
	while (InputManager != ':')
	{
		fin.get(InputManager);
	}

	// Read in the vertex count
	fin >> VertexCount_;

	// Set the number of indices to be the same as the vertex count
	IndexCount_ = VertexCount_;

	// Create the model using the vertex count that was read in
	Mesh_ = new VertexData[VertexCount_];
	if (!Mesh_)
	{
		return false;
	}

	Indices_ = new unsigned long[IndexCount_];
	if (!Indices_)
	{
		return false;
	}

	// Read up to the beginning of the data
	fin.get(InputManager);
	while (InputManager != ':')
	{
		fin.get(InputManager);
	}
	fin.get(InputManager);
	fin.get(InputManager);

	//==========================
	// Construct Mesh from File
	//==========================

	// Read in the vertex data.
	for (i = 0; i<VertexCount_; i++)
	{
		fin >> Mesh_[i].position.x >> Mesh_[i].position.y >> Mesh_[i].position.z;
		fin >> Mesh_[i].texture.x >> Mesh_[i].texture.y;
		fin >> Mesh_[i].normal.x >> Mesh_[i].normal.y >> Mesh_[i].normal.z;

		Mesh_[i].texture.z = 0;
		Mesh_[i].binormal = D3DXVECTOR3(0, 0, 0);
		Mesh_[i].tangent = D3DXVECTOR3(0, 0, 0);

		Indices_[i] = i;
	}

	// Close the model file.
	fin.close();

	return true;
}

bool ModelMesh::LoadModelFromObj(string Directory, const char* ModelFilename)
{
	ifstream fin;
	char linebuffer[256];
	vector<D3DXVECTOR3> Vertices;
	vector<D3DXVECTOR2> TextureCoords;
	vector<D3DXVECTOR3> FaceNormals;
	vector<D3DXVECTOR3> Faces;
	vector<wstring> TextureFilenames;
	vector<int> TextureIDs;
	D3DXVECTOR3 TempVertex;
	D3DXVECTOR2 TempTexCoord;
	D3DXVECTOR3 TempFace[3];
	float VertexID;
	float TextureID;
	float NormalID;

	//===================
	// Load the OBJ File
	//===================

	// Open the file
	fin.open(Directory + ModelFilename);

	// Check if file couldnt be opened
	if (fin.fail()) { return false; }

	// Loop through the file lines 
	while (!fin.eof())
	{
		// Get the current line
		fin.getline(linebuffer, 256);

		if (string(linebuffer).find("v ") == 0) // Line defines a TempVertex position
		{
			//Read the TempVertex line and push the data into the vector
			sscanf_s(string(linebuffer).c_str(), "v %f %f %f", &TempVertex.x, &TempVertex.y, &TempVertex.z);
			Vertices.push_back(TempVertex);
		}
		else if (string(linebuffer).find("vt ") == 0) // Line defines a TempVertex texture
		{
			//Read the texture coord line and push the data into the vector
			sscanf_s(string(linebuffer).c_str(), "vt %f %f", &TempTexCoord.x, &TempTexCoord.y);
			TextureCoords.push_back(TempTexCoord);
		}
		else if (string(linebuffer).find("vn ") == 0) // Line defines a TempVertex normal
		{
			//Read the normal line and push the data into the vector
			sscanf_s(string(linebuffer).c_str(), "vn %f %f %f", &TempVertex.x, &TempVertex.y, &TempVertex.z);
			FaceNormals.push_back(TempVertex);
		}
		else if (string(linebuffer).find("f ") == 0) // Line defines a triangle (Faces)
		{
			//Read the face line
			sscanf_s(string(linebuffer).c_str(), "f %f/%f/%f %f/%f/%f %f/%f/%f",
				&TempFace[0].x, &TempFace[0].y, &TempFace[0].z,
				&TempFace[1].x, &TempFace[1].y, &TempFace[1].z,
				&TempFace[2].x, &TempFace[2].y, &TempFace[2].z);

			// Add the Faces into the vector
			Faces.push_back(TempFace[0]);
			Faces.push_back(TempFace[1]);
			Faces.push_back(TempFace[2]);

			// Add the texture IDs for the three Faces
			TextureIDs.push_back(TextureFilenames.size() - 1);
			TextureIDs.push_back(TextureFilenames.size() - 1);
			TextureIDs.push_back(TextureFilenames.size() - 1);
		}
		else if (string(linebuffer).find("usemtl ") == 0) // Line is a texture filename 
		{
			// Split the string up as we only need the second part (actual file name)
			istringstream ss(linebuffer);
			istream_iterator<string> begin(ss), end;
			vector<string> stringParts(begin, end);

			// Compile the full filename
			string filename = Directory + stringParts[1] + ".dds";

			// Check if the file is already on the list
			if (std::find(TextureFilenames.begin(), TextureFilenames.end(), wstring(filename.begin(), filename.end())) == TextureFilenames.end())
			{
				// Add the filename to the vector
				TextureFilenames.push_back(wstring(filename.begin(), filename.end()));
			}
		}
	}

	// Close the file
	fin.close();

	//====================
	// Construct the Mesh
	//====================

	// Create the model using the vertex count that was read in
	IndexCount_ = VertexCount_ = Faces.size();
	Mesh_ = new VertexData[VertexCount_];
	if (!Mesh_) { return false; }

	Indices_ = new unsigned long[IndexCount_];
	if (!Indices_) { return false; }

	for (unsigned int Index = 0; Index < Faces.size(); Index++)
	{
		// Read in Face Data
		VertexID = Faces[Index].x - 1;
		TextureID = Faces[Index].y - 1;
		NormalID = Faces[Index].z - 1;

		// Grab data from the vectors and apply them to the model
		Mesh_[Index].position.x = Vertices[VertexID].x;
		Mesh_[Index].position.y = Vertices[VertexID].y;
		Mesh_[Index].position.z = Vertices[VertexID].z * -1; //Flip for RH WindowManager

		Mesh_[Index].texture.x = TextureCoords[TextureID].x;
		Mesh_[Index].texture.y = 1 - TextureCoords[TextureID].y; //Flip for RH WindowManager
		Mesh_[Index].texture.z = TextureIDs[Index];

		Mesh_[Index].normal.x = FaceNormals[NormalID].x;
		Mesh_[Index].normal.y = FaceNormals[NormalID].y;
		Mesh_[Index].normal.z = FaceNormals[NormalID].z * -1; //Flip for RH WindowManager

		Indices_[Index] = Index;
	}

	//===============
	// Load Textures
	//===============

	Result_ = CreateMaterial(TextureFilenames);
	if (!Result_)
	{
		return false;
	}

	// Clean up
	Vertices.clear();
	TextureCoords.clear();
	FaceNormals.clear();
	Faces.clear();
	TextureFilenames.clear();
	TextureIDs.clear();

	return true;
}
