#pragma once

#include <d3dx11.h>
#include <d3dx10math.h>

#include <algorithm>
#include <fstream>
#include <string>
#include <sstream>
#include <iterator>
#include <vector>

#include "Material.h"
#include "DataStructs.h"

class Mesh
{
public:
	Mesh();
	~Mesh();

	// Shutdown
	void Shutdown();

	// Rendering
	bool Render(D3D_PRIMITIVE_TOPOLOGY = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Material Handling
	bool CreateMaterial(Material* material);
	bool CreateMaterial(WCHAR* BaseTexture, WCHAR* NormalTexture = NULL, WCHAR* AlphaTexture = NULL, WCHAR* NoiseTexture = NULL, WCHAR* DistortionTexture = NULL);
	bool CreateMaterial(vector<wstring> TextureArray);

	// Rendering
	int GetIndexCount();
	Material* GetMaterial();

	// Collision
	BoundingBox GetBoundingBox();

	// Vertexing
	ID3D11Buffer* GetIndexBuffer();
	ID3D11Buffer* GetVertexBuffer();

protected:
	// Buffer Functions
	bool InitialiseBuffers();

	// Model Building
	bool BuildModelFromModelData();

	// Vector Calculations
	void CalculateTangentBinormals();
	void CalculateTangentBinormal(VertexData vertex1, VertexData vertex2, VertexData vertex3, D3DXVECTOR3& tangent, D3DXVECTOR3& binormal);
	void CalculateHardNormals();
	void CalculateHardNormal(D3DXVECTOR3 tangent, D3DXVECTOR3 binormal, D3DXVECTOR3& normal);

	// Collision Detection
	void CalculateBoundingBox();

	// Model Data
	VertexData* Mesh_;
	VertexData* ModelData_;
	unsigned long* Indices_;

	// Terrain
	ObjSize Terrain_;

	// Buffers
	ID3D11Buffer* IndexBuffer_;
	ID3D11Buffer* VertexBuffer_;

	// Data Counters
	int IndexCount_;
	int VertexCount_;

	// Material
	Material* Material_;

	// Collision
	BoundingBox BoundingBox_;

	// Flags
	bool Result_;
};