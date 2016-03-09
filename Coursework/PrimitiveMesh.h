#pragma once

#include <vector>

#include "Mesh3D.h"
#include "Rect.h"
#include "Vector3.h"

class PrimitiveMesh : public Mesh3D
{
public:
	PrimitiveMesh();
	~PrimitiveMesh();

	bool CreateSphere(Rect3D LongLat, float Radius);
	bool CreatePlane(Rect3D PlaneSize, Rect3D NoOfTiles, float TextureRepeat);
	bool Create2DBox(Rect3D Plane);
	bool Create3DBox(Rect3D BoxSize, float TextureRepeat);
	bool CreateSkyPlane(float QuadCount, float PlaneWidth, float MaxHeight, float TextureRepeat);
};

