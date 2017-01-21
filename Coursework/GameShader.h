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

enum ProjectionMode { Perspective, Orthographic};
enum ViewMode { BaseView, View, Reflection };

using namespace std;

class GameShader
{
public:
	GameShader();
	GameShader(const GameShader&);
	~GameShader();
	
	virtual bool initialise(HWND hwnd);

	// terminate
	void terminate();

	// rendering
	void render(int indexCount);
	void render(int vertexCount, int instanceCount);

	// rendering
	virtual bool prepare(Material* objMaterial, Transform* objTransform = 0);

	// Properties
	void setRenderMode(ProjectionMode projMode, ViewMode viewMode);

	// Shader Data Management
	void addShader(string shaderName);
	void addLayout(LPCSTR semanticName, UINT semanticIndex, DXGI_FORMAT format, UINT inputSlot, UINT alignedByteOffset, D3D11_INPUT_CLASSIFICATION inputSlotClass, UINT instanceDataStepRate);
	void addSamplerState(D3D11_FILTER Filter, D3D11_TEXTURE_ADDRESS_MODE AddressUVW, FLOAT MipLODBias, UINT MaxAnisotropy, D3D11_COMPARISON_FUNC ComparisonFunc, D3DXVECTOR4 borderColor, FLOAT MinLOD, FLOAT MaxLOD);
	bool buildShader(HWND hwnd);

	// Shader Management
	void sendBuffersToShader();
	void sendTextureToShader(int shaderSlot, ID3D11ShaderResourceView* shaderTexture);

	// Buffer Management
	template <class T>
	void addBuffer(ShaderType type)
	{
		// create a buffer
		ConstantBuffer* tempBuffer = new ConstantBuffer;
		tempBuffer->build<T>();

		// Add to the required shader
		switch (type)
		{
			case VertexShader:
				m_vertexBuffers.push_back(tempBuffer);
				break;

			case PixelShader:
				m_pixelBuffers.push_back(tempBuffer);
				break;

			case HullShader:
				m_hullBuffers.push_back(tempBuffer);
				break;

			case DomainShader:
				m_domainBuffers.push_back(tempBuffer);
				break;
		}
	}

	template <class T>
	void updateBuffer(ShaderType shaderType, int bufferSlot, T bufferData)
	{
		switch (shaderType)
		{
			case VertexShader:
				m_vertexBuffers[bufferSlot]->map(bufferData);
				break;

			case PixelShader:
				m_pixelBuffers[bufferSlot]->map(bufferData);
				break;

			case HullShader:
				m_hullBuffers[bufferSlot]->map(bufferData);
				break;

			case DomainShader:
				m_domainBuffers[bufferSlot]->map(bufferData);
				break;
		}
	}

protected:
	// Error Handling
	void outputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, const WCHAR* shaderFilename);

	// set up
	HRESULT compileShader(HWND hwnd, string filename, LPCSTR EntryPoint, LPCSTR Version, ID3D10Blob** Buffer);
	bool m_result;
	
	// Shader
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11HullShader* m_hullShader;
	ID3D11DomainShader* m_domainShader;

	// Interface
	ID3D11InputLayout* m_layout;

	// Storage for compiling the shader
	vector<string> m_shaderFiles;
	vector<D3D11_INPUT_ELEMENT_DESC> m_inputElements;
	vector<ID3D11SamplerState*> m_samplerStates;

	// Buffers
	vector<ConstantBuffer*> m_vertexBuffers;
	vector<ConstantBuffer*> m_pixelBuffers;
	vector<ConstantBuffer*> m_hullBuffers;
	vector<ConstantBuffer*> m_domainBuffers;

	// Matrix Buffer
	MatrixBuffer m_matrixBuffer;
};