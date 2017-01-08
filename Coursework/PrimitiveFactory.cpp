#include "PrimitiveFactory.h"

PrimitiveFactory::PrimitiveFactory()
{
}

PrimitiveFactory::~PrimitiveFactory()
{
}


// Procedurally Generating Mesh3D
bool PrimitiveFactory::createSphere(Rect3D sphereSections, float sphereRadius, Model& model)
{
	vector<D3DXVECTOR3> vertexPositions;
	vector<D3DXVECTOR2> textureCoordinates;
	vector<D3DXVECTOR3> vertexNormals;
	vector<int> vertexIndices;

	float Lats, Longs, Theta, Delta;
	float x, y, z;
	int curIndex;
	float U, V;
	int rowOffset;

	int vertexCount, indexCount;
	VertexData* primitiveMesh;
	unsigned long* Indices;

	//=======================
	// Construct Vertex Data
	//=======================

	// Calculate vertex delta
	Longs = 1 / ((float)sphereSections.width - 1);
	Lats = 1 / ((float)sphereSections.height - 1);

	// Calculate texture delta
	U = 5 / (float)sphereSections.width;
	V = 5 / (float)sphereSections.height;

	// Calculate row offset
	rowOffset = sphereSections.width + 1;

	for (int ring = 0; ring <= sphereSections.width; ring++)
	{
		for (int sect = 0; sect <= sphereSections.height; sect++)
		{
			Theta = 2 * D3DX_PI * sect * Lats;
			Delta = D3DX_PI * ring * Longs;

			x = cos(Theta)*sin(Delta);
			y = cos(Delta);
			z = sin(Theta)*sin(Delta);

			float length = sqrt((x * x) + (y*y) + (z*z));

			vertexPositions.push_back(D3DXVECTOR3(x * sphereRadius, y * sphereRadius, z*sphereRadius));
			textureCoordinates.push_back(D3DXVECTOR2(sect*U, ring*V));
			vertexNormals.push_back(D3DXVECTOR3(x / length, y / length, z / length));
		}
	}

	// Calculate the indices
	for (int y = 0; y < sphereSections.width; y++)
	{
		for (int x = 0; x < sphereSections.height; x++)
		{
			vertexIndices.push_back(x + ((y + 1) * rowOffset)); //Top Left
			vertexIndices.push_back((x + 1) + ((y + 1) * rowOffset)); //Top Right
			vertexIndices.push_back((x + 1) + (y* rowOffset)); //Bottom Right

			vertexIndices.push_back(x + (y* rowOffset)); //Bottom Left
			vertexIndices.push_back(x + (y + 1) * rowOffset); //Top Left
			vertexIndices.push_back((x + 1) + (y * rowOffset)); //Bottom Right
		}
	}

	//=============
	// build Model
	//=============

	// Count how many vertices and indices we have
	vertexCount = vertexPositions.size();
	indexCount = vertexIndices.size();

	// create the data arrays
	Indices = new unsigned long[indexCount];
	if (!Indices)
	{
		return false;
	}

	primitiveMesh = new VertexData[vertexCount + 1];
	if (!primitiveMesh)
	{
		return false;
	}

	// set up the model
	for (int i = 0; i < indexCount; i++)
	{
		curIndex = vertexIndices[i];

		primitiveMesh[curIndex].position = vertexPositions[curIndex];
		primitiveMesh[curIndex].texture = textureCoordinates[curIndex];
		primitiveMesh[curIndex].normal = vertexNormals[curIndex];

		Indices[i] = curIndex;
	}

	// Clean Up
	vertexPositions.clear();
	textureCoordinates.clear();
	vertexNormals.clear();
	vertexIndices.clear();

	//=============
	// create Mesh
	//=============

	Mesh3D* newMesh = new Mesh3D;
	newMesh->setMesh(primitiveMesh, Indices);
	newMesh->setIndexCount(indexCount);
	newMesh->setVertexCount(vertexCount);
	m_result = newMesh->build();
	if (!m_result)
	{
		return false;
	}

	// Add mesh to model
	model.addMesh(newMesh);

	return true;
}

bool PrimitiveFactory::createPlane(Rect3D planeSize, Rect3D tileCount, float textureRepeat, Model& model)
{
	vector<D3DXVECTOR3> vertexPositions;
	vector<D3DXVECTOR2> textureCoordinates;
	vector<D3DXVECTOR3> vertexNormals;
	vector<int> vertexIndices;
	int curIndex, rowOffset;
	float halfW, halfD;
	float tileWidth, tileDepth;
	float textureWidth, textureDepth;

	int vertexCount, indexCount;
	VertexData* primitiveMesh;
	unsigned long* Indices;

	//=======================
	// Construct Vertex Data
	//=======================

	// Calculate the mid way point so the object is centered
	halfW = planeSize.width / 2.0f;
	halfD = planeSize.height / 2.0f;

	// Calculate how big each tile is
	tileWidth = planeSize.width / tileCount.width;
	tileDepth = planeSize.height / tileCount.height;

	// Calculate the difference in texture coordinates between vertices
	textureWidth = tileWidth * textureRepeat;
	textureDepth = tileDepth * textureRepeat;

	// Calculate how much the next row is offset for calculating indices
	rowOffset = tileCount.width + 1;

	// Calculate vertices, texture coordinates and normals
	for (int z = 0; z <= tileCount.height; z++)
	{
		for (int x = 0; x <= tileCount.width; x++)
		{
			// Calculate the vertex position
			vertexPositions.push_back(D3DXVECTOR3((x * tileWidth) - halfW, 0, (z * tileDepth) - halfD));
			textureCoordinates.push_back(D3DXVECTOR2(x * textureWidth, z * textureDepth));
			vertexNormals.push_back(D3DXVECTOR3(0, 1, 0));
		}
	}

	// Calculate the indices
	for (int y = 0; y < tileCount.height; y++)
	{
		for (int x = 0; x < tileCount.width; x++)
		{
			vertexIndices.push_back(x + ((y + 1) * rowOffset)); //Top Left
			vertexIndices.push_back((x + 1) + ((y + 1) * rowOffset)); //Top Right
			vertexIndices.push_back((x + 1) + (y* rowOffset)); //Bottom Right

			vertexIndices.push_back(x + (y* rowOffset)); //Bottom Left
			vertexIndices.push_back(x + (y + 1) * rowOffset); //Top Left
			vertexIndices.push_back((x + 1) + (y * rowOffset)); //Bottom Right
		}
	}

	//=============
	// build Model
	//=============

	// Count how many vertices and indices we have
	vertexCount = vertexPositions.size();
	indexCount = vertexIndices.size();

	// create the data arrays
	Indices = new unsigned long[indexCount];
	if (!Indices)
	{
		return false;
	}
	primitiveMesh = new VertexData[vertexCount];
	if (!primitiveMesh)
	{
		return false;
	}

	// set up the model
	for (int i = 0; i < indexCount; i++)
	{
		curIndex = vertexIndices[i];

		primitiveMesh[curIndex].position = vertexPositions[curIndex];
		primitiveMesh[curIndex].texture = textureCoordinates[curIndex];
		primitiveMesh[curIndex].normal = vertexNormals[curIndex];

		Indices[i] = curIndex;
	}

	// Clean Up
	vertexPositions.clear();
	textureCoordinates.clear();
	vertexNormals.clear();
	vertexIndices.clear();

	//=============
	// create Mesh
	//=============

	Mesh3D* newMesh = new Mesh3D;
	newMesh->setMesh(primitiveMesh, Indices);
	newMesh->setIndexCount(indexCount);
	newMesh->setVertexCount(vertexCount);
	m_result = newMesh->build();
	if (!m_result)
	{
		return false;
	}

	// Add mesh to model
	model.addMesh(newMesh);

	return true;
}

bool PrimitiveFactory::create2DBox(Rect3D planeSize, Model& model)
{
	int vertexCount, indexCount;
	VertexData* primitiveMesh;
	unsigned long* Indices;

	// set the number of vertices in the vertex array.
	vertexCount = 6;

	// set the number of indices in the index array.
	indexCount = 6;

	// create the vertex array.
	primitiveMesh = new VertexData[vertexCount];
	if (!primitiveMesh)
	{
		return false;
	}

	// create the index array.
	Indices = new unsigned long[indexCount];
	if (!Indices)
	{
		return false;
	}

	float halfW = planeSize.width / 2;
	float halfH = planeSize.height / 2;

	// onload the vertex array with data.
	primitiveMesh[0].position = D3DXVECTOR3(-halfW, halfH, 0.0f);  // Top left
	primitiveMesh[1].position = D3DXVECTOR3(halfW, halfH, 0.0f);  // Top right
	primitiveMesh[2].position = D3DXVECTOR3(halfW, -halfH, 0.0f);  // Bottom right
	primitiveMesh[3].position = D3DXVECTOR3(-halfW, -halfH, 0.0f);  // Bottom left
	primitiveMesh[4].position = D3DXVECTOR3(-halfW, halfH, 0.0f);  // Top left
	primitiveMesh[5].position = D3DXVECTOR3(halfW, -halfH, 0.0f);  // Bottom right

	primitiveMesh[0].texture = D3DXVECTOR2(0.0f, 0.0f); // Top left
	primitiveMesh[1].texture = D3DXVECTOR2(1.0f, 0.0f); // Top right
	primitiveMesh[2].texture = D3DXVECTOR2(1.0f, 1.0f); // Bottom right
	primitiveMesh[3].texture = D3DXVECTOR2(0.0f, 1.0f); // Bottom left
	primitiveMesh[4].texture = D3DXVECTOR2(0.0f, 0.0f); // Top left
	primitiveMesh[5].texture = D3DXVECTOR2(1.0f, 1.0f); // Bottom right

	// onload the index array with data.
	Indices[0] = 0;  // Top left
	Indices[1] = 1;  // Top right
	Indices[2] = 2;  // Bottom right
	Indices[3] = 3;  // Bottom left
	Indices[4] = 4;  // Top left
	Indices[5] = 5;  // Bottom right

	//=============
	// create Mesh
	//=============

	Mesh3D* newMesh = new Mesh3D;
	newMesh->setMesh(primitiveMesh, Indices);
	newMesh->setIndexCount(indexCount);
	newMesh->setVertexCount(vertexCount);
	m_result = newMesh->build();
	if (!m_result)
	{
		return false;
	}

	// Add mesh to model
	model.addMesh(newMesh);

	return true;
}

bool PrimitiveFactory::create3DBox(Rect3D BoxSize, float tileFactor, Model& model)
{
	vector<D3DXVECTOR3> vertexPositions;
	vector<D3DXVECTOR3> TextureCoords;
	Vector3 textureRepeat;
	float halfW, halfH, halfD;
	int vertexCount, indexCount;
	VertexData* primitiveMesh;
	unsigned long* Indices;

	// Calculate half dimensions
	halfW = BoxSize.width / 2.0f;
	halfH = BoxSize.height / 2.0f;
	halfD = BoxSize.depth / 2.0f;

	// Calculate texture tiling
	textureRepeat.x = BoxSize.width * tileFactor;
	textureRepeat.y = BoxSize.height * tileFactor;
	textureRepeat.z = BoxSize.depth * tileFactor;

	// Define the number of verts and indices
	vertexCount = 24;
	indexCount = 36;

	// create the vertex array.
	primitiveMesh = new VertexData[vertexCount];
	if (!primitiveMesh)
	{
		return false;
	}

	//====================
	// Define Vertex Data
	//====================

	// Front
	primitiveMesh[0].position = D3DXVECTOR3(-halfW, halfH, -halfD);
	primitiveMesh[1].position = D3DXVECTOR3(halfW, halfH, -halfD);
	primitiveMesh[2].position = D3DXVECTOR3(-halfW, -halfH, -halfD);
	primitiveMesh[3].position = D3DXVECTOR3(halfW, -halfH, -halfD);

	primitiveMesh[0].texture = D3DXVECTOR2(0.0f, 0.0f);
	primitiveMesh[1].texture = D3DXVECTOR2(textureRepeat.x, 0.0f);
	primitiveMesh[2].texture = D3DXVECTOR2(0.0f, textureRepeat.y);
	primitiveMesh[3].texture = D3DXVECTOR2(textureRepeat.x, textureRepeat.y);

	// Right
	primitiveMesh[4].position = D3DXVECTOR3(halfW, halfH, -halfD);
	primitiveMesh[5].position = D3DXVECTOR3(halfW, halfH, halfD);
	primitiveMesh[6].position = D3DXVECTOR3(halfW, -halfH, -halfD);
	primitiveMesh[7].position = D3DXVECTOR3(halfW, -halfH, halfD);

	primitiveMesh[4].texture = D3DXVECTOR2(0.0f, 0.0f);
	primitiveMesh[5].texture = D3DXVECTOR2(textureRepeat.z, 0.0f);
	primitiveMesh[6].texture = D3DXVECTOR2(0.0f, textureRepeat.y);
	primitiveMesh[7].texture = D3DXVECTOR2(textureRepeat.z, textureRepeat.y);

	// Top
	primitiveMesh[8].position = D3DXVECTOR3(-halfW, halfH, halfD);
	primitiveMesh[9].position = D3DXVECTOR3(halfW, halfH, halfD);
	primitiveMesh[10].position = D3DXVECTOR3(-halfW, halfH, -halfD);
	primitiveMesh[11].position = D3DXVECTOR3(halfW, halfH, -halfD);

	primitiveMesh[8].texture = D3DXVECTOR2(0.0f, 0.0f);
	primitiveMesh[9].texture = D3DXVECTOR2(textureRepeat.x, 0.0f);
	primitiveMesh[10].texture = D3DXVECTOR2(0.0f, textureRepeat.z);
	primitiveMesh[11].texture = D3DXVECTOR2(textureRepeat.x, textureRepeat.z);

	// Back
	primitiveMesh[12].position = D3DXVECTOR3(halfW, halfH, halfD);
	primitiveMesh[13].position = D3DXVECTOR3(-halfW, halfH, halfD);
	primitiveMesh[14].position = D3DXVECTOR3(halfW, -halfH, halfD);
	primitiveMesh[15].position = D3DXVECTOR3(-halfW, -halfH, halfD);

	primitiveMesh[12].texture = D3DXVECTOR2(0.0f, 0.0f);
	primitiveMesh[13].texture = D3DXVECTOR2(textureRepeat.x, 0.0f);
	primitiveMesh[14].texture = D3DXVECTOR2(0.0f, textureRepeat.y);
	primitiveMesh[15].texture = D3DXVECTOR2(textureRepeat.x, textureRepeat.y);

	// Left
	primitiveMesh[16].position = D3DXVECTOR3(-halfW, halfH, halfD);
	primitiveMesh[17].position = D3DXVECTOR3(-halfW, halfH, -halfD);
	primitiveMesh[18].position = D3DXVECTOR3(-halfW, -halfH, halfD);
	primitiveMesh[19].position = D3DXVECTOR3(-halfW, -halfH, -halfD);

	primitiveMesh[16].texture = D3DXVECTOR2(0.0f, 0.0f);
	primitiveMesh[17].texture = D3DXVECTOR2(textureRepeat.z, 0.0f);
	primitiveMesh[18].texture = D3DXVECTOR2(0.0f, textureRepeat.y);
	primitiveMesh[19].texture = D3DXVECTOR2(textureRepeat.z, textureRepeat.y);

	// Bottom
	primitiveMesh[20].position = D3DXVECTOR3(-halfW, -halfH, -halfD);
	primitiveMesh[21].position = D3DXVECTOR3(halfW, -halfH, -halfD);
	primitiveMesh[22].position = D3DXVECTOR3(-halfW, -halfH, halfD);
	primitiveMesh[23].position = D3DXVECTOR3(halfW, -halfH, halfD);

	primitiveMesh[20].texture = D3DXVECTOR2(0.0f, 0.0f);
	primitiveMesh[21].texture = D3DXVECTOR2(textureRepeat.x, 0.0f);
	primitiveMesh[22].texture = D3DXVECTOR2(0.0f, textureRepeat.z);
	primitiveMesh[23].texture = D3DXVECTOR2(textureRepeat.x, textureRepeat.z);

	// create the index array.
	Indices = new unsigned long[indexCount]
	{
		0, 1, 3, 2, 0, 3, // Front
		4, 5, 7, 6, 4, 7, // Right
		12, 13, 15, 14, 12, 15, // Back
		16, 17, 19, 18, 16, 19, // Left
		8, 9, 11, 10, 8, 11, // Top
		20, 21, 23, 22, 20, 23, // Bottom
	};

	// Calculate normals, tangent and binormals
	calculateHardNormals(vertexCount, primitiveMesh);

	//=============
	// create Mesh
	//=============

	Mesh3D* newMesh = new Mesh3D;
	newMesh->setMesh(primitiveMesh, Indices);
	newMesh->setIndexCount(indexCount);
	newMesh->setVertexCount(vertexCount);
	m_result = newMesh->build();
	if (!m_result)
	{
		return false;
	}

	// Add mesh to model
	model.addMesh(newMesh);

	return true;
}

bool PrimitiveFactory::createSkyPlane(float quadCount, float planeWidth, float maxHeight, float textureRepeat, Model& model)
{
	float quadSize, radius, constant, textureDelta;
	D3DXVECTOR3 position;
	D3DXVECTOR2 texture;
	int index, index1, index2, index3, index4;

	int vertexCount, indexCount;
	VertexData* meshData;
	VertexData* primitiveMesh;
	unsigned long* Indices;

	//====================
	// create Vertex Data
	//====================

	// create the array to hold the sky plane coordinates.
	meshData = new VertexData[(quadCount + 1) * (quadCount + 1)];
	if (!meshData)
	{
		return false;
	}

	// Determine the size of each quad on the sky plane.
	quadSize = planeWidth / quadCount;

	// Calculate the radius of the sky plane based on the width.
	radius = planeWidth / 2.0f;

	// Calculate the height constant to increment by.
	constant = maxHeight / (radius * radius);

	// Calculate the texture coordinate increment value.
	textureDelta = (float)textureRepeat / (float)quadCount;

	// Loop through the sky plane and build the coordinates based on the increment values given.
	for (int j = 0; j <= quadCount; j++)
	{
		for (int i = 0; i <= quadCount; i++)
		{
			// Calculate the vertex coordinates.
			position.x = (-radius) + ((float)i * quadSize);
			position.z = (-radius) + ((float)j * quadSize);
			position.y = maxHeight - (constant * ((position.x * position.x) + (position.z * position.z)));

			// Calculate the texture coordinates.
			texture.x = (float)i * textureDelta;
			texture.y = (float)j * textureDelta;

			// Calculate the index into the sky plane array to add this coordinate.
			index = j * (quadCount + 1) + i;

			// Add the coordinates to the sky plane array.
			meshData[index].position = position;
			meshData[index].texture = texture;
		}
	}

	//=================
	// build the Model
	//=================

	// Calculate the number of vertices in the sky plane mesh.
	vertexCount = (quadCount + 1) * (quadCount + 1) * 6;

	// set the index count to the same as the vertex count.
	indexCount = vertexCount;

	// create the vertex array.
	primitiveMesh = new VertexData[vertexCount];
	if (!primitiveMesh)
	{
		return false;
	}

	// create the index array.
	Indices = new unsigned long[indexCount];
	if (!Indices)
	{
		return false;
	}

	// initialise the index into the vertex array.
	index = 0;

	// onload the vertex and index array with the sky plane array data.
	for (int j = 0; j<quadCount; j++)
	{
		for (int i = 0; i<quadCount; i++)
		{
			index1 = j * (quadCount + 1) + i;
			index2 = j * (quadCount + 1) + (i + 1);
			index3 = (j + 1) * (quadCount + 1) + i;
			index4 = (j + 1) * (quadCount + 1) + (i + 1);

			// Triangle 1 - Upper Left
			primitiveMesh[index].position = meshData[index1].position;
			primitiveMesh[index].texture = meshData[index1].texture;
			Indices[index] = index;
			index++;

			// Triangle 1 - Upper Right
			primitiveMesh[index].position = meshData[index2].position;
			primitiveMesh[index].texture = meshData[index2].texture;
			Indices[index] = index;
			index++;

			// Triangle 1 - Bottom Left
			primitiveMesh[index].position = meshData[index3].position;
			primitiveMesh[index].texture = meshData[index3].texture;
			Indices[index] = index;
			index++;

			// Triangle 2 - Bottom Left
			primitiveMesh[index].position = meshData[index3].position;
			primitiveMesh[index].texture = meshData[index3].texture;
			Indices[index] = index;
			index++;

			// Triangle 2 - Upper Right
			primitiveMesh[index].position = meshData[index2].position;
			primitiveMesh[index].texture = meshData[index2].texture;
			Indices[index] = index;
			index++;

			// Triangle 2 - Bottom Right
			primitiveMesh[index].position = meshData[index4].position;
			primitiveMesh[index].texture = meshData[index4].texture;
			Indices[index] = index;
			index++;
		}
	}

	//=============
	// create Mesh
	//=============

	Mesh3D* newMesh = new Mesh3D;
	newMesh->setMesh(primitiveMesh, Indices);
	newMesh->setIndexCount(indexCount);
	newMesh->setVertexCount(vertexCount);
	m_result = newMesh->build();
	if (!m_result)
	{
		return false;
	}

	// Add mesh to model
	model.addMesh(newMesh);

	return true;
}

void PrimitiveFactory::calculateHardNormals(int vertexCount, VertexData* mesh)
{
	int faceCount, index;
	VertexData vertex1, vertex2, vertex3;
	D3DXVECTOR3 tangent, binormal, normal;

	// Calculate the number of faces in the model.
	faceCount = vertexCount / 3;

	// initialise the index to the model data.
	index = 0;

	// Go through all the faces and calculate the the tangent, binormal, and normal vectors.
	for (int i = 0; i<faceCount; i++)
	{
		// get the three vertices for this face from the model.
		vertex1 = mesh[index];
		index++;

		vertex2 = mesh[index];
		index++;

		vertex3 = mesh[index];
		index++;

		// Calculate tangent and binormals for the current face
		calculateTangentBinormal(vertex1, vertex2, vertex3, tangent, binormal);

		// Calculate normals using tangent and binormal
		calculateHardNormal(tangent, binormal, normal);
		mesh[index - 1].normal = normal;
		mesh[index - 2].normal = normal;
		mesh[index - 3].normal = normal;
	}
}