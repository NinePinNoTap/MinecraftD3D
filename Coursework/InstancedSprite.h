#pragma once

#include "Sprite.h"
#include "InstanceData.h"

class InstancedSprite : public Sprite
{
public:
	InstancedSprite();
	~InstancedSprite();

	bool Render();

	// Instance Management
	void AddInstance(InstanceData instanceData);
	void AddInstance(D3DXVECTOR3 position, D3DXVECTOR2 textureOffset, D3DXVECTOR2 textureTotal);
	void ClearInstances();
	void RebuildInstanceBuffer();

	// Getters
	int GetInstanceCount();
	ID3D11Buffer* GetInstanceBuffer();

private:
	bool RenderMeshes();
	bool SendModelToPipeline(Mesh3D* objMesh);

	ID3D11Buffer* InstanceBuffer_;
	int InstanceCount_;
	std::vector<InstanceData> Instances_;
};

