#pragma once

#include "GameObject.h"

struct InstanceData
{
	D3DXVECTOR3 position;
	D3DXVECTOR2 textureOffset;
	D3DXVECTOR2 textureTotal;
};

class InstancedGameObject : public GameObject
{
public:
	InstancedGameObject();
	~InstancedGameObject();

	// Frame
	bool Render();

	// Instance Management
	void AddInstance(InstanceData instanceData);
	void AddInstance(D3DXVECTOR3 position, D3DXVECTOR2 textureOffset, D3DXVECTOR2 textureTotal);
	void ClearInstances();
	void RebuildInstanceBuffer();

	// Getters
	int GetInstanceCount();
	ID3D11Buffer* GetInstanceBuffer();

protected:
	bool SendModelToPipeline(Mesh3D* objMesh);

	ID3D11Buffer* InstanceBuffer_;
	int InstanceCount_;
	std::vector<InstanceData> Instances_;
};

