#include "ConstantBuffer.h"

ConstantBuffer::ConstantBuffer()
{
	m_buffer = 0;
}

ConstantBuffer::~ConstantBuffer()
{
}

// terminate
void ConstantBuffer::terminate()
{
	if (m_buffer)
	{
		m_buffer->Release();
		m_buffer = 0;
	}
}

void ConstantBuffer::submit(ShaderType Type, int Slot, int NoOfBuffers)
{
	switch (Type)
	{
		case VertexShader:
			DirectXManager::getInstance()->getDeviceContext()->VSSetConstantBuffers(Slot, NoOfBuffers, &m_buffer);
			break;

		case PixelShader:
			DirectXManager::getInstance()->getDeviceContext()->PSSetConstantBuffers(Slot, NoOfBuffers, &m_buffer);
			break;

		case HullShader:
			DirectXManager::getInstance()->getDeviceContext()->HSSetConstantBuffers(Slot, NoOfBuffers, &m_buffer);
			break;

		case DomainShader:
			DirectXManager::getInstance()->getDeviceContext()->DSSetConstantBuffers(Slot, NoOfBuffers, &m_buffer);
			break;
	}
}