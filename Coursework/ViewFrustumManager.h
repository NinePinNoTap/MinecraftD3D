#pragma once

#include <d3d11.h>
#include <d3dx10math.h>

#include "Camera.h"

#include "DirectXManager.h"
#include "Singleton.h"

class ViewFrustumManager : public Singleton<ViewFrustumManager>
{
public:
	ViewFrustumManager();
	~ViewFrustumManager();

	void update();

	bool checkPoint(D3DXVECTOR3 position);
	bool checkCube(D3DXVECTOR3 position, float radius);

private:
	static const int NO_OF_PLANES = 6;
	D3DXPLANE m_viewPlanes[NO_OF_PLANES];
};

