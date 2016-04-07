#pragma once

#include <d3d11.h>
#include <d3dx10math.h>
#include <d3dx11async.h>
#include <fstream>
#include <vector>

#include "ConstantBuffer.h"
#include "DirectXManager.h"
#include "Material.h"
#include "Mesh3D.h"
#include "Utilities.h"
#include "Transform.h"
#include "ShaderBuffers.h"

using namespace std;

class GameShader
{
public:
	GameShader();
	GameShader(const GameShader&);
	~GameShader();
	
	virtual bool Initialise(HWND hwnd) { return true; }

	// Shutdown
	void Shutdown();

	// Rendering
	void Render(int indexCount);
	void Render(int vertexCount, int instanceCount);

	// Rendering
	virtual bool Prepare(Mesh3D* objMesh, Material* objMaterial, Transform* objTransform = 0);

	// Shader Data Management
	void AddShader(string shaderName);
	void AddLayout(LPCSTR semanticName, UINT semanticIndex, DXGI_FORMAT format, UINT inputSlot, UINT alignedByteOffset, D3D11_INPUT_CLASSIFICATION inputSlotClass, UINT instanceDataStepRate);
	void AddSamplerState(D3D11_FILTER Filter, D3D11_TEXTURE_ADDRESS_MODE AddressUVW, FLOAT MipLODBias, UINT MaxAnisotropy, D3D11_COMPARISON_FUNC ComparisonFunc, D3DXVECTOR4 borderColor, FLOAT MinLOD, FLOAT MaxLOD);
	bool BuildShader(HWND hwnd);
	
	// Buffer Management
	template <class T>
	void AddBuffer(ShaderType type)
	{
		// Create a buffer
		ConstantBuffer* tempBuffer = new ConstantBuffer;
		tempBuffer->BuildBuffer<T>();

		// Add to the required shader
		switch (type)
		{
			case VertexShader:
				VertexBuffers_.push_back(tempBuffer);
				break;

			case PixelShader:
				PixelBuffers_.push_back(tempBuffer);
				break;

			case HullShader:
				HullBuffers_.push_back(tempBuffer);
				break;

			case DomainShader:
				DomainBuffers_.push_back(tempBuffer);
				break;
		}
	}

	template <class T>
	void UpdateBuffer(ShaderType shaderType, int bufferSlot, T bufferData)
	{
		switch (shaderType)
		{
			case VertexShader:
				VertexBuffers_[bufferSlot]->MapBuffer(bufferData);
				break;

			case PixelShader:
				PixelBuffers_[bufferSlot]->MapBuffer(bufferData);
				break;

			case HullShader:
				HullBuffers_[bufferSlot]->MapBuffer(bufferData);
				break;

			case DomainShader:
				DomainBuffers_[bufferSlot]->MapBuffer(bufferData);
				break;
		}
	}

	// Shader Management
	void SendBuffersToShader();
	void SendTextureToShader(int shaderSlot, ID3D11ShaderResourceView* shaderTexture);

protected:
	// Error Handling
	void OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, const WCHAR* shaderFilename);

	// Set up
	HRESULT CompileShader(HWND hwnd, string filename, LPCSTR EntryPoint, LPCSTR Version, ID3D10Blob** Buffer);
	bool Result_;
	
	// Shader
	ID3D11VertexShader* VertexShader_;
	ID3D11PixelShader* PixelShader_;
	ID3D11HullShader* HullShader_;
	ID3D11DomainShader* DomainShader_;

	// Interface
	ID3D11InputLayout* Layout_;

	// Storage for compiling the shader
	vector<string> ShaderFiles_;
	vector<D3D11_INPUT_ELEMENT_DESC> LayoutElement_;
	vector<ID3D11SamplerState*> SampleStates_;

	// Buffers
	vector<ConstantBuffer*> VertexBuffers_;
	vector<ConstantBuffer*> PixelBuffers_;
	vector<ConstantBuffer*> HullBuffers_;
	vector<ConstantBuffer*> DomainBuffers_;
};