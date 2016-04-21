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

	void Frame()
	{
		float zMinimum, r;
		D3DXMATRIX matrix, projectionMatrix, viewMatrix;

		Camera::Instance()->Render();
		Camera::Instance()->GetViewMatrix(viewMatrix);
		DirectXManager::Instance()->GetProjectionMatrix(projectionMatrix);

		// Calculate the minimum Z distance in the frustum.
		zMinimum = -projectionMatrix._43 / projectionMatrix._33;
		r = Rendering::FarClipPlane / (Rendering::FarClipPlane - zMinimum);
		projectionMatrix._33 = r;
		projectionMatrix._43 = -r * zMinimum;

		// Create the frustum matrix from the view matrix and updated projection matrix.
		D3DXMatrixMultiply(&matrix, &viewMatrix, &projectionMatrix);

		// Calculate near plane of frustum.
		ViewPlanes_[0].a = matrix._14 + matrix._13;
		ViewPlanes_[0].b = matrix._24 + matrix._23;
		ViewPlanes_[0].c = matrix._34 + matrix._33;
		ViewPlanes_[0].d = matrix._44 + matrix._43;
		D3DXPlaneNormalize(&ViewPlanes_[0], &ViewPlanes_[0]);

		// Calculate far plane of frustum.
		ViewPlanes_[1].a = matrix._14 - matrix._13;
		ViewPlanes_[1].b = matrix._24 - matrix._23;
		ViewPlanes_[1].c = matrix._34 - matrix._33;
		ViewPlanes_[1].d = matrix._44 - matrix._43;
		D3DXPlaneNormalize(&ViewPlanes_[1], &ViewPlanes_[1]);

		// Calculate left plane of frustum.
		ViewPlanes_[2].a = matrix._14 + matrix._11;
		ViewPlanes_[2].b = matrix._24 + matrix._21;
		ViewPlanes_[2].c = matrix._34 + matrix._31;
		ViewPlanes_[2].d = matrix._44 + matrix._41;
		D3DXPlaneNormalize(&ViewPlanes_[2], &ViewPlanes_[2]);

		// Calculate right plane of frustum.
		ViewPlanes_[3].a = matrix._14 - matrix._11;
		ViewPlanes_[3].b = matrix._24 - matrix._21;
		ViewPlanes_[3].c = matrix._34 - matrix._31;
		ViewPlanes_[3].d = matrix._44 - matrix._41;
		D3DXPlaneNormalize(&ViewPlanes_[3], &ViewPlanes_[3]);

		// Calculate top plane of frustum.
		ViewPlanes_[4].a = matrix._14 - matrix._12;
		ViewPlanes_[4].b = matrix._24 - matrix._22;
		ViewPlanes_[4].c = matrix._34 - matrix._32;
		ViewPlanes_[4].d = matrix._44 - matrix._42;
		D3DXPlaneNormalize(&ViewPlanes_[4], &ViewPlanes_[4]);

		// Calculate bottom plane of frustum.
		ViewPlanes_[5].a = matrix._14 + matrix._12;
		ViewPlanes_[5].b = matrix._24 + matrix._22;
		ViewPlanes_[5].c = matrix._34 + matrix._32;
		ViewPlanes_[5].d = matrix._44 + matrix._42;
		D3DXPlaneNormalize(&ViewPlanes_[5], &ViewPlanes_[5]);

		return;
	}

	bool CheckPoint(D3DXVECTOR3 position)
	{
		// Check if the point is inside all six planes of the view frustum.
		for (int i = 0; i < NO_OF_PLANES; i++)
		{
			if (D3DXPlaneDotCoord(&ViewPlanes_[i], &position) < 0.0f)
			{
				return false;
			}
		}

		return true;
	}

	bool CheckCube(D3DXVECTOR3 position, float radius)
	{
		int i;

		float x = position.x;
		float y = position.y;
		float z = position.z;

		// Check if any one point of the cube is in the view frustum.
		for (i = 0; i < NO_OF_PLANES; i++)
		{
			if (D3DXPlaneDotCoord(&ViewPlanes_[i], &D3DXVECTOR3((x - radius), (y - radius), (z - radius))) >= 0.0f)
			{
				continue;
			}

			if (D3DXPlaneDotCoord(&ViewPlanes_[i], &D3DXVECTOR3((x + radius), (y - radius), (z - radius))) >= 0.0f)
			{
				continue;
			}

			if (D3DXPlaneDotCoord(&ViewPlanes_[i], &D3DXVECTOR3((x - radius), (y + radius), (z - radius))) >= 0.0f)
			{
				continue;
			}

			if (D3DXPlaneDotCoord(&ViewPlanes_[i], &D3DXVECTOR3((x + radius), (y + radius), (z - radius))) >= 0.0f)
			{
				continue;
			}

			if (D3DXPlaneDotCoord(&ViewPlanes_[i], &D3DXVECTOR3((x - radius), (y - radius), (z + radius))) >= 0.0f)
			{
				continue;
			}

			if (D3DXPlaneDotCoord(&ViewPlanes_[i], &D3DXVECTOR3((x + radius), (y - radius), (z + radius))) >= 0.0f)
			{
				continue;
			}

			if (D3DXPlaneDotCoord(&ViewPlanes_[i], &D3DXVECTOR3((x - radius), (y + radius), (z + radius))) >= 0.0f)
			{
				continue;
			}

			if (D3DXPlaneDotCoord(&ViewPlanes_[i], &D3DXVECTOR3((x + radius), (y + radius), (z + radius))) >= 0.0f)
			{
				continue;
			}

			return false;
		}

		return true;
	}

private:
	static const int NO_OF_PLANES = 6;
	D3DXPLANE ViewPlanes_[NO_OF_PLANES];
};

