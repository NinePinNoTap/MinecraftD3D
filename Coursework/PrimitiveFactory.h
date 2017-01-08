#pragma once

#include <d3dx11.h>
#include <d3dx10math.h>
#include <vector>

#include "Model.h"
#include "Rect.h"
#include "Utilities.h"
#include "VertexData.h"
#include "Vector3.h"

class PrimitiveFactory
{
public:
	PrimitiveFactory();
	~PrimitiveFactory();

	bool createSphere(Rect3D LongLat, float Radius, Model& model);
	bool createPlane(Rect3D PlaneSize, Rect3D NoOfTiles, float TextureRepeat, Model& model);
	bool create2DBox(Rect3D Plane, Model& model);
	bool create3DBox(Rect3D BoxSize, float TextureRepeat, Model& model);
	bool createSkyPlane(float QuadCount, float PlaneWidth, float MaxHeight, float TextureRepeat, Model& model);

private:
	void calculateHardNormals(int vertexCount, VertexData* mesh);
	bool m_result;
};

