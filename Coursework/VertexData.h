#pragma once

#include <d3d11.h>
#include <d3dx10math.h>

// Data Struct for Representing Vertex Data
struct VertexData
{
	VertexData()
	{
		position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		texture = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		tangent = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		binormal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	D3DXVECTOR3 position;
	D3DXVECTOR3 texture;
	D3DXVECTOR3 normal;
	D3DXVECTOR3 tangent;
	D3DXVECTOR3 binormal;
};
