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

	// Shutdown
	void Shutdown();

	// Building
	template <class T>
	bool BuildBuffer()
	{
		HRESULT Result_;
		D3D11_BUFFER_DESC bufferDesc;

		// Define the buffer description
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufferDesc.ByteWidth = sizeof(T);
		bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bufferDesc.MiscFlags = 0;
		bufferDesc.StructureByteStride = 0;

		// Create the buffer
		Result_ = DirectXManager::Instance()->GetDevice()->CreateBuffer(&bufferDesc, 0, &Buffer_);
		if (FAILED(Result_))
		{
			return false;
		}

		return true;
	}

	// Rendering
	template <class T>
	void MapBuffer(T Data)
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;

		// Lock the buffer
		DirectXManager::Instance()->GetDeviceContext()->Map(Buffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

		// Copy the data to it
		CopyMemory(mappedResource.pData, &Data, sizeof(T));

		// Unlock the buffer
		DirectXManager::Instance()->GetDeviceContext()->Unmap(Buffer_, 0);
	}

	void SendToShader(ShaderType Type, int Slot, int NoOfBuffers = 1);

private:
	ID3D11Buffer* Buffer_;
	HRESULT Result_;
};


