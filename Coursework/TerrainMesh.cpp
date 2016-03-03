#include "TerrainMesh.h"

TerrainMesh::TerrainMesh()
{
	Seed_ = 0;
}

TerrainMesh::~TerrainMesh()
{
}

bool TerrainMesh::CreateTerrain(ObjSize TerrainSize, Vector2 TextureRepeat, int Scale)
{
	// Store terrain size
	Terrain_ = TerrainSize;

	// Create a flat terrain
	BuildFlatTerrain(TextureRepeat, Scale);

	// Calculate terrain normals
	Result_ = CalculateTerrainNormals();
	if (!Result_)
	{
		return false;
	}

	// Construct the final terrain mesh
	Result_ = BuildModelFromModelData();
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
}

void TerrainMesh::CreateTerrainWithPerlinNoise(ObjSize TerrainSize, Vector2 TextureRepeat, int Scale, float Smoothing, int Seed)
{
	int index;
	float noise;
	D3DXVECTOR3 TempVertex;
	PerlinNoiseGenerator NoiseGen;

	if (Seed == -1)
		Seed = rand() % 10000;

	// Don't regenerate if the seed is the same
	if (Seed_ == Seed)
		return;

	NoiseGen.SetSeed(Seed);

	// Store seed
	Seed_ = Seed;

	// Store terrain size
	Terrain_ = TerrainSize;

	// Create the model
	ModelData_ = new VertexData[Terrain_.width * Terrain_.height];

	// Calculate texture tiling
	float uW = (TextureRepeat.x / Terrain_.width);
	float uH = (TextureRepeat.y / Terrain_.height);

	// Read the image data into the height map
	for (int j = 0; j<Terrain_.height; j++)
	{
		for (int i = 0; i<Terrain_.width; i++)
		{
			// Calculate the index
			index = (Terrain_.width * j) + i;

			// Create noise
			double x = (double)j / ((double)Terrain_.width);
			double y = (double)i / ((double)Terrain_.height);
			noise = NoiseGen.CreateNoise(10 * x, 10 * y, 0.8f);

			// Create the vertex position
			TempVertex.x = (float)i * Scale;
			TempVertex.y = floor(255 * noise) / Smoothing;
			TempVertex.z = (float)j * Scale;

			// Store the position and calculate texture
			ModelData_[index].position = TempVertex;
			ModelData_[index].texture = D3DXVECTOR3(i * uW, j * uH, 0.0f);
		}
	}

	// Calculate terrain normals
	CalculateTerrainNormals();

	// Construct the final terrain mesh
	BuildModelFromModelData();
	
	// Calculate model binormals, normals and tangents
	CalculateTangentBinormals();

	// Calculate collision
	CalculateBoundingBox();

	// Initialise the vertex and index buffer
	InitialiseBuffers();
}

bool TerrainMesh::BuildFlatTerrain(Vector2 TextureRepeat, int Scale)
{
	int index;
	D3DXVECTOR3 TempVertex;

	// Create the model
	ModelData_ = new VertexData[Terrain_.width * Terrain_.height];
	if (!ModelData_) { return false; }

	// Calculate texture tiling
	float uW = (TextureRepeat.x / Terrain_.width);
	float uH = (TextureRepeat.y / Terrain_.height);

	// Read the image data into the height map
	for (int j = 0; j<Terrain_.height; j++)
	{
		for (int i = 0; i<Terrain_.width; i++)
		{
			// Calculate the index
			index = (Terrain_.width * j) + i;

			// Create the vertex position
			TempVertex.x = (float)i * Scale;
			TempVertex.y = 0;
			TempVertex.z = (float)j * Scale;

			// Store the position and calculate texture
			ModelData_[index].position = TempVertex;
			ModelData_[index].texture = D3DXVECTOR3(i * uW, j * uH, 0.0f);
		}
	}
}

bool TerrainMesh::CalculateTerrainNormals()
{
	int index, index1, index2, index3, index4;
	int count;
	float length;
	Vector3* Normals;

	D3DXVECTOR3 vertex[3];
	D3DXVECTOR3 vector[2];
	D3DXVECTOR3 sum;

	//===================
	// Calculate Normals
	//===================

	// Create a temporary array to hold the un-normalized normal vectors.
	Normals = new Vector3[(Terrain_.height - 1) * (Terrain_.width - 1)];
	if (!Normals) { return false; }

	// Go through all the faces in the mesh and calculate their normals
	for (int j = 0; j<(Terrain_.height - 1); j++)
	{
		for (int i = 0; i<(Terrain_.width - 1); i++)
		{
			// Calculate the vertices in the face
			index1 = (j * Terrain_.width) + i; // Bottom Left
			index2 = (j * Terrain_.width) + (i + 1); // Bottom Right
			index3 = ((j + 1) * Terrain_.width) + i; // Top Left

			// Get the vertices
			vertex[0] = ModelData_[index1].position;
			vertex[1] = ModelData_[index2].position;
			vertex[2] = ModelData_[index3].position;

			// Calculate the two vectors for this face
			vector[0].x = vertex[0].x - vertex[2].x;
			vector[0].y = vertex[0].y - vertex[2].y;
			vector[0].z = vertex[0].z - vertex[2].z;
			vector[1].x = vertex[2].x - vertex[1].x;
			vector[1].y = vertex[2].y - vertex[1].y;
			vector[1].z = vertex[2].z - vertex[1].z;

			index = (j * (Terrain_.width - 1)) + i;

			// Calculate the cross product of those two vectors to get the un-normalized value for this face normal
			Normals[index].x = (vector[0].y * vector[1].z) - (vector[0].z * vector[1].y);
			Normals[index].y = (vector[0].z * vector[1].x) - (vector[0].x * vector[1].z);
			Normals[index].z = (vector[0].x * vector[1].y) - (vector[0].y * vector[1].x);
		}
	}

	//=================
	// Average Normals
	//=================

	// Go through each vertice and calculate an average normal
	for (int j = 0; j<Terrain_.height; j++)
	{
		for (int i = 0; i<Terrain_.width; i++)
		{
			// Initialise the totals
			sum = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			// Initialise the count
			count = 0;

			// Bottom left face
			if (((i - 1) >= 0) && ((j - 1) >= 0))
			{
				index = ((j - 1) * (Terrain_.width - 1)) + (i - 1);

				sum.x += Normals[index].x;
				sum.y += Normals[index].y;
				sum.z += Normals[index].z;
				count++;
			}

			// Bottom right face
			if ((i < (Terrain_.width - 1)) && ((j - 1) >= 0))
			{
				index = ((j - 1) * (Terrain_.width - 1)) + i;

				sum.x += Normals[index].x;
				sum.y += Normals[index].y;
				sum.z += Normals[index].z;
				count++;
			}

			// Upper left face
			if (((i - 1) >= 0) && (j < (Terrain_.height - 1)))
			{
				index = (j * (Terrain_.width - 1)) + (i - 1);

				sum.x += Normals[index].x;
				sum.y += Normals[index].y;
				sum.z += Normals[index].z;
				count++;
			}

			// Upper right face
			if ((i < (Terrain_.width - 1)) && (j < (Terrain_.height - 1)))
			{
				index = (j * (Terrain_.width - 1)) + i;

				sum.x += Normals[index].x;
				sum.y += Normals[index].y;
				sum.z += Normals[index].z;
				count++;
			}

			// Calculate averages
			sum.x = (sum.x / (float)count);
			sum.y = (sum.y / (float)count);
			sum.z = (sum.z / (float)count);

			// Calculate the length of this normal
			length = sqrt((sum.x * sum.x) + (sum.y * sum.y) + (sum.z * sum.z));

			// Get an index to the vertex location in the height map array.
			index = (j * Terrain_.width) + i;

			// Normalise the normal for this vertex
			ModelData_[index].normal = D3DXVECTOR3(sum.x / length, sum.y / length, sum.z / length);
		}
	}

	// Release the temporary normals
	delete[] Normals;
	Normals = 0;

	return true;
}