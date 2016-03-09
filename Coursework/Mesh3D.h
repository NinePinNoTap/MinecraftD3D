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
	~Mesh3D();

	// Shutdown
	void Shutdown();

	// Functionality
	bool Build();

	// Setters
	void SetIndexCount(int count);
	void SetVertexCount(int count);
	void SetMesh(VertexData* mesh, unsigned long* indices);

	// Getters
	int GetIndexCount();
	BoundingBox GetBoundingBox();
	ID3D11Buffer* GetIndexBuffer();
	ID3D11Buffer* GetVertexBuffer();

protected:
	// Initialising
	bool InitialiseBuffers();

	// Vectors
	void CalculateTangentBinormals();

	// Collision Detection
	void CalculateBoundingBox();
	
	// Collision
	BoundingBox BoundingBox_;

	// Mesh3D
	VertexData* Mesh_;
	unsigned long* Indices_;

	// Data Counters
	int IndexCount_;
	int VertexCount_;

	// Buffers
	ID3D11Buffer* IndexBuffer_;
	ID3D11Buffer* VertexBuffer_;

	// Flags
	bool Result_;
};