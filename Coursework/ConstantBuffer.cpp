#include "ConstantBuffer.h"

ConstantBuffer::ConstantBuffer()
{
	Buffer_ = 0;
}

ConstantBuffer::~ConstantBuffer()
{
}

// Shutdown
void ConstantBuffer::Shutdown()
{
	if (Buffer_)
	{
		Buffer_->Release();
		Buffer_ = 0;
	}
}

void ConstantBuffer::SendToShader(ShaderType Type, int Slot, int NoOfBuffers)
{
	switch (Type)
	{
		case VertexShader:
			DirectXManager::Instance()->GetDeviceContext()->VSSetConstantBuffers(Slot, NoOfBuffers, &Buffer_);
			break;

		case PixelShader:
			DirectXManager::Instance()->GetDeviceContext()->PSSetConstantBuffers(Slot, NoOfBuffers, &Buffer_);
			break;

		case HullShader:
			DirectXManager::Instance()->GetDeviceContext()->HSSetConstantBuffers(Slot, NoOfBuffers, &Buffer_);
			break;

		case DomainShader:
			DirectXManager::Instance()->GetDeviceContext()->DSSetConstantBuffers(Slot, NoOfBuffers, &Buffer_);
			break;
	}
}