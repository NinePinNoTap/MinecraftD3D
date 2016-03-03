#include "Mesh.h"
#include "DirectXManager.h"

Mesh::Mesh()
{
	// Initialise pointers
	Mesh_ = 0;
	IndexBuffer_ = 0;
	VertexBuffer_ = 0;
	Material_ = 0;
}

Mesh::~Mesh()
{
}

// Initialising
bool Mesh::InitialiseBuffers()
{
	D3D11_BUFFER_DESC vertexBufferDesc;
	D3D11_BUFFER_DESC indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData;
	D3D11_SUBRESOURCE_DATA indexData;
	HRESULT Result;

	// Set up the description of the vertex buffer
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexData)* VertexCount_;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data
	vertexData.pSysMem = Mesh_;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer
	Result = DirectXManager::Instance()->GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &VertexBuffer_);
	if (FAILED(Result))
	{
		return false;
	}

	// Set up the description of the static index buffer
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long)* IndexCount_;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data
	indexData.pSysMem = Indices_;

	// Create the index buffer
	Result = DirectXManager::Instance()->GetDevice()->CreateBuffer(&indexBufferDesc, &indexData, &IndexBuffer_);
	if (FAILED(Result))
	{
		return false;
	}

	// Release the arrays now that the vertex and index buffers have been created and loaded
	delete[] Mesh_;
	Mesh_ = 0;

	delete[] Indices_;
	Indices_ = 0;

	return true;
}

// Shutdown
void Mesh::Shutdown()
{
	// Shutdown pointers
	if (Mesh_)
	{
		delete Mesh_;
		Mesh_ = 0;
	}

	if (IndexBuffer_)
	{
		IndexBuffer_->Release();
		IndexBuffer_ = 0;
	}

	if (VertexBuffer_)
	{
		VertexBuffer_->Release();
		VertexBuffer_ = 0;
	}

	if (Material_)
	{
		Material_->Shutdown();
		delete Material_;
		Material_ = 0;
	}
}

// Rendering
bool Mesh::Render(D3D_PRIMITIVE_TOPOLOGY Topology)
{
	// Pass to the graphics pipeline for rendering
	unsigned int stride;
	unsigned int offset;

	// Set vertex buffer stride and offset
	stride = sizeof(VertexData);
	offset = 0;

	// Set the vertex buffer to active in the InputManager assembler so it can be rendered
	DirectXManager::Instance()->GetDeviceContext()->IASetVertexBuffers(0, 1, &VertexBuffer_, &stride, &offset);

	// Set the index buffer to active in the InputManager assembler so it can be rendered
	DirectXManager::Instance()->GetDeviceContext()->IASetIndexBuffer(IndexBuffer_, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer
	DirectXManager::Instance()->GetDeviceContext()->IASetPrimitiveTopology(Topology);

	return true;
}

// Model Building
bool Mesh::BuildModelFromModelData()
{
	int index, index1, index2, index3, index4;

	// Set the number of vertices
	VertexCount_ = (Terrain_.width - 1) * (Terrain_.height - 1) * 6;

	// Create the vertex array
	Mesh_ = new VertexData[VertexCount_];
	if (!Mesh_) { return false; }

	// Set the index count to the same as the vertex count
	IndexCount_ = VertexCount_;

	// Create the index array
	Indices_ = new unsigned long[IndexCount_];
	if (!Indices_) { return false; }

	// Load the terrain model with the height map terrain data.
	index = 0;

	// Look through and create the model
	for (int j = 0; j<(Terrain_.height - 1); j++)
	{
		for (int i = 0; i<(Terrain_.width - 1); i++)
		{
			// Calculate index ids
			index1 = (Terrain_.width * j) + i;          // Bottom left.
			index2 = (Terrain_.width * j) + (i + 1);      // Bottom right.
			index3 = (Terrain_.width * (j + 1)) + i;      // Upper left.
			index4 = (Terrain_.width * (j + 1)) + (i + 1);  // Upper right.

			// Upper left
			Mesh_[index] = ModelData_[index3];
			Mesh_[index].texture = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			Indices_[index] = index;
			index++;

			// Upper right
			Mesh_[index] = ModelData_[index4];
			Mesh_[index].texture = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
			Indices_[index] = index;
			index++;

			// Bottom left
			Mesh_[index] = ModelData_[index1];
			Mesh_[index].texture = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			Indices_[index] = index;
			index++;

			// Bottom left
			Mesh_[index] = ModelData_[index1];
			Mesh_[index].texture = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			Indices_[index] = index;
			index++;

			// Upper right
			Mesh_[index] = ModelData_[index4];
			Mesh_[index].texture = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
			Indices_[index] = index;
			index++;

			// Bottom right
			Mesh_[index] = ModelData_[index2];
			Mesh_[index].texture = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
			Indices_[index] = index;
			index++;
		}
	}

	// Clean Up
	delete[] ModelData_;

	return true;
}

// Vector Calculations
void Mesh::CalculateTangentBinormals()
{
	int faceCount, index;
	VertexData vertex1, vertex2, vertex3;
	D3DXVECTOR3 tangent, binormal;

	// Calculate the number of faces in the model.
	faceCount = VertexCount_ / 3;

	// Initialise the index to the model data.
	index = 0;

	// Go through all the faces and calculate the the tangent, binormal, and normal vectors.
	for (int i = 0; i<faceCount; i++)
	{
		// Get the three vertices for this face from the model.
		vertex1 = Mesh_[index];
		index++;

		vertex2 = Mesh_[index];
		index++;

		vertex3 = Mesh_[index];
		index++;

		// Calculate tangent and binormals for the current face
		CalculateTangentBinormal(vertex1, vertex2, vertex3, tangent, binormal);

		// Store the normal, tangent, and binormal for this face back in the vertex data
		Mesh_[index - 1].tangent = tangent;
		Mesh_[index - 1].binormal = binormal;

		Mesh_[index - 2].tangent = tangent;
		Mesh_[index - 2].binormal = binormal;

		Mesh_[index - 3].tangent = tangent;
		Mesh_[index - 3].binormal = binormal;
	}

	return;
}

void Mesh::CalculateTangentBinormal(VertexData vertex1, VertexData vertex2, VertexData vertex3, D3DXVECTOR3& tangent, D3DXVECTOR3& binormal)
{
	D3DXVECTOR3 vector[2];
	D3DXVECTOR2 texture[2];
	float den;
	float length;

	// Calculate the two vectors for this face
	vector[0] = vertex2.position - vertex1.position;
	vector[1] = vertex3.position - vertex1.position;

	// Calculate the tu and tv texture vectors
	texture[0].x = vertex2.texture.x - vertex1.texture.x;
	texture[0].y = vertex2.texture.y - vertex1.texture.y;

	texture[1].x = vertex3.texture.x - vertex1.texture.x;
	texture[1].y = vertex3.texture.y - vertex1.texture.y;

	// Calculate the denominator of the tangent/binormal equation
	den = 1.0f / (texture[0].x * texture[1].y - texture[1].x * texture[0].y);

	// Calculate the cross products and multiply by the coefficient to get the tangent and binormal
	tangent.x = (texture[1].y * vector[0].x - texture[0].y * vector[1].x) * den;
	tangent.y = (texture[1].y * vector[0].y - texture[0].y * vector[1].y) * den;
	tangent.z = (texture[1].y * vector[0].z - texture[0].y * vector[1].z) * den;

	binormal.x = (texture[0].x * vector[1].x - texture[1].x * vector[0].x) * den;
	binormal.y = (texture[0].x * vector[1].y - texture[1].x * vector[0].y) * den;
	binormal.z = (texture[0].x * vector[1].z - texture[1].x * vector[0].z) * den;

	// Calculate the length of this normal
	length = sqrt((tangent.x * tangent.x) + (tangent.y * tangent.y) + (tangent.z * tangent.z));

	// Normalize the tangent and then store it
	tangent.x = tangent.x / length;
	tangent.y = tangent.y / length;
	tangent.z = tangent.z / length;

	// Calculate the length of this normal
	length = sqrt((binormal.x * binormal.x) + (binormal.y * binormal.y) + (binormal.z * binormal.z));

	// Normalize the binormal and then store it
	binormal.x = binormal.x / length;
	binormal.y = binormal.y / length;
	binormal.z = binormal.z / length;

	return;
}

void Mesh::CalculateHardNormals()
{
	int faceCount, index;
	VertexData vertex1, vertex2, vertex3;
	D3DXVECTOR3 tangent, binormal, normal;

	// Calculate the number of faces in the model.
	faceCount = VertexCount_ / 3;

	// Initialise the index to the model data.
	index = 0;

	// Go through all the faces and calculate the the tangent, binormal, and normal vectors.
	for (int i = 0; i<faceCount; i++)
	{
		// Get the three vertices for this face from the model.
		vertex1 = Mesh_[index];
		index++;

		vertex2 = Mesh_[index];
		index++;

		vertex3 = Mesh_[index];
		index++;

		// Calculate tangent and binormals for the current face
		CalculateTangentBinormal(vertex1, vertex2, vertex3, tangent, binormal);

		// Calculate normals using tangent and binormal
		CalculateHardNormal(tangent, binormal, normal);
		Mesh_[index - 1].normal = normal;
		Mesh_[index - 2].normal = normal;
		Mesh_[index - 3].normal = normal;
	}
}

void Mesh::CalculateHardNormal(D3DXVECTOR3 tangent, D3DXVECTOR3 binormal, D3DXVECTOR3& normal)
{
	float length;

	// Calculate the cross product of the tangent and binormal which will give the normal vector.
	normal.x = (tangent.y * binormal.z) - (tangent.z * binormal.y);
	normal.y = (tangent.z * binormal.x) - (tangent.x * binormal.z);
	normal.z = (tangent.x * binormal.y) - (tangent.y * binormal.x);

	// Calculate the length of the normal.
	length = sqrt((normal.x * normal.x) + (normal.y * normal.y) + (normal.z * normal.z));

	// Normalize the normal.
	normal.x = normal.x / length;
	normal.y = normal.y / length;
	normal.z = normal.z / length;

	return;
}

// Collision Detection
void Mesh::CalculateBoundingBox()
{
	D3DXVECTOR3 Pos;

	for (int i = 0; i < VertexCount_; i++)
	{
		Pos = Mesh_[i].position;

		// x-axis
		if (Pos.x < BoundingBox_.left)
		{
			BoundingBox_.left = Pos.x;
		}

		else if (Pos.x > BoundingBox_.right)
		{
			BoundingBox_.right = Pos.x;
		}

		// y-axis
		if (Pos.y < BoundingBox_.bottom)
		{
			BoundingBox_.bottom = Pos.y;
		}
		else if (Pos.y > BoundingBox_.top)
		{
			BoundingBox_.top = Pos.y;
		}

		// z-axis
		if (Pos.z < BoundingBox_.front)
		{
			BoundingBox_.front = Pos.z;
		}
		else if (Pos.z > BoundingBox_.back)
		{
			BoundingBox_.back = Pos.z;
		}
	}
}

// Material Handling
bool Mesh::CreateMaterial(Material* material)
{
	// Store the material
	Material_ = material;
	if (!Material_)
	{
		return false;
	}

	return true;
}

bool Mesh::CreateMaterial(WCHAR* BaseTexture, WCHAR* NormalTexture, WCHAR* AlphaTexture, WCHAR* NoiseTexture, WCHAR* DistortionTexture)
{
	Material_ = new Material;
	if (!Material_)
	{
		return false;
	}

	// Set the textures
	if (BaseTexture)
	{
		Result_ = Material_->SetBaseTexture(BaseTexture);
		if (!Result_)
		{
			return false;
		}
	}
	if (NormalTexture)
	{
		Result_ = Material_->SetNormalTexture(NormalTexture);
		if (!Result_)
		{
			return false;
		}
	}
	if (AlphaTexture)
	{
		Result_ = Material_->SetAlphaTexture(AlphaTexture);
		if (!Result_)
		{
			return false;
		}
	}
	if (NoiseTexture)
	{
		Result_ = Material_->SetNoiseTexture(NoiseTexture);
		if (!Result_)
		{
			return false;
		}
	}
	if (DistortionTexture)
	{
		Result_ = Material_->SetDistortionTexture(DistortionTexture);
		if (!Result_)
		{
			return false;
		}
	}

	return true;
}

bool Mesh::CreateMaterial(vector<wstring> TextureArray)
{
	Material_ = new Material;
	if (!Material_)
	{
		return false;
	}

	// Set the textures
	Result_ = Material_->SetTextureArray(TextureArray);
	if (!Result_)
	{
		return false;
	}

	return true;
}

// Rendering Getters
int Mesh::GetIndexCount()
{
	return IndexCount_;
}

Material* Mesh::GetMaterial()
{
	return Material_;
}

BoundingBox Mesh::GetBoundingBox()
{
	return BoundingBox_;
}

ID3D11Buffer* Mesh::GetIndexBuffer()
{
	return IndexBuffer_;
}

ID3D11Buffer* Mesh::GetVertexBuffer()
{
	return VertexBuffer_;
}