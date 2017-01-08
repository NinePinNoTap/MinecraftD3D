#include "Mesh3D.h"

Mesh3D::Mesh3D()
{
	// initialise pointers
	m_mesh = 0;
	m_indices = 0;
	m_indexBuffer = 0;
	m_vertexBuffer = 0;
}

Mesh3D::Mesh3D(const Mesh3D& mesh)
{
	*this = mesh; // copy the value
}

Mesh3D::~Mesh3D()
{

}

// Initialising
bool Mesh3D::initialiseBuffers()
{
	D3D11_BUFFER_DESC vertexBufferDesc;
	D3D11_BUFFER_DESC indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData;
	D3D11_SUBRESOURCE_DATA indexData;
	HRESULT result;

	// set up the description of the vertex buffer
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexData)* m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data
	vertexData.pSysMem = m_mesh;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer
	result = DirectXManager::getInstance()->getDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// set up the description of the static index buffer
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long)* m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data
	indexData.pSysMem = m_indices;

	// create the index buffer
	result = DirectXManager::getInstance()->getDevice()->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Release the arrays now that the vertex and index buffers have been created and loaded
	delete[] m_mesh;
	m_mesh = 0;

	delete[] m_indices;
	m_indices = 0;

	return true;
}

// terminate
void Mesh3D::terminate()
{
	// terminate pointers
	if (m_mesh)
	{
		delete m_mesh;
		m_mesh = 0;
	}

	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}
}

// Mesh3D Building
bool Mesh3D::build()
{
	if (!m_mesh)
		return false;

	if (!m_indices)
		return false;

	// Calculate model binormals, normals and tangents
	calculateTangentBinormals();

	// Calculate collision
	calculateBoundingBox();

	// initialise the vertex and index buffer
	m_result = initialiseBuffers();
	if (!m_result)
	{
		return false;
	}

	// set flags
	m_isActive = true;

	return true;
}

// setters
void Mesh3D::setIndexCount(int count)
{
	m_indexCount = count;
}

void Mesh3D::setVertexCount(int count)
{
	m_vertexCount = count;
}

void Mesh3D::setMesh(VertexData* mesh, unsigned long* indices)
{
	m_mesh = mesh;
	m_indices = indices;
}

void Mesh3D::setActive(bool flag)
{
	m_isActive = flag;
}

// getters
int Mesh3D::getIndexCount()
{
	return m_indexCount;
}
int Mesh3D::getVertexCount()
{
	return m_vertexCount;
}

BoundingBox Mesh3D::getBoundingBox()
{
	return m_boundingBox;
}

ID3D11Buffer* Mesh3D::getIndexBuffer()
{
	return m_indexBuffer;
}

ID3D11Buffer* Mesh3D::getVertexBuffer()
{
	return m_vertexBuffer;
}

bool Mesh3D::isActive()
{
	return m_isActive;
}

void Mesh3D::calculateTangentBinormals()
{
	int faceCount, index;
	VertexData vertex1, vertex2, vertex3;
	D3DXVECTOR3 tangent, binormal;

	// Calculate the number of faces in the model.
	faceCount = m_vertexCount / 3;

	// initialise the index to the model data.
	index = 0;

	// Go through all the faces and calculate the the tangent, binormal, and normal vectors.
	for (int i = 0; i < faceCount; i++)
	{
		// get the three vertices for this face from the model.
		vertex1 = m_mesh[index];
		index++;

		vertex2 = m_mesh[index];
		index++;

		vertex3 = m_mesh[index];
		index++;

		// Calculate tangent and binormals for the current face
		calculateTangentBinormal(vertex1, vertex2, vertex3, tangent, binormal);

		// Store the normal, tangent, and binormal for this face back in the vertex data
		m_mesh[index - 1].tangent = tangent;
		m_mesh[index - 1].binormal = binormal;

		m_mesh[index - 2].tangent = tangent;
		m_mesh[index - 2].binormal = binormal;

		m_mesh[index - 3].tangent = tangent;
		m_mesh[index - 3].binormal = binormal;
	}

	return;
}

// Collision Detection
void Mesh3D::calculateBoundingBox()
{
	D3DXVECTOR3 vertexPos;

	for (int i = 0; i < m_vertexCount; i++)
	{
		vertexPos = m_mesh[i].position;

		// x-axis
		if (vertexPos.x < m_boundingBox.left)
		{
			m_boundingBox.left = vertexPos.x;
		}

		else if (vertexPos.x > m_boundingBox.right)
		{
			m_boundingBox.right = vertexPos.x;
		}

		// y-axis
		if (vertexPos.y < m_boundingBox.bottom)
		{
			m_boundingBox.bottom = vertexPos.y;
		}
		else if (vertexPos.y > m_boundingBox.top)
		{
			m_boundingBox.top = vertexPos.y;
		}

		// z-axis
		if (vertexPos.z < m_boundingBox.front)
		{
			m_boundingBox.front = vertexPos.z;
		}
		else if (vertexPos.z > m_boundingBox.back)
		{
			m_boundingBox.back = vertexPos.z;
		}
	}
}