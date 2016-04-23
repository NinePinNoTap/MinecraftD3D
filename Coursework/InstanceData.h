#pragma once

struct InstanceData
{
	InstanceData()
	{

	}
	InstanceData(D3DXVECTOR3 pos, D3DXVECTOR2 texOffset, D3DXVECTOR2 texTotal)
	{
		position = pos;
		textureOffset = texOffset;
		textureTotal = texTotal;
	}

	D3DXVECTOR3 position;
	D3DXVECTOR2 textureOffset;
	D3DXVECTOR2 textureTotal;
};