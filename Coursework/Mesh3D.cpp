#include "Mesh3D.h"

Mesh3D::Mesh3D()
{
	// Initialise pointers
	Mesh_ = 0;
	Indices_ = 0;
	IndexBuffer_ = 0;
	VertexBuffer_ = 0;
}

Mesh3D::Mesh3D(const Mesh3D& mesh)
{
	*this = mesh; // copy the value
}

Mesh3D::~Mesh3D()
{

}

// Initialising
bool Mesh3D::InitialiseBuffers()
{
	D3D11_BUFFER_DESC vertexBufferDesc;
	D3D11_BUFFER_DESC indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData;
	D3D11_SUBRESOURCE_DATA indexData;
	HRESULT result;

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
	Result_ = DirectXManager::Instance()->GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &VertexBuffer_);
	if (FAILED(Result_))
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
	Result_ = DirectXManager::Instance()->GetDevice()->CreateBuffer(&indexBufferDesc, &indexData, &IndexBuffer_);
	if (FAILED(Result_))
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
void Mesh3D::Shutdown()
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
}

// Mesh3D Building
bool Mesh3D::Build()
{
	if (!Mesh_)
		return false;

	if (!Indices_)
		return false;

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

	// Set flags
	IsActive_ = true;

	return true;
}

// Setters
void Mesh3D::SetIndexCount(int count)
{
	IndexCount_ = count;
}

void Mesh3D::SetVertexCount(int count)
{
	VertexCount_ = count;
}

void Mesh3D::SetMesh(VertexData* mesh, unsigned long* indices)
{
	Mesh_ = mesh;
	Indices_ = indices;
}

void Mesh3D::SetActive(bool flag)
{
	IsActive_ = flag;
}

// Getters
int Mesh3D::GetIndexCount()
{
	return IndexCount_;
}
int Mesh3D::GetVertexCount()
{
	return VertexCount_;
}

BoundingBox Mesh3D::GetBoundingBox()
{
	return BoundingBox_;
}

ID3D11Buffer* Mesh3D::GetIndexBuffer()
{
	return IndexBuffer_;
}

ID3D11Buffer* Mesh3D::GetVertexBuffer()
{
	return VertexBuffer_;
}

bool Mesh3D::IsActive()
{
	return IsActive_;
}

void Mesh3D::CalculateTangentBinormals()
{
	int faceCount, index;
	VertexData vertex1, vertex2, vertex3;
	D3DXVECTOR3 tangent, binormal;

	// Calculate the number of faces in the model.
	faceCount = VertexCount_ / 3;

	// Initialise the index to the model data.
	index = 0;

	// Go through all the faces and calculate the the tangent, binormal, and normal vectors.
	for (int i = 0; i < faceCount; i++)
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

// Collision Detection
void Mesh3D::CalculateBoundingBox()
{
	D3DXVECTOR3 vertexPos;

	for (int i = 0; i < VertexCount_; i++)
	{
		vertexPos = Mesh_[i].position;

		// x-axis
		if (vertexPos.x < BoundingBox_.left)
		{
			BoundingBox_.left = vertexPos.x;
		}

		else if (vertexPos.x > BoundingBox_.right)
		{
			BoundingBox_.right = vertexPos.x;
		}

		// y-axis
		if (vertexPos.y < BoundingBox_.bottom)
		{
			BoundingBox_.bottom = vertexPos.y;
		}
		else if (vertexPos.y > BoundingBox_.top)
		{
			BoundingBox_.top = vertexPos.y;
		}

		// z-axis
		if (vertexPos.z < BoundingBox_.front)
		{
			BoundingBox_.front = vertexPos.z;
		}
		else if (vertexPos.z > BoundingBox_.back)
		{
			BoundingBox_.back = vertexPos.z;
		}
	}
}