#include "TerrainFactory.h"

TerrainFactory::TerrainFactory()
{

}
TerrainFactory::~TerrainFactory()
{

}

bool TerrainFactory::createTerrain(Rect3D terrainSize, Vector2 textureRepeat, int terrainScale, Model& terrainModel)
{
	int index;
	D3DXVECTOR3 tempVertex;
	VertexData* modelData;
	bool m_result;

	const int terrainVertices = terrainSize.width * terrainSize.height;

	// create the model
	modelData = new VertexData[terrainVertices];
	if (!modelData)
	{
		return false;
	}

	// Calculate texture tiling
	float uW = (textureRepeat.x / terrainSize.width);
	float uH = (textureRepeat.y / terrainSize.height);

	// Read the image data into the height map
	for (int j = 0; j<terrainSize.height; j++)
	{
		for (int i = 0; i<terrainSize.width; i++)
		{
			// Calculate the index
			index = (terrainSize.width * j) + i;

			// create the vertex position
			tempVertex.x = (float)i * terrainScale;
			tempVertex.y = 0;
			tempVertex.z = (float)j * terrainScale;

			// Store the position and calculate texture
			modelData[index].position = tempVertex;
			modelData[index].texture = D3DXVECTOR2(i * uW, j * uH);
		}
	}

	// Calculate terrain normals
	m_result = calculateNormals(terrainSize, modelData);
	if (!m_result)
	{
		return false;
	}

	// Construct the final terrain mesh
	m_result = buildModelFromData(terrainSize, modelData, terrainModel);
	if (!m_result)
	{
		return false;
	}

	return true;
}

bool TerrainFactory::createTerrainWithPerlinNoise(const Rect3D terrainSize, Vector2 textureRepeat, int Scale, float Smoothing, Model& model, int Seed)
{
	int index;
	float noise;
	D3DXVECTOR3 tempVertex;
	PerlinNoise perlinNoise;
	VertexData* modelData;

	//===============================
	// set Up Perlin noise Generator
	//===============================

	perlinNoise.setSeed(Seed);

	//=============================
	// create Perlin noise Terrain
	//=============================

	const int Size = terrainSize.width * terrainSize.height;

	// create the model
	modelData = new VertexData[Size];

	// Calculate texture tiling
	float uW = (textureRepeat.x / terrainSize.width);
	float uH = (textureRepeat.y / terrainSize.height);

	// Read the image data into the height map
	for (int j = 0; j<terrainSize.height; j++)
	{
		for (int i = 0; i<terrainSize.width; i++)
		{
			// Calculate the index
			index = (terrainSize.width * j) + i;

			// create noise
			double x = (double)j / ((double)terrainSize.width);
			double y = (double)i / ((double)terrainSize.height);
			noise = perlinNoise.createNoise(10 * x, 10 * y, 0.8f);

			// create the vertex position
			tempVertex.x = (float)i * Scale;
			tempVertex.y = floor(255 * noise) / Smoothing;
			tempVertex.z = (float)j * Scale;

			// Store the position and calculate texture
			modelData[index].position = tempVertex;
			modelData[index].texture = D3DXVECTOR2(i * uW, j * uH);
		}
	}

	// Calculate terrain normals
	calculateNormals(terrainSize, modelData);

	// Construct the final terrain mesh
	buildModelFromData(terrainSize, modelData, model);

	// Clean Up
	delete[] modelData;

	return true;
}

bool TerrainFactory::buildModelFromData(Rect3D Terrain, VertexData* modelData, Model& model)
{
	int index, index1, index2, index3, index4;
	int vertexCount, indexCount;
	VertexData* terrainMesh;
	unsigned long* Indices;

	// set the number of vertices
	vertexCount = (Terrain.width - 1) * (Terrain.height - 1) * 6;

	// create the vertex array
	terrainMesh = new VertexData[vertexCount];
	if (!terrainMesh)
	{
		return false;
	}

	// set the index count to the same as the vertex count
	indexCount = vertexCount;

	// create the index array
	Indices = new unsigned long[indexCount];
	if (!Indices)
	{
		return false;
	}

	// onload the terrain model with the height map terrain data.
	index = 0;

	// Look through and create the model
	for (int j = 0; j<(Terrain.height - 1); j++)
	{
		for (int i = 0; i<(Terrain.width - 1); i++)
		{
			// Calculate index ids
			index1 = (Terrain.width * j) + i;          // Bottom left.
			index2 = (Terrain.width * j) + (i + 1);      // Bottom right.
			index3 = (Terrain.width * (j + 1)) + i;      // Upper left.
			index4 = (Terrain.width * (j + 1)) + (i + 1);  // Upper right.

			// Upper left
			terrainMesh[index] = modelData[index3];
			terrainMesh[index].texture = D3DXVECTOR2(0.0f, 0.0f);
			Indices[index] = index;
			index++;

			// Upper right
			terrainMesh[index] = modelData[index4];
			terrainMesh[index].texture = D3DXVECTOR2(1.0f, 0.0f);
			Indices[index] = index;
			index++;

			// Bottom left
			terrainMesh[index] = modelData[index1];
			terrainMesh[index].texture = D3DXVECTOR2(0.0f, 1.0f);
			Indices[index] = index;
			index++;

			// Bottom left
			terrainMesh[index] = modelData[index1];
			terrainMesh[index].texture = D3DXVECTOR2(0.0f, 1.0f);
			Indices[index] = index;
			index++;

			// Upper right
			terrainMesh[index] = modelData[index4];
			terrainMesh[index].texture = D3DXVECTOR2(1.0f, 0.0f);
			Indices[index] = index;
			index++;

			// Bottom right
			terrainMesh[index] = modelData[index2];
			terrainMesh[index].texture = D3DXVECTOR2(1.0f, 1.0f);
			Indices[index] = index;
			index++;
		}
	}

	//=============
	// create Mesh
	//=============
	
	Mesh3D* newMesh = new Mesh3D;
	newMesh->setMesh(terrainMesh, Indices);
	newMesh->setIndexCount(indexCount);
	newMesh->setVertexCount(vertexCount);
	m_result = newMesh->build();
	if (!m_result)
	{
		return false;
	}

	model.addMesh(newMesh);

	return true;
}

bool TerrainFactory::calculateNormals(Rect3D terrainSize, VertexData* modelData)
{
	int index, index1, index2, index3;
	int count;
	float length;
	Vector3* Normals;

	D3DXVECTOR3 vertex[3];
	D3DXVECTOR3 vector[2];
	D3DXVECTOR3 sum;

	//===================
	// Calculate Normals
	//===================

	// create a temporary array to hold the un-normalized normal vectors.
	Normals = new Vector3[(terrainSize.height - 1) * (terrainSize.width - 1)];
	if (!Normals)
	{
		return false;
	}

	// Go through all the faces in the mesh and calculate their normals
	for (int j = 0; j<(terrainSize.height - 1); j++)
	{
		for (int i = 0; i<(terrainSize.width - 1); i++)
		{
			// Calculate the vertices in the face
			index1 = (j * terrainSize.width) + i; // Bottom Left
			index2 = (j * terrainSize.width) + (i + 1); // Bottom Right
			index3 = ((j + 1) * terrainSize.width) + i; // Top Left

			// get the vertices
			vertex[0] = modelData[index1].position;
			vertex[1] = modelData[index2].position;
			vertex[2] = modelData[index3].position;

			// Calculate the two vectors for this face
			vector[0].x = vertex[0].x - vertex[2].x;
			vector[0].y = vertex[0].y - vertex[2].y;
			vector[0].z = vertex[0].z - vertex[2].z;
			vector[1].x = vertex[2].x - vertex[1].x;
			vector[1].y = vertex[2].y - vertex[1].y;
			vector[1].z = vertex[2].z - vertex[1].z;

			index = (j * (terrainSize.width - 1)) + i;

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
	for (int j = 0; j<terrainSize.height; j++)
	{
		for (int i = 0; i<terrainSize.width; i++)
		{
			// initialise the totals
			sum = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			// initialise the count
			count = 0;

			// Bottom left face
			if (((i - 1) >= 0) && ((j - 1) >= 0))
			{
				index = ((j - 1) * (terrainSize.width - 1)) + (i - 1);

				sum.x += Normals[index].x;
				sum.y += Normals[index].y;
				sum.z += Normals[index].z;
				count++;
			}

			// Bottom right face
			if ((i < (terrainSize.width - 1)) && ((j - 1) >= 0))
			{
				index = ((j - 1) * (terrainSize.width - 1)) + i;

				sum.x += Normals[index].x;
				sum.y += Normals[index].y;
				sum.z += Normals[index].z;
				count++;
			}

			// Upper left face
			if (((i - 1) >= 0) && (j < (terrainSize.height - 1)))
			{
				index = (j * (terrainSize.width - 1)) + (i - 1);

				sum.x += Normals[index].x;
				sum.y += Normals[index].y;
				sum.z += Normals[index].z;
				count++;
			}

			// Upper right face
			if ((i < (terrainSize.width - 1)) && (j < (terrainSize.height - 1)))
			{
				index = (j * (terrainSize.width - 1)) + i;

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

			// get an index to the vertex location in the height map array.
			index = (j * terrainSize.width) + i;

			// normalise the normal for this vertex
			modelData[index].normal = D3DXVECTOR3(sum.x / length, sum.y / length, sum.z / length);
		}
	}

	// Release the temporary normals
	delete[] Normals;
	Normals = 0;

	return true;
}