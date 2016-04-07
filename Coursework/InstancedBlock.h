#pragma once

#include "Block.h"

struct InstanceType
{
	D3DXVECTOR3 position;
	D3DXVECTOR2 textureOffset;
	D3DXVECTOR2 textureTotal;
};

class InstancedBlock : public Block
{
public:
	InstancedBlock() : Block()
	{
		m_instanceBuffer = 0;
		m_instanceCount = 0;
	}
	~InstancedBlock();

	bool Render()
	{
		// Make sure the object is active
		if (!IsActive_)
			return true;

		// Make sure we have a shader to use
		if (!Shader_)
			return true;

		if (!Model_)
			return true;

		if (m_instanceCount == 0)
		{
			return true;
		}

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
				Shader_->Render(Model_->GetMesh(i)->GetVertexCount(), m_instanceCount);
			}
		}
	}

	void AddInstance(D3DXVECTOR3 position, D3DXVECTOR2 offset)
	{
		InstanceType instance;
		instance.position = position;
		instance.textureOffset = offset;
		instance.textureTotal = D3DXVECTOR2(4, 3);

		m_Instances.push_back(instance);
	}

	int GetInstanceCount()
	{
		return m_instanceCount;
	}

	bool RebuildInstanceBuffer()
	{
		InstanceType* instances = 0;
		D3D11_BUFFER_DESC instanceBufferDesc;
		D3D11_SUBRESOURCE_DATA instanceData;
		bool result;

		if (m_instanceCount == m_Instances.size())
		{
			return false;
		}

		// Set the number of instances in the array.
		m_instanceCount = m_Instances.size();

		if (m_instanceCount == 0)
			return false;

		// Create the instance array.
		instances = new InstanceType[m_instanceCount];
		if (!instances)
		{
			return false;
		}

		for (int i = 0; i < m_instanceCount; i++)
		{
			instances[i] = m_Instances[i];
		}

		// Set up the description of the instance buffer.
		instanceBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		instanceBufferDesc.ByteWidth = sizeof(InstanceType) * m_instanceCount;
		instanceBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		instanceBufferDesc.CPUAccessFlags = 0;
		instanceBufferDesc.MiscFlags = 0;
		instanceBufferDesc.StructureByteStride = 0;

		// Give the subresource structure a pointer to the instance data.
		instanceData.pSysMem = instances;
		instanceData.SysMemPitch = 0;
		instanceData.SysMemSlicePitch = 0;

		// Create the instance buffer.
		result = DirectXManager::Instance()->GetDevice()->CreateBuffer(&instanceBufferDesc, &instanceData, &m_instanceBuffer);
		if (FAILED(result))
		{
			return false;
		}

		// Release the instance array now that the instance buffer has been created and loaded.
		delete[] instances;
		instances = 0;

		m_Instances.clear();

		return true;
	}

	bool SendModelToPipeline(Mesh3D* objMesh)
	{
		unsigned int strides[2];
		unsigned int offsets[2];
		ID3D11Buffer* bufferPointers[2];

		// Set the buffer strides.
		strides[0] = sizeof(VertexData);
		strides[1] = sizeof(InstanceType);

		// Set the buffer offsets.
		offsets[0] = 0;
		offsets[1] = 0;

		// Set the array of pointers to the vertex and instance buffers.
		bufferPointers[0] = objMesh->GetVertexBuffer();
		bufferPointers[1] = m_instanceBuffer;

		// Set the vertex buffer to active in the input assembler so it can be rendered.
		DirectXManager::Instance()->GetDeviceContext()->IASetVertexBuffers(0, 2, bufferPointers, strides, offsets);

		// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
		DirectXManager::Instance()->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		return true;
	}

private:
	ID3D11Buffer* m_instanceBuffer;
	int m_instanceCount;

	std::vector<InstanceType> m_Instances;
};

