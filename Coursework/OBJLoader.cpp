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
	vector<D3DXVECTOR3> Vertices;
	vector<D3DXVECTOR2> TextureCoords;
	vector<D3DXVECTOR3> FaceNormals;
	vector<D3DXVECTOR3> Faces;
	vector<wstring> TextureFilenames;
	vector<int> TextureIDs;
	D3DXVECTOR3 TempVertex;
	D3DXVECTOR2 TempTexCoord;
	D3DXVECTOR3 TempFace[3];
	float VertexID, TextureID, NormalID;
	int indexCount, vertexCount;
	VertexData* ObjMesh;
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
			string filename = TEXTURE_DIR + stringParts[1] + ".dds";

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
	// Construct the Mesh3D
	//====================

	// Create the model using the vertex count that was read in
	indexCount = vertexCount = Faces.size();
	ObjMesh = new VertexData[vertexCount];
	if (!ObjMesh) { return false; }

	Indices = new unsigned long[indexCount];
	if (!Indices) { return false; }

	for (unsigned int Index = 0; Index < Faces.size(); Index++)
	{
		// Read in Face Data
		VertexID = Faces[Index].x - 1;
		TextureID = Faces[Index].y - 1;
		NormalID = Faces[Index].z - 1;

		// Grab data from the vectors and apply them to the model
		ObjMesh[Index].position.x = Vertices[VertexID].x;
		ObjMesh[Index].position.y = Vertices[VertexID].y;
		ObjMesh[Index].position.z = Vertices[VertexID].z * -1; //Flip for RH WindowManager

		ObjMesh[Index].texture.x = TextureCoords[TextureID].x;
		ObjMesh[Index].texture.y = 1 - TextureCoords[TextureID].y; //Flip for RH WindowManager
		ObjMesh[Index].texture.z = TextureIDs[Index];

		ObjMesh[Index].normal.x = FaceNormals[NormalID].x;
		ObjMesh[Index].normal.y = FaceNormals[NormalID].y;
		ObjMesh[Index].normal.z = FaceNormals[NormalID].z * -1; //Flip for RH WindowManager

		Indices[Index] = Index;
	}

	//================
	// Finalise Model
	//================

	model.GetMesh()->SetMesh(ObjMesh, Indices);
	model.GetMesh()->SetIndexCount(indexCount);
	model.GetMesh()->SetVertexCount(vertexCount);
	model.GetMaterial()->SetTextureArray(TextureFilenames);

	//==========
	// Clean up
	//==========

	Vertices.clear();
	TextureCoords.clear();
	FaceNormals.clear();
	Faces.clear();
	TextureFilenames.clear();
	TextureIDs.clear();

	return model.GetMesh()->Build();
}