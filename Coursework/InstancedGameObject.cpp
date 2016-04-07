#include "InstancedGameObject.h"

InstancedGameObject::InstancedGameObject() : GameObject()
{
	InstanceBuffer_ = 0;
	InstanceCount_ = 0;
}

InstancedGameObject::~InstancedGameObject()
{
}

bool InstancedGameObject::Render()
{
	// Make sure the object is active
	if (!IsActive_)
		return true;

	// Make sure we have a shader to use
	if (!Shader_)
		return true;

	// Make sure we have a model to render
	if (!Model_)
		return true;
	
	// Make sure we have instances to render
	if (InstanceCount_ == 0)
		return true;

	// Render the model
	for (int i = 0; i < Model_->GetMeshCount(); i++)
	{
		// Make sure the mesh is active for it to be rendered
		if (Model_->GetMesh(i)->IsActive())
		{
			// Send model to pipeline
			SendModelToPipeline(Model_->GetMesh(i));

			// Send material to shader
			Shader_->Prepare(Model_->GetMesh(i), Model_->GetMaterial(i), Transform_);

			// Render Object
			Shader_->Render(Model_->GetMesh(i)->GetVertexCount(), InstanceCount_);
		}
	}
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

	return true;
}

void InstancedGameObject::AddInstance(D3DXVECTOR3 position, D3DXVECTOR2 textureOffset)
{
	InstanceData instanceData;
	instanceData.position = position;
	instanceData.textureOffset = textureOffset;
	instanceData.textureTotal = D3DXVECTOR2(4, 3);

	Instances_.push_back(instanceData);

	InstanceCount_ = Instances_.size();
}

void InstancedGameObject::ClearInstances()
{
	Instances_.clear();
}

void InstancedGameObject::RebuildInstanceBuffer()
{
	InstanceData* instances = 0;
	D3D11_BUFFER_DESC instanceBufferDesc;
	D3D11_SUBRESOURCE_DATA instanceData;
	HRESULT result;

	// Define how many instances we have
	InstanceCount_ = Instances_.size();

	// Make sure we have instances to buffer
	if (InstanceCount_ == 0)
	{
		return;
	}

	// Create the instance array.
	instances = new InstanceData[InstanceCount_];
	if (!instances)
	{
		return;
	}

	for (int i = 0; i < InstanceCount_; i++)
	{
		instances[i] = Instances_[i];
	}

	// Set up the description of the instance buffer.
	instanceBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	instanceBufferDesc.ByteWidth = sizeof(InstanceData) * InstanceCount_;
	instanceBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	instanceBufferDesc.CPUAccessFlags = 0;
	instanceBufferDesc.MiscFlags = 0;
	instanceBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the instance data.
	instanceData.pSysMem = instances;
	instanceData.SysMemPitch = 0;
	instanceData.SysMemSlicePitch = 0;

	// Create the instance buffer.
	result = DirectXManager::Instance()->GetDevice()->CreateBuffer(&instanceBufferDesc, &instanceData, &InstanceBuffer_);
	if (FAILED(result))
	{
		return;
	}

	//==========
	// Clean Up
	//==========

	delete[] instances;
	instances = 0;

	Instances_.clear();
}

int InstancedGameObject::GetInstanceCount()
{
	return InstanceCount_;
}

ID3D11Buffer* InstancedGameObject::GetInstanceBuffer()
{
	return InstanceBuffer_;
}