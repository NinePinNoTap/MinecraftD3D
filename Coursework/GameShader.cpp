#include "GameShader.h"
#include "Camera.h"

GameShader::GameShader()
{
	// initialise pointers to 0
	m_vertexShader = 0;
	m_pixelShader = 0;
	m_hullShader = 0;
	m_domainShader = 0;
	m_layout = 0;
}

GameShader::GameShader(const GameShader& other)
{
}

GameShader::~GameShader()
{
}

bool GameShader::initialise(HWND hwnd)
{
	return true;
}

// terminate
void GameShader::terminate()
{
	// terminate Shaders
	if (m_vertexShader)
	{
		m_vertexShader->Release();
		m_vertexShader = 0;
	}

	if (m_pixelShader)
	{
		m_pixelShader->Release();
		m_pixelShader = 0;
	}

	if (m_hullShader)
	{
		m_hullShader->Release();
		m_hullShader = 0;
	}

	if (m_domainShader)
	{
		m_domainShader->Release();
		m_domainShader = 0;
	}

	// terminate InputManager Layout
	if (m_layout)
	{
		m_layout->Release();
		m_layout = 0;
	}

	// terminate Sample States
	if (!m_samplerStates.empty())
	{
		for (unsigned int i = 0; i < m_samplerStates.size(); i++)
		{
			m_samplerStates[i]->Release();
			m_samplerStates.erase(m_samplerStates.begin() + i);
		}
	}
}

// rendering
void GameShader::render(int indexCount)
{
	// set the vertex InputManager layout
	DirectXManager::getInstance()->getDeviceContext()->IASetInputLayout(m_layout);

	//=============
	// set Shaders
	//=============

	// Base Shaders
	DirectXManager::getInstance()->getDeviceContext()->VSSetShader(m_vertexShader, NULL, 0);
	DirectXManager::getInstance()->getDeviceContext()->PSSetShader(m_pixelShader, NULL, 0);

	// Hull and Domain if required, set to NULL if so the engine doesn't previous passed
	if (m_hullShader)
	{
		DirectXManager::getInstance()->getDeviceContext()->HSSetShader(m_hullShader, NULL, 0);
	}
	else
	{
		DirectXManager::getInstance()->getDeviceContext()->HSSetShader(NULL, NULL, 0);
	}

	if (m_domainShader)
	{
		DirectXManager::getInstance()->getDeviceContext()->DSSetShader(m_domainShader, NULL, 0);
	}
	else
	{
		DirectXManager::getInstance()->getDeviceContext()->DSSetShader(NULL, NULL, 0);
	}

	//====================
	// set Sampler States
	//====================

	for (unsigned int i = 0; i < m_samplerStates.size(); i++)
	{
		DirectXManager::getInstance()->getDeviceContext()->PSSetSamplers(i, 1, &m_samplerStates[i]);
	}

	// render the object
	DirectXManager::getInstance()->getDeviceContext()->DrawIndexed(indexCount, 0, 0);

	return;
}

void GameShader::render(int vertexCount, int instanceCount)
{
	// set the vertex InputManager layout
	DirectXManager::getInstance()->getDeviceContext()->IASetInputLayout(m_layout);

	//=============
	// set Shaders
	//=============

	// Base Shaders
	DirectXManager::getInstance()->getDeviceContext()->VSSetShader(m_vertexShader, NULL, 0);
	DirectXManager::getInstance()->getDeviceContext()->PSSetShader(m_pixelShader, NULL, 0);

	// Hull and Domain if required, set to NULL if so the engine doesn't previous passed
	if (m_hullShader)
	{
		DirectXManager::getInstance()->getDeviceContext()->HSSetShader(m_hullShader, NULL, 0);
	}
	else
	{
		DirectXManager::getInstance()->getDeviceContext()->HSSetShader(NULL, NULL, 0);
	}

	if (m_domainShader)
	{
		DirectXManager::getInstance()->getDeviceContext()->DSSetShader(m_domainShader, NULL, 0);
	}
	else
	{
		DirectXManager::getInstance()->getDeviceContext()->DSSetShader(NULL, NULL, 0);
	}

	//====================
	// set Sampler States
	//====================

	for (unsigned int i = 0; i < m_samplerStates.size(); i++)
	{
		DirectXManager::getInstance()->getDeviceContext()->PSSetSamplers(i, 1, &m_samplerStates[i]);
	}

	// render the object
	DirectXManager::getInstance()->getDeviceContext()->DrawInstanced(vertexCount, instanceCount, 0, 0);

	return;
}

bool GameShader::prepare(Material* objMaterial, Transform* objTransform)
{
	return true;
}

void GameShader::setrenderMode(ProjectionMode projMode, ViewMode viewMode)
{
	DirectXManager::getInstance()->getWorldMatrix(m_matrixBuffer.world);

	switch (projMode)
	{
		case ProjectionMode::Perspective:
			DirectXManager::getInstance()->getProjectionMatrix(m_matrixBuffer.projection);
			break;

		case ProjectionMode::Orthographic:
			DirectXManager::getInstance()->getOrthoMatrix(m_matrixBuffer.projection);
			break;
	}

	switch (viewMode)
	{
		case ViewMode::BaseView:
			Camera::getInstance()->get2DViewMatrix(m_matrixBuffer.view);
			break;

		case ViewMode::View:
			Camera::getInstance()->getViewMatrix(m_matrixBuffer.view);
			break;

		case ViewMode::Reflection:
			Camera::getInstance()->getReflectionMatrix(m_matrixBuffer.view);
			break;
	}

	Camera::getInstance()->getReflectionMatrix(m_matrixBuffer.reflection);
}

// Shader Management
void GameShader::addShader(string shaderName)
{
	m_shaderFiles.push_back(shaderName);
}

void GameShader::addLayout(LPCSTR semanticName, UINT semanticIndex, DXGI_FORMAT format, UINT inputSlot, UINT alignedByteOffset, D3D11_INPUT_CLASSIFICATION inputSlotClass, UINT instanceDataStepRate)
{
	D3D11_INPUT_ELEMENT_DESC layoutDesc;

	// set up the description of the element
	layoutDesc.SemanticName = semanticName;
	layoutDesc.SemanticIndex = semanticIndex;
	layoutDesc.Format = format;
	layoutDesc.InputSlot = inputSlot;
	layoutDesc.AlignedByteOffset = alignedByteOffset;
	layoutDesc.InputSlotClass = inputSlotClass;
	layoutDesc.InstanceDataStepRate = instanceDataStepRate;
	
	m_inputElements.push_back(layoutDesc);
}

void GameShader::addSamplerState(D3D11_FILTER Filter, D3D11_TEXTURE_ADDRESS_MODE AddressUVW, FLOAT MipLODBias, UINT MaxAnisotropy, D3D11_COMPARISON_FUNC ComparisonFunc, D3DXVECTOR4 borderColor, FLOAT MinLOD, FLOAT MaxLOD)
{
	HRESULT result;
	D3D11_SAMPLER_DESC samplerDesc;
	ID3D11SamplerState* samplerState;

	// create a description
	samplerDesc.Filter = Filter;
	samplerDesc.AddressU = AddressUVW;
	samplerDesc.AddressV = AddressUVW;
	samplerDesc.AddressW = AddressUVW;
	samplerDesc.MipLODBias = MipLODBias;
	samplerDesc.MaxAnisotropy = MaxAnisotropy;
	samplerDesc.ComparisonFunc = ComparisonFunc;
	samplerDesc.BorderColor[0] = borderColor.x;
	samplerDesc.BorderColor[1] = borderColor.y;
	samplerDesc.BorderColor[2] = borderColor.z;
	samplerDesc.BorderColor[3] = borderColor.w;
	samplerDesc.MinLOD = MinLOD;
	samplerDesc.MaxLOD = MaxLOD;

	// create the sampler state using the above description
	result = DirectXManager::getInstance()->getDevice()->CreateSamplerState(&samplerDesc, &samplerState);
	if (FAILED(result))
	{
		outputToDebug("Could not create sampler state");
	}

	// Add to the vector
	m_samplerStates.push_back(samplerState);
}

bool GameShader::buildShader(HWND hwnd)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* hullShaderBuffer;
	ID3D10Blob* domainShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC* polygonLayout;
	unsigned int numElements;

	// initialise the pointers to null
	errorMessage = 0;
	vertexShaderBuffer = 0;
	hullShaderBuffer = 0;
	domainShaderBuffer = 0;
	pixelShaderBuffer = 0;

	// Make sure we have shader files to build
	if (m_shaderFiles.empty())
	{
		return false;
	}

	//================
	// Compile Shader
	//================

	for (unsigned int i = 0; i < m_shaderFiles.size(); i++)
	{
		// Find out the type and compile it
		if (m_shaderFiles[i].find(".vs") != std::string::npos)
		{
			result = compileShader(hwnd, m_shaderFiles[i], "VertexShaderMain", "vs_5_0", &vertexShaderBuffer);
			if (FAILED(result))
			{
				return false;
			}
			result = DirectXManager::getInstance()->getDevice()->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_vertexShader);
			if (FAILED(result))
			{
				return false;
			}
		}
		else if (m_shaderFiles[i].find(".ps") != std::string::npos)
		{
			result = compileShader(hwnd, m_shaderFiles[i], "PixelShaderMain", "ps_5_0", &pixelShaderBuffer);
			if (FAILED(result))
			{
				return false;
			}
			result = DirectXManager::getInstance()->getDevice()->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_pixelShader);
			if (FAILED(result))
			{
				return false;
			}
		}
		else if (m_shaderFiles[i].find(".hs") != std::string::npos)
		{
			result = compileShader(hwnd, m_shaderFiles[i], "HullShaderMain", "hs_5_0", &hullShaderBuffer);
			if (FAILED(result))
			{
				return false;
			}
			result = DirectXManager::getInstance()->getDevice()->CreateHullShader(hullShaderBuffer->GetBufferPointer(), hullShaderBuffer->GetBufferSize(), NULL, &m_hullShader);
			if (FAILED(result))
			{
				return false;
			}
		}
		else if (m_shaderFiles[i].find(".ds") != std::string::npos)
		{
			result = compileShader(hwnd, m_shaderFiles[i], "DomainShaderMain", "ds_5_0", &domainShaderBuffer);
			if (FAILED(result))
			{
				return false;
			}
			result = DirectXManager::getInstance()->getDevice()->CreateDomainShader(domainShaderBuffer->GetBufferPointer(), domainShaderBuffer->GetBufferSize(), NULL, &m_domainShader);
			if (FAILED(result))
			{
				return false;
			}
		}
	}

	//=======================
	// create Polygon Layout
	//=======================

	if (m_inputElements.empty())
	{
		return false;
	}

	// Access how many layout settings we have
	numElements = m_inputElements.size();

	// create the layout description
	polygonLayout = new D3D11_INPUT_ELEMENT_DESC[numElements];

	for (unsigned int i = 0; i < m_inputElements.size(); i++)
	{
		polygonLayout[i] = m_inputElements[i];
	}
	
	// create the vertex input layout
	result = DirectXManager::getInstance()->getDevice()->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &m_layout);
	if (FAILED(result))
	{
		return false;
	}

	//==========
	// Clean Up
	//==========
	
	if (vertexShaderBuffer)
	{
		vertexShaderBuffer->Release();
		vertexShaderBuffer = 0;
	}

	if (pixelShaderBuffer)
	{
		pixelShaderBuffer->Release();
		pixelShaderBuffer = 0;
	}

	if (hullShaderBuffer)
	{
		hullShaderBuffer->Release();
		hullShaderBuffer = 0;
	}

	if (domainShaderBuffer)
	{
		domainShaderBuffer->Release();
		domainShaderBuffer = 0;
	}

	m_shaderFiles.clear();
	m_inputElements.clear();

	return true;
}

void GameShader::sendBuffersToShader()
{
	// Vertex Shader
	if (!m_vertexBuffers.empty())
	{
		for (unsigned int i = 0; i < m_vertexBuffers.size(); i++)
		{
			m_vertexBuffers[i]->submit(VertexShader, i);
		}
	}

	// Pixel Shader
	if (!m_pixelBuffers.empty())
	{
		for (unsigned int i = 0; i < m_pixelBuffers.size(); i++)
		{
			m_pixelBuffers[i]->submit(PixelShader, i);
		}
	}

	// Hull Shader
	if (!m_hullBuffers.empty())
	{
		for (unsigned int i = 0; i < m_hullBuffers.size(); i++)
		{
			m_hullBuffers[i]->submit(HullShader, i);
		}
	}

	// Domain Shader
	if (!m_domainBuffers.empty())
	{
		for (unsigned int i = 0; i < m_domainBuffers.size(); i++)
		{
			m_domainBuffers[i]->submit(DomainShader, i);
		}
	}
}

void GameShader::sendTextureToShader(int shaderSlot, ID3D11ShaderResourceView* shaderTexture)
{
	// Send the texture to the shader
	DirectXManager::getInstance()->getDeviceContext()->PSSetShaderResources(shaderSlot, 1, &shaderTexture);
}

HRESULT GameShader::compileShader(HWND hwnd, string filename, LPCSTR EntryPoint, LPCSTR Version, ID3D10Blob** Buffer)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	std::wstring output;
	
	// create the full extension
	string compiledFilename = Directory::Shader + filename;

	// Add the shader directory to the filename
	output = std::wstring(compiledFilename.begin(), compiledFilename.end());

	// Compile the Shader
	result = D3DX11CompileFromFile(output.c_str(), NULL, NULL, EntryPoint, Version, D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, Buffer, &errorMessage, NULL);

	// Check if it failed
	if (FAILED(result))
	{
		if (errorMessage)
		{
			// Output the error message to a text file
			outputShaderErrorMessage(errorMessage, hwnd, output.c_str());
		}
		else
		{
			// Notify the user that the file doesnt exist
			MessageBox(hwnd, output.c_str(), L"Missing Shader File", MB_OK);
		}

		return result;
	}

	return S_OK;
}

// Error Handling
void GameShader::outputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, const WCHAR* shaderFilename)
{
	char* compileErrors;
	unsigned long bufferSize, i;
	ofstream fout;

	// get a pointer to the error message text buffer
	compileErrors = (char*)(errorMessage->GetBufferPointer());

	// get the length of the message.
	bufferSize = errorMessage->GetBufferSize();

	// Open a file to write the error message to.
	fout.open("shader-error.txt");

	fout << shaderFilename << endl;

	// Write out the error message.
	for (i = 0; i<bufferSize; i++)
	{
		fout << compileErrors[i];
	}

	// Close the file.
	fout.close();

	// Release the error message.
	errorMessage->Release();
	errorMessage = 0;

	// Pop a message up on the screen to notify the user to check the text file for compile errors.
	MessageBox(hwnd, L"Error compiling shader. Check shader-error.txt for message.", shaderFilename, MB_OK);

	return;
}
