#include "ViewFrustumManager.h"

ViewFrustumManager::ViewFrustumManager()
{
}

ViewFrustumManager::~ViewFrustumManager()
{
}

void ViewFrustumManager::update()
{
	float zMinimum, r;
	D3DXMATRIX matrix, projectionMatrix, viewMatrix;

	Camera::getInstance()->render();
	Camera::getInstance()->getViewMatrix(viewMatrix);
	DirectXManager::getInstance()->getProjectionMatrix(projectionMatrix);

	// Calculate the minimum Z distance in the frustum.
	zMinimum = -projectionMatrix._43 / projectionMatrix._33;
	r = rendering::FarClipPlane / (rendering::FarClipPlane - zMinimum);
	projectionMatrix._33 = r;
	projectionMatrix._43 = -r * zMinimum;

	// create the frustum matrix from the view matrix and updated projection matrix.
	D3DXMatrixMultiply(&matrix, &viewMatrix, &projectionMatrix);

	// Calculate near plane of frustum.
	m_viewPlanes[0].a = matrix._14 + matrix._13;
	m_viewPlanes[0].b = matrix._24 + matrix._23;
	m_viewPlanes[0].c = matrix._34 + matrix._33;
	m_viewPlanes[0].d = matrix._44 + matrix._43;
	D3DXPlaneNormalize(&m_viewPlanes[0], &m_viewPlanes[0]);

	// Calculate far plane of frustum.
	m_viewPlanes[1].a = matrix._14 - matrix._13;
	m_viewPlanes[1].b = matrix._24 - matrix._23;
	m_viewPlanes[1].c = matrix._34 - matrix._33;
	m_viewPlanes[1].d = matrix._44 - matrix._43;
	D3DXPlaneNormalize(&m_viewPlanes[1], &m_viewPlanes[1]);

	// Calculate left plane of frustum.
	m_viewPlanes[2].a = matrix._14 + matrix._11;
	m_viewPlanes[2].b = matrix._24 + matrix._21;
	m_viewPlanes[2].c = matrix._34 + matrix._31;
	m_viewPlanes[2].d = matrix._44 + matrix._41;
	D3DXPlaneNormalize(&m_viewPlanes[2], &m_viewPlanes[2]);

	// Calculate right plane of frustum.
	m_viewPlanes[3].a = matrix._14 - matrix._11;
	m_viewPlanes[3].b = matrix._24 - matrix._21;
	m_viewPlanes[3].c = matrix._34 - matrix._31;
	m_viewPlanes[3].d = matrix._44 - matrix._41;
	D3DXPlaneNormalize(&m_viewPlanes[3], &m_viewPlanes[3]);

	// Calculate top plane of frustum.
	m_viewPlanes[4].a = matrix._14 - matrix._12;
	m_viewPlanes[4].b = matrix._24 - matrix._22;
	m_viewPlanes[4].c = matrix._34 - matrix._32;
	m_viewPlanes[4].d = matrix._44 - matrix._42;
	D3DXPlaneNormalize(&m_viewPlanes[4], &m_viewPlanes[4]);

	// Calculate bottom plane of frustum.
	m_viewPlanes[5].a = matrix._14 + matrix._12;
	m_viewPlanes[5].b = matrix._24 + matrix._22;
	m_viewPlanes[5].c = matrix._34 + matrix._32;
	m_viewPlanes[5].d = matrix._44 + matrix._42;
	D3DXPlaneNormalize(&m_viewPlanes[5], &m_viewPlanes[5]);

	return;
}

bool ViewFrustumManager::checkPoint(D3DXVECTOR3 position)
{
	// Check if the point is inside all six planes of the view frustum.
	for (int i = 0; i < NO_OF_PLANES; i++)
	{
		if (D3DXPlaneDotCoord(&m_viewPlanes[i], &position) < 0.0f)
		{
			return false;
		}
	}

	return true;
}

bool ViewFrustumManager::checkCube(D3DXVECTOR3 position, float radius)
{
	int i;

	float x = position.x;
	float y = position.y;
	float z = position.z;

	// Check if any one point of the cube is in the view frustum.
	for (i = 0; i < NO_OF_PLANES; i++)
	{
		if (D3DXPlaneDotCoord(&m_viewPlanes[i], &D3DXVECTOR3((x - radius), (y - radius), (z - radius))) >= 0.0f)
		{
			continue;
		}

		if (D3DXPlaneDotCoord(&m_viewPlanes[i], &D3DXVECTOR3((x + radius), (y - radius), (z - radius))) >= 0.0f)
		{
			continue;
		}

		if (D3DXPlaneDotCoord(&m_viewPlanes[i], &D3DXVECTOR3((x - radius), (y + radius), (z - radius))) >= 0.0f)
		{
			continue;
		}

		if (D3DXPlaneDotCoord(&m_viewPlanes[i], &D3DXVECTOR3((x + radius), (y + radius), (z - radius))) >= 0.0f)
		{
			continue;
		}

		if (D3DXPlaneDotCoord(&m_viewPlanes[i], &D3DXVECTOR3((x - radius), (y - radius), (z + radius))) >= 0.0f)
		{
			continue;
		}

		if (D3DXPlaneDotCoord(&m_viewPlanes[i], &D3DXVECTOR3((x + radius), (y - radius), (z + radius))) >= 0.0f)
		{
			continue;
		}

		if (D3DXPlaneDotCoord(&m_viewPlanes[i], &D3DXVECTOR3((x - radius), (y + radius), (z + radius))) >= 0.0f)
		{
			continue;
		}

		if (D3DXPlaneDotCoord(&m_viewPlanes[i], &D3DXVECTOR3((x + radius), (y + radius), (z + radius))) >= 0.0f)
		{
			continue;
		}

		return false;
	}

	return true;
}