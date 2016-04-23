#include "InstancedSprite.h"
#include "AssetManager.h"

InstancedSprite::InstancedSprite() : Sprite()
{
	InstanceBuffer_ = 0;
	InstanceCount_ = 0;
}

InstancedSprite::~InstancedSprite()
{
}

bool InstancedSprite::Render()
{
	if (!IsActive_ || !Shader_ || !Model_)
		return true;

	// Define how we want to see the model
	Shader_->SetRenderMode(ProjectionMode::Orthographic, ViewMode::BaseView);

	// Define how we want the model to be rendered
	SetRenderModes();

	// Render Mesh
	RenderMeshes();

	// Reset Pipeline Settings
	ResetRenderModes();

	return true;
}

bool InstancedSprite::RenderMeshes()
{
	// Render the model
	for (int i = 0; i < Model_->GetMeshCount(); i++)
	{
		// Make sure the mesh is active for it to be rendered
		if (Model_->GetMesh(i)->IsActive())
		{
			// Send model to pipeline
			SendModelToPipeline(Model_->GetMesh(i));

			// Send material to shader
			Shader_->Prepare(Model_->GetMaterial(i), Transform_);

			// Render Object
			Shader_->Render(Model_->GetMesh(i)->GetVertexCount(), InstanceCount_);
		}
	}

	return true;
}

bool InstancedSprite::SendModelToPipeline(Mesh3D* objMesh)
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

	// Clean Up
	bufferPointers[0] = 0;
	bufferPointers[1] = 0;

	return true;
}

void InstancedSprite::AddInstance(InstanceData instanceData)
{
	// Add to list
	Instances_.push_back(instanceData);

	// Store number of instances
	InstanceCount_ = Instances_.size();
}

void InstancedSprite::AddInstance(D3DXVECTOR3 position, D3DXVECTOR2 textureOffset, D3DXVECTOR2 textureTotal)
{
	// Create Instance
	InstanceData instanceData;
	instanceData.position = position;
	instanceData.textureOffset = textureOffset;
	instanceData.textureTotal = textureTotal;

	// Add to list
	Instances_.push_back(instanceData);

	// Store number of instances
	InstanceCount_ = Instances_.size();
}

void InstancedSprite::ClearInstances()
{
	Instances_.clear();
}

void InstancedSprite::RebuildInstanceBuffer()
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

	// Check if we have already created instances
	if (InstanceBuffer_)
	{
		InstanceBuffer_->Release();
		InstanceBuffer_ = 0;
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

int InstancedSprite::GetInstanceCount()
{
	return InstanceCount_;
}

ID3D11Buffer* InstancedSprite::GetInstanceBuffer()
{
	return InstanceBuffer_;
}