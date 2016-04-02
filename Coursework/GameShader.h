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

	// Initialising
	bool InitialiseRender(HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename, WCHAR* hsFilename = 0, WCHAR* dsFilename = 0);

	// Shutdown
	void Shutdown();

	// Rendering
	void Render(int indexCount);

	// Shader Data Management
	void AddSamplerState(D3D11_FILTER Filter, D3D11_TEXTURE_ADDRESS_MODE TextureMode);

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

	// Rendering
	virtual bool Prepare(Mesh3D* objMesh, Material* objMaterial, Transform* objTransform)
	{
		return true;
	}

	void SendBuffersToShader();
	void SendTextureToShader(int Slot, ID3D11ShaderResourceView* Texture);

protected:
	// Error Handling
	void OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename);

	// Set up
	bool CompileShader(HWND hwnd, WCHAR* filename, LPCSTR EntryPoint, LPCSTR Version, ID3D10Blob** Buffer);

	// Shader
	ID3D11VertexShader* VertexShader_;
	ID3D11PixelShader* PixelShader_;
	ID3D11HullShader* HullShader_;
	ID3D11DomainShader* DomainShader_;

	// Interface
	ID3D11InputLayout* Layout_;

	// Sample State
	vector<ID3D11SamplerState*> SampleStates_;

	// Buffers
	vector<ConstantBuffer*> VertexBuffers_;
	vector<ConstantBuffer*> PixelBuffers_;
	vector<ConstantBuffer*> HullBuffers_;
	vector<ConstantBuffer*> DomainBuffers_;
};


