#pragma once

#include "Sprite.h"
#include "InstanceData.h"

class InstancedSprite : public Sprite
{
public:
	InstancedSprite();
	~InstancedSprite();

	bool render();

	// getInstance Management
	void addInstance(InstanceData instanceData);
	void addInstance(D3DXVECTOR3 position, D3DXVECTOR2 textureOffset, D3DXVECTOR2 textureTotal);
	void clearInstances();
	void rebuildInstanceBuffer();

	// getters
	int getInstanceCount();
	ID3D11Buffer* getInstanceBuffer();

private:
	bool renderMeshes();
	bool sendModelToPipeline(Mesh3D* objMesh);

	ID3D11Buffer* m_instanceBuffer;
	int m_instanceCount;
	std::vector<InstanceData> m_instances;
};

