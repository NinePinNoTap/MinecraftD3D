#include "PrimitiveMesh.h"

PrimitiveMesh::PrimitiveMesh()
{
}

PrimitiveMesh::~PrimitiveMesh()
{
}


// Procedurally Generating Mesh
bool PrimitiveMesh::CreateSphere(Rect3D LongLat, float Radius)
{
	vector<D3DXVECTOR3> VertexPositions;
	vector<D3DXVECTOR3> TextureCoordinates;
	vector<D3DXVECTOR3> Normals;
	vector<int> VertexIndices;

	float Lats, Longs, Theta, Delta;
	float x, y, z;
	int CurrentIndex;
	float U, V;
	int RowOffset;

	//=======================
	// Construct Vertex Data
	//=======================

	// Calculate vertex delta
	Longs = 1 / ((float)LongLat.width - 1);
	Lats = 1 / ((float)LongLat.height - 1);

	// Calculate texture delta
	U = 5 / (float)LongLat.width;
	V = 5 / (float)LongLat.height;

	// Calculate row offset
	RowOffset = LongLat.width + 1;

	for (int ring = 0; ring <= LongLat.width; ring++)
	{
		for (int sect = 0; sect <= LongLat.height; sect++)
		{
			Theta = 2 * D3DX_PI * sect * Lats;
			Delta = D3DX_PI * ring * Longs;

			x = cos(Theta)*sin(Delta);
			y = cos(Delta);
			z = sin(Theta)*sin(Delta);

			float length = sqrt((x * x) + (y*y) + (z*z));

			VertexPositions.push_back(D3DXVECTOR3(x * Radius, y * Radius, z*Radius));
			TextureCoordinates.push_back(D3DXVECTOR3(sect*U, ring*V, 0.0f));
			Normals.push_back(D3DXVECTOR3(x / length, y / length, z / length));
		}
	}

	// Calculate the indices
	for (int y = 0; y < LongLat.width; y++)
	{
		for (int x = 0; x < LongLat.height; x++)
		{
			VertexIndices.push_back(x + ((y + 1) * RowOffset)); //Top Left
			VertexIndices.push_back((x + 1) + ((y + 1) * RowOffset)); //Top Right
			VertexIndices.push_back((x + 1) + (y* RowOffset)); //Bottom Right

			VertexIndices.push_back(x + (y* RowOffset)); //Bottom Left
			VertexIndices.push_back(x + (y + 1) * RowOffset); //Top Left
			VertexIndices.push_back((x + 1) + (y * RowOffset)); //Bottom Right
		}
	}

	//=============
	// Build Model
	//=============

	// Count how many vertices and indices we have
	VertexCount_ = VertexPositions.size();
	IndexCount_ = VertexIndices.size();

	// Create the data arrays
	Indices_ = new unsigned long[IndexCount_];
	if (!Indices_)
	{
		return false;
	}

	Mesh_ = new VertexData[VertexCount_ + 1];
	if (!Mesh_)
	{
		return false;
	}

	// Set up the model
	for (int i = 0; i < IndexCount_; i++)
	{
		CurrentIndex = VertexIndices[i];

		Mesh_[CurrentIndex].position = VertexPositions[CurrentIndex];
		Mesh_[CurrentIndex].texture = TextureCoordinates[CurrentIndex];
		Mesh_[CurrentIndex].normal = Normals[CurrentIndex];

		Indices_[i] = CurrentIndex;
	}

	// Calculate collision
	CalculateBoundingBox();

	// Create the buffers
	Result_ = InitialiseBuffers();
	if (!Result_)
	{
		return false;
	}

	// Clean Up
	VertexPositions.clear();
	TextureCoordinates.clear();
	Normals.clear();
	VertexIndices.clear();

	return true;
}

bool PrimitiveMesh::CreatePlane(Rect3D PlaneSize, Rect3D NoOfTiles, float TextureRepeat)
{
	vector<D3DXVECTOR3> VertexPositions;
	vector<D3DXVECTOR3> TextureCoordinates;
	vector<D3DXVECTOR3> Normals;
	vector<int> VertexIndices;
	int CurrentIndex, RowOffset;
	float HalfWidth, HalfDepth;
	float TileWidth, TileDepth;
	float TextureWidth, TextureDepth;

	//=======================
	// Construct Vertex Data
	//=======================

	// Calculate the mid way point so the object is centered
	HalfWidth = PlaneSize.width / 2.0f;
	HalfDepth = PlaneSize.height / 2.0f;

	// Calculate how big each tile is
	TileWidth = PlaneSize.width / NoOfTiles.width;
	TileDepth = PlaneSize.height / NoOfTiles.height;

	// Calculate the difference in texture coordinates between vertices
	TextureWidth = TileWidth * TextureRepeat;
	TextureDepth = TileDepth * TextureRepeat;

	// Calculate how much the next row is offset for calculating indices
	RowOffset = NoOfTiles.width + 1;

	// Calculate vertices, texture coordinates and normals
	for (int z = 0; z <= NoOfTiles.height; z++)
	{
		for (int x = 0; x <= NoOfTiles.width; x++)
		{
			// Calculate the vertex position
			VertexPositions.push_back(D3DXVECTOR3((x * TileWidth) - HalfWidth, 0, (z * TileDepth) - HalfDepth));
			TextureCoordinates.push_back(D3DXVECTOR3(x * TextureWidth, z * TextureDepth, 0.0f));
			Normals.push_back(D3DXVECTOR3(0, 1, 0));
		}
	}

	// Calculate the indices
	for (int y = 0; y < NoOfTiles.height; y++)
	{
		for (int x = 0; x < NoOfTiles.width; x++)
		{
			VertexIndices.push_back(x + ((y + 1) * RowOffset)); //Top Left
			VertexIndices.push_back((x + 1) + ((y + 1) * RowOffset)); //Top Right
			VertexIndices.push_back((x + 1) + (y* RowOffset)); //Bottom Right

			VertexIndices.push_back(x + (y* RowOffset)); //Bottom Left
			VertexIndices.push_back(x + (y + 1) * RowOffset); //Top Left
			VertexIndices.push_back((x + 1) + (y * RowOffset)); //Bottom Right
		}
	}

	//=============
	// Build Model
	//=============

	// Count how many vertices and indices we have
	VertexCount_ = VertexPositions.size();
	IndexCount_ = VertexIndices.size();

	// Create the data arrays
	Indices_ = new unsigned long[IndexCount_];
	if (!Indices_)
	{
		return false;
	}
	Mesh_ = new VertexData[VertexCount_];
	if (!Mesh_)
	{
		return false;
	}

	// Set up the model
	for (int i = 0; i < IndexCount_; i++)
	{
		CurrentIndex = VertexIndices[i];

		Mesh_[CurrentIndex].position = VertexPositions[CurrentIndex];
		Mesh_[CurrentIndex].texture = TextureCoordinates[CurrentIndex];
		Mesh_[CurrentIndex].normal = Normals[CurrentIndex];

		Indices_[i] = CurrentIndex;
	}

	// Calculate collision
	CalculateBoundingBox();

	// Create the buffers
	Result_ = InitialiseBuffers();
	if (!Result_)
	{
		return false;
	}

	// Clean Up
	VertexPositions.clear();
	TextureCoordinates.clear();
	Normals.clear();
	VertexIndices.clear();

	return true;
}

bool PrimitiveMesh::Create2DBox(Rect3D Plane)
{
	// Set the number of vertices in the vertex array.
	VertexCount_ = 6;

	// Set the number of indices in the index array.
	IndexCount_ = 6;

	// Create the vertex array.
	Mesh_ = new VertexData[VertexCount_];
	if (!Mesh_)
	{
		return false;
	}

	// Create the index array.
	Indices_ = new unsigned long[IndexCount_];
	if (!Indices_)
	{
		return false;
	}

	float halfW = Plane.width / 2;
	float halfH = Plane.height / 2;

	// Load the vertex array with data.
	Mesh_[0].position = D3DXVECTOR3(-halfW, halfH, 0.0f);  // Top left
	Mesh_[1].position = D3DXVECTOR3(halfW, halfH, 0.0f);  // Top right
	Mesh_[2].position = D3DXVECTOR3(halfW, -halfH, 0.0f);  // Bottom right
	Mesh_[3].position = D3DXVECTOR3(-halfW, -halfH, 0.0f);  // Bottom left
	Mesh_[4].position = D3DXVECTOR3(-halfW, halfH, 0.0f);  // Top left
	Mesh_[5].position = D3DXVECTOR3(halfW, -halfH, 0.0f);  // Bottom right

	Mesh_[0].texture = D3DXVECTOR3(0.0f, 0.0f, 0.0f); // Top left
	Mesh_[1].texture = D3DXVECTOR3(1.0f, 0.0f, 0.0f); // Top right
	Mesh_[2].texture = D3DXVECTOR3(1.0f, 1.0f, 0.0f); // Bottom right
	Mesh_[3].texture = D3DXVECTOR3(0.0f, 1.0f, 0.0f); // Bottom left
	Mesh_[4].texture = D3DXVECTOR3(0.0f, 0.0f, 0.0f); // Top left
	Mesh_[5].texture = D3DXVECTOR3(1.0f, 1.0f, 0.0f); // Bottom right

	// Load the index array with data.
	Indices_[0] = 0;  // Top left
	Indices_[1] = 1;  // Top right
	Indices_[2] = 2;  // Bottom right
	Indices_[3] = 3;  // Bottom left
	Indices_[4] = 4;  // Top left
	Indices_[5] = 5;  // Bottom right

	// Calculate collision
	CalculateBoundingBox();

	// Create buffers
	Result_ = InitialiseBuffers();
	if (!Result_)
	{
		return false;
	}

	return true;
}

bool PrimitiveMesh::Create3DBox(Rect3D BoxSize, float TextureRepeat)
{
	vector<D3DXVECTOR3> VertexPositions;
	vector<D3DXVECTOR3> TextureCoords;
	Vector3 TextureTiling;

	float halfW, halfH, halfD;
	int Index;

	// Calculate half dimensions
	halfW = BoxSize.width / 2.0f;
	halfH = BoxSize.height / 2.0f;
	halfD = BoxSize.depth / 2.0f;

	// Calculate texture tiling
	TextureTiling.x = BoxSize.width * TextureRepeat;
	TextureTiling.y = BoxSize.height * TextureRepeat;
	TextureTiling.z = BoxSize.depth * TextureRepeat;

	// Define the number of verts and indices
	VertexCount_ = 24;
	IndexCount_ = 36;

	// Create the vertex array.
	Mesh_ = new VertexData[VertexCount_];
	if (!Mesh_)
	{
		return false;
	}

	//====================
	// Define Vertex Data
	//====================

	// Front
	Mesh_[0].position = D3DXVECTOR3(-halfW, halfH, -halfD);
	Mesh_[1].position = D3DXVECTOR3(halfW, halfH, -halfD);
	Mesh_[2].position = D3DXVECTOR3(-halfW, -halfH, -halfD);
	Mesh_[3].position = D3DXVECTOR3(halfW, -halfH, -halfD);

	Mesh_[0].texture = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	Mesh_[1].texture = D3DXVECTOR3(TextureTiling.x, 0.0f, 0.0f);
	Mesh_[2].texture = D3DXVECTOR3(0.0f, TextureTiling.y, 0.0f);
	Mesh_[3].texture = D3DXVECTOR3(TextureTiling.x, TextureTiling.y, 0.0f);

	// Right
	Mesh_[4].position = D3DXVECTOR3(halfW, halfH, -halfD);
	Mesh_[5].position = D3DXVECTOR3(halfW, halfH, halfD);
	Mesh_[6].position = D3DXVECTOR3(halfW, -halfH, -halfD);
	Mesh_[7].position = D3DXVECTOR3(halfW, -halfH, halfD);

	Mesh_[4].texture = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	Mesh_[5].texture = D3DXVECTOR3(TextureTiling.z, 0.0f, 0.0f);
	Mesh_[6].texture = D3DXVECTOR3(0.0f, TextureTiling.y, 0.0f);
	Mesh_[7].texture = D3DXVECTOR3(TextureTiling.z, TextureTiling.y, 0.0f);

	// Top
	Mesh_[8].position = D3DXVECTOR3(-halfW, halfH, halfD);
	Mesh_[9].position = D3DXVECTOR3(halfW, halfH, halfD);
	Mesh_[10].position = D3DXVECTOR3(-halfW, halfH, -halfD);
	Mesh_[11].position = D3DXVECTOR3(halfW, halfH, -halfD);

	Mesh_[8].texture = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	Mesh_[9].texture = D3DXVECTOR3(TextureTiling.x, 0.0f, 0.0f);
	Mesh_[10].texture = D3DXVECTOR3(0.0f, TextureTiling.z, 0.0f);
	Mesh_[11].texture = D3DXVECTOR3(TextureTiling.x, TextureTiling.z, 0.0f);

	// Back
	Mesh_[12].position = D3DXVECTOR3(halfW, halfH, halfD);
	Mesh_[13].position = D3DXVECTOR3(-halfW, halfH, halfD);
	Mesh_[14].position = D3DXVECTOR3(halfW, -halfH, halfD);
	Mesh_[15].position = D3DXVECTOR3(-halfW, -halfH, halfD);

	Mesh_[12].texture = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	Mesh_[13].texture = D3DXVECTOR3(TextureTiling.x, 0.0f, 0.0f);
	Mesh_[14].texture = D3DXVECTOR3(0.0f, TextureTiling.y, 0.0f);
	Mesh_[15].texture = D3DXVECTOR3(TextureTiling.x, TextureTiling.y, 0.0f);

	// Left
	Mesh_[16].position = D3DXVECTOR3(-halfW, halfH, halfD);
	Mesh_[17].position = D3DXVECTOR3(-halfW, halfH, -halfD);
	Mesh_[18].position = D3DXVECTOR3(-halfW, -halfH, halfD);
	Mesh_[19].position = D3DXVECTOR3(-halfW, -halfH, -halfD);

	Mesh_[16].texture = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	Mesh_[17].texture = D3DXVECTOR3(TextureTiling.z, 0.0f, 0.0f);
	Mesh_[18].texture = D3DXVECTOR3(0.0f, TextureTiling.y, 0.0f);
	Mesh_[19].texture = D3DXVECTOR3(TextureTiling.z, TextureTiling.y, 0.0f);

	// Bottom
	Mesh_[20].position = D3DXVECTOR3(-halfW, -halfH, -halfD);
	Mesh_[21].position = D3DXVECTOR3(halfW, -halfH, -halfD);
	Mesh_[22].position = D3DXVECTOR3(-halfW, -halfH, halfD);
	Mesh_[23].position = D3DXVECTOR3(halfW, -halfH, halfD);

	Mesh_[20].texture = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	Mesh_[21].texture = D3DXVECTOR3(TextureTiling.x, 0.0f, 0.0f);
	Mesh_[22].texture = D3DXVECTOR3(0.0f, TextureTiling.z, 0.0f);
	Mesh_[23].texture = D3DXVECTOR3(TextureTiling.x, TextureTiling.z, 0.0f);

	// Create the index array.
	Indices_ = new unsigned long[IndexCount_]
	{
		0, 1, 3, 2, 0, 3, // Front
			4, 5, 7, 6, 4, 7, // Right
			12, 13, 15, 14, 12, 15, // Back
			16, 17, 19, 18, 16, 19, // Left
			8, 9, 11, 10, 8, 11, // Top
			20, 21, 23, 22, 20, 23, // Bottom
	};

	// Calculate normals, tangent and binormals
	CalculateHardNormals();
	CalculateTangentBinormals();

	// Calcualte collision detection
	CalculateBoundingBox();

	// Create buffers
	Result_ = InitialiseBuffers();
	if (!Result_)
	{
		return false;
	}

	return true;
}

bool PrimitiveMesh::CreateSkyPlane(float QuadCount, float PlaneWidth, float MaxHeight, float TextureRepeat)
{
	float quadSize, radius, constant, textureDelta;
	D3DXVECTOR3 position;
	D3DXVECTOR3 texture;
	int index, index1, index2, index3, index4;

	//====================
	// Create Vertex Data
	//====================

	// Create the array to hold the sky plane coordinates.
	ModelData_ = new VertexData[(QuadCount + 1) * (QuadCount + 1)];
	if (!ModelData_) { return false; }

	// Determine the size of each quad on the sky plane.
	quadSize = PlaneWidth / QuadCount;

	// Calculate the radius of the sky plane based on the width.
	radius = PlaneWidth / 2.0f;

	// Calculate the height constant to increment by.
	constant = MaxHeight / (radius * radius);

	// Calculate the texture coordinate increment value.
	textureDelta = (float)TextureRepeat / (float)QuadCount;

	// Loop through the sky plane and build the coordinates based on the increment values given.
	for (int j = 0; j <= QuadCount; j++)
	{
		for (int i = 0; i <= QuadCount; i++)
		{
			// Calculate the vertex coordinates.
			position.x = (-radius) + ((float)i * quadSize);
			position.z = (-radius) + ((float)j * quadSize);
			position.y = MaxHeight - (constant * ((position.x * position.x) + (position.z * position.z)));

			// Calculate the texture coordinates.
			texture.x = (float)i * textureDelta;
			texture.y = (float)j * textureDelta;
			texture.z = 0.0f;

			// Calculate the index into the sky plane array to add this coordinate.
			index = j * (QuadCount + 1) + i;

			// Add the coordinates to the sky plane array.
			ModelData_[index].position = position;
			ModelData_[index].texture = texture;
		}
	}

	//=================
	// Build the Model
	//=================

	// Calculate the number of vertices in the sky plane mesh.
	VertexCount_ = (QuadCount + 1) * (QuadCount + 1) * 6;

	// Set the index count to the same as the vertex count.
	IndexCount_ = VertexCount_;

	// Create the vertex array.
	Mesh_ = new VertexData[VertexCount_];
	if (!Mesh_) { return false; }

	// Create the index array.
	Indices_ = new unsigned long[IndexCount_];
	if (!Indices_) { return false; }

	// Initialise the index into the vertex array.
	index = 0;

	// Load the vertex and index array with the sky plane array data.
	for (int j = 0; j<QuadCount; j++)
	{
		for (int i = 0; i<QuadCount; i++)
		{
			index1 = j * (QuadCount + 1) + i;
			index2 = j * (QuadCount + 1) + (i + 1);
			index3 = (j + 1) * (QuadCount + 1) + i;
			index4 = (j + 1) * (QuadCount + 1) + (i + 1);

			// Triangle 1 - Upper Left
			Mesh_[index].position = ModelData_[index1].position;
			Mesh_[index].texture = ModelData_[index1].texture;
			Indices_[index] = index;
			index++;

			// Triangle 1 - Upper Right
			Mesh_[index].position = ModelData_[index2].position;
			Mesh_[index].texture = ModelData_[index2].texture;
			Indices_[index] = index;
			index++;

			// Triangle 1 - Bottom Left
			Mesh_[index].position = ModelData_[index3].position;
			Mesh_[index].texture = ModelData_[index3].texture;
			Indices_[index] = index;
			index++;

			// Triangle 2 - Bottom Left
			Mesh_[index].position = ModelData_[index3].position;
			Mesh_[index].texture = ModelData_[index3].texture;
			Indices_[index] = index;
			index++;

			// Triangle 2 - Upper Right
			Mesh_[index].position = ModelData_[index2].position;
			Mesh_[index].texture = ModelData_[index2].texture;
			Indices_[index] = index;
			index++;

			// Triangle 2 - Bottom Right
			Mesh_[index].position = ModelData_[index4].position;
			Mesh_[index].texture = ModelData_[index4].texture;
			Indices_[index] = index;
			index++;
		}
	}

	// Clean Up
	delete[] ModelData_;

	// Initialise Buffers
	Result_ = InitialiseBuffers();
	if (!Result_)
	{
		return false;
	}

	return true;
}
