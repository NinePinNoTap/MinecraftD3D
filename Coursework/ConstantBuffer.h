#pragma once

#include <d3d11.h>
#include <d3dx10math.h>

#include "DirectXManager.h"
#include "ShaderBuffers.h"

class ConstantBuffer
{
public:
	ConstantBuffer();
	~ConstantBuffer();

	// terminate
	void terminate();

	// Building
	template <class T>
	bool build()
	{
		D3D11_BUFFER_DESC bufferDesc;
		HRESULT result;

		// Define the buffer description
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufferDesc.ByteWidth = sizeof(T);
		bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bufferDesc.MiscFlags = 0;
		bufferDesc.StructureByteStride = 0;

		// create the buffer
		result = DirectXManager::getInstance()->getDevice()->CreateBuffer(&bufferDesc, 0, &m_buffer);
		if (FAILED(result))
		{
			return false;
		}

		return true;
	}

	// rendering
	template <class T>
	void map(T Data)
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;

		// Lock the buffer
		DirectXManager::getInstance()->getDeviceContext()->Map(m_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

		// Copy the data to it
		CopyMemory(mappedResource.pData, &Data, sizeof(T));

		// Unlock the buffer
		DirectXManager::getInstance()->getDeviceContext()->Unmap(m_buffer, 0);
	}

	void submit(ShaderType Type, int Slot, int NoOfBuffers = 1);

private:
	ID3D11Buffer* m_buffer;
};


