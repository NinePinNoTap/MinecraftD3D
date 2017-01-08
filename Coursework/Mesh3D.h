#pragma once

#include <d3dx11.h>
#include <d3dx10math.h>

#include "DirectXManager.h"
#include "BoundingBox.h"
#include "VertexData.h"
#include "Utilities.h"

class Mesh3D
{
public:
	Mesh3D();
	Mesh3D(const Mesh3D& mesh);
	~Mesh3D();

	// terminate
	void terminate();

	// Functionality
	bool build();

	// setters
	void setIndexCount(int count);
	void setVertexCount(int count);
	void setMesh(VertexData* mesh, unsigned long* indices);
	void setActive(bool flag);

	// getters
	int getIndexCount();
	int getVertexCount();
	ID3D11Buffer* getIndexBuffer();
	ID3D11Buffer* getVertexBuffer();
	BoundingBox getBoundingBox();
	bool isActive();

protected:
	// Initialising
	bool initialiseBuffers();

	// Vectors
	void calculateTangentBinormals();

	// Collision Detection
	void calculateBoundingBox();
	
	// Collision
	BoundingBox m_boundingBox;

	// Mesh3D
	VertexData* m_mesh;
	unsigned long* m_indices;

	// Data Counters
	int m_indexCount;
	int m_vertexCount;

	// Buffers
	ID3D11Buffer* m_indexBuffer;
	ID3D11Buffer* m_vertexBuffer;

	// Flags
	bool m_result;
	bool m_isActive;
};