#include "InstancedGameObject.h"


InstancedGameObject::InstancedGameObject() : GameObject()
{
	InstanceBuffer_ = 0;
	InstanceCount_ = 0;
}


InstancedGameObject::~InstancedGameObject()
{
}

bool InstancedGameObject::SendModelToPipeline(Mesh3D* objMesh)
{
	unsigned int strides[2];
	unsigned int offsets[2];
	ID3D11Buffer* bufferPointers[2];

	// Set vertex buffer stride and offset
	strides[0] = sizeof(VertexData);
	strides[1] = sizeof(InstanceData);
	offsets[0] = 0;
	offsets[1] = 0;

	// Get data buffers
	bufferPointers[0] = objMesh->GetVertexBuffer();
	bufferPointers[1] = InstanceBuffer_;

	// Set the vertex buffer to active in the input assembler so it can be rendered
	DirectXManager::Instance()->GetDeviceContext()->IASetVertexBuffers(0, 2, bufferPointers, strides, offsets);

	// Set the type of primitive that should be rendered from this vertex buffer
	DirectXManager::Instance()->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void InstancedGameObject::AddInstance(D3DXVECTOR3 position, D3DXVECTOR2 textureOffset)
{
	InstanceData instanceData;
	instanceData.position = position;
	instanceData.textureOffset = textureOffset;

	Instances_.push_back(instanceData);

	InstanceCount_ = Instances_.size();
}

void InstancedGameObject::ClearInstances()
{
	Instances_.clear();
}

void InstancedGameObject::RebuildInstanceBuffer()
{
	
}

int InstancedGameObject::GetInstanceCount()
{
	return InstanceCount_;
}

ID3D11Buffer* InstancedGameObject::GetInstanceBuffer()
{
	return InstanceBuffer_;
}