#pragma once

#include <d3dx11.h>
#include <d3dx10math.h>

#include "Rect.h"

// Data Struct for Representing a Bounding Box
struct BoundingBox
{
	BoundingBox()
	{
		left = 0.0f;
		right = 0.0f;
		top = 0.0f;
		bottom = 0.0f;
		front = 0.0f;
		back = 0.0f;
	}

	BoundingBox(Rect3D rect, D3DXVECTOR3 position)
	{
		left = position.x - (rect.width / 2.0f);
		right = position.x + (rect.width / 2.0f);
		top = position.y + (rect.height / 2.0f);
		bottom = position.y - (rect.height / 2.0f);
		front = position.z - (rect.depth / 2.0f);
		back = position.z + (rect.depth / 2.0f);
	}

	void SetPosition(D3DXVECTOR3 position)
	{
		left += position.x;
		right += position.x;
		top += position.y;
		bottom += position.y;
		front += position.z;
		back += position.z;
	}

	float left;
	float right;
	float top;
	float bottom;
	float front;
	float back;
};