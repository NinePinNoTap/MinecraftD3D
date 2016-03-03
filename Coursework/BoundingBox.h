#pragma once

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

	float left;
	float right;
	float top;
	float bottom;
	float front;
	float back;
};