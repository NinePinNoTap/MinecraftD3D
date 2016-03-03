#pragma once

#include "Mesh.h"
#include "DataStructs.h"

class PrimitiveMesh : public Mesh
{
public:
	PrimitiveMesh();
	~PrimitiveMesh();

	bool CreateSphere(ObjSize LongLat, float Radius);
	bool CreatePlane(ObjSize PlaneSize, ObjSize NoOfTiles, float TextureRepeat);
	bool Create2DBox(ObjSize Plane);
	bool Create3DBox(ObjSize BoxSize, float TextureRepeat);
	bool CreateSkyPlane(float QuadCount, float PlaneWidth, float MaxHeight, float TextureRepeat);
};

