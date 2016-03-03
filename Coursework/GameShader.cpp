#include "GameShader.h"

GameShader::GameShader()
{
	// Initialise pointers to 0
	VertexShader_ = 0;
	PixelShader_ = 0;
	HullShader_ = 0;
	DomainShader_ = 0;
	Layout_ = 0;
}

GameShader::GameShader(const GameShader& other)
{
}

GameShader::~GameShader()
{
}

// Initialising
bool GameShader::InitialiseShader(HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename, WCHAR* hsFilename, WCHAR* dsFilename)
{
	HRESULT Result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* hullShaderBuffer;
	ID3D10Blob* domainShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[5];
	unsigned int numElements;

	// Initialise the pointers to null
	errorMessage = 0;
	vertexShaderBuffer = 0;
	hullShaderBuffer = 0;
	domainShaderBuffer = 0;
	pixelShaderBuffer = 0;

	//=================
	// Compile Shaders
	//=================

	// Vertex Shader
	Result = CompileShader(hwnd, vsFilename, "VertexShaderMain", "vs_5_0", &vertexShaderBuffer);
	if (FAILED(Result))
	{
		return false;
	}
	
	// Pixel Shader
	Result = CompileShader(hwnd, psFilename, "PixelShaderMain", "ps_5_0", &pixelShaderBuffer);
	if (FAILED(Result))
	{
		return false;
	}

	// Hull Shader (If we need to)
	if (hsFilename)
	{
		Result = CompileShader(hwnd, hsFilename, "HullShaderMain", "hs_5_0", &hullShaderBuffer);
		if (FAILED(Result))
		{
			return false;
		}
	}

	// Domain Shader (If we need to)
	if (dsFilename)
	{
		Result = CompileShader(hwnd, dsFilename, "DomainShaderMain", "ds_5_0", &domainShaderBuffer);
		if (FAILED(Result))
		{
			return false;
		}
	}

	//=======================
	// Create Shader Buffers
	//=======================

	Result = DirectXManager::Instance()->GetDevice()->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &VertexShader_);
	if (FAILED(Result)) { return false; }

	Result = DirectXManager::Instance()->GetDevice()->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &PixelShader_);
	if (FAILED(Result)) { return false; }

	if (hsFilename)
	{
		Result = DirectXManager::Instance()->GetDevice()->CreateHullShader(hullShaderBuffer->GetBufferPointer(), hullShaderBuffer->GetBufferSize(), NULL, &HullShader_);
		if (FAILED(Result)) { return false; }
	}
	if (dsFilename)
	{
		Result = DirectXManager::Instance()->GetDevice()->CreateDomainShader(domainShaderBuffer->GetBufferPointer(), domainShaderBuffer->GetBufferSize(), NULL, &DomainShader_);
		if (FAILED(Result)) { return false; }
	}

	//=================================
	// Create Vertex InputManager Description
	//=================================

	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	polygonLayout[2].SemanticName = "NORMAL";
	polygonLayout[2].SemanticIndex = 0;
	polygonLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[2].InputSlot = 0;
	polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[2].InstanceDataStepRate = 0;

	polygonLayout[3].SemanticName = "TANGENT";
	polygonLayout[3].SemanticIndex = 0;
	polygonLayout[3].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[3].InputSlot = 0;
	polygonLayout[3].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[3].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[3].InstanceDataStepRate = 0;

	polygonLayout[4].SemanticName = "BINORMAL";
	polygonLayout[4].SemanticIndex = 0;
	polygonLayout[4].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[4].InputSlot = 0;
	polygonLayout[4].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[4].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[4].InstanceDataStepRate = 0;

	// Count how many elements
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// Create the vertex InputManager layout
	Result = DirectXManager::Instance()->GetDevice()->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &Layout_);
	if (FAILED(Result)) { return false; }

	//==========
	// Clean Up
	//==========

	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;

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

	return true;
}

bool GameShader::CompileShader(HWND hwnd, WCHAR* filename, LPCSTR EntryPoint, LPCSTR Version, ID3D10Blob** Buffer)
{
	HRESULT Result;
	ID3D10Blob* errorMessage;

	// Compile the Shader
	Result = D3DX11CompileFromFile(filename, NULL, NULL, EntryPoint, Version, D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, Buffer, &errorMessage, NULL);

	// Check if it failed
	if (FAILED(Result))
	{
		if (errorMessage)
		{
			// Output the error message to a text file
			OutputShaderErrorMessage(errorMessage, hwnd, filename);

			// Clean Up
			errorMessage->Release();
			errorMessage = 0;
		}
		else
		{
			// Notify the user that the file doesnt exist
			MessageBox(hwnd, filename, L"Missing Shader File", MB_OK);
		}

		return false;
	}

	return true;
}

// Shutdown
void GameShader::Shutdown()
{
	// Shutdown Shaders
	if (VertexShader_)
	{
		VertexShader_->Release();
		VertexShader_ = 0;
	}

	if (PixelShader_)
	{
		PixelShader_->Release();
		PixelShader_ = 0;
	}

	if (HullShader_)
	{
		HullShader_->Release();
		HullShader_ = 0;
	}

	if (DomainShader_)
	{
		DomainShader_->Release();
		DomainShader_ = 0;
	}

	// Shutdown InputManager Layout
	if (Layout_)
	{
		Layout_->Release();
		Layout_ = 0;
	}

	// Shutdown Sample States
	if (!SampleStates_.empty())
	{
		for (int i = 0; i < SampleStates_.size(); i++)
		{
			SampleStates_[i]->Release();
			SampleStates_.erase(SampleStates_.begin() + i);
		}
	}
}

// Rendering
void GameShader::Render(int indexCount)
{
	// Set the vertex InputManager layout
	DirectXManager::Instance()->GetDeviceContext()->IASetInputLayout(Layout_);

	//=============
	// Set Shaders
	//=============

	// Base Shaders
	DirectXManager::Instance()->GetDeviceContext()->VSSetShader(VertexShader_, NULL, 0);
	DirectXManager::Instance()->GetDeviceContext()->PSSetShader(PixelShader_, NULL, 0);

	// Hull and Domain if required, set to NULL if so the engine doesn't previous passed
	if (HullShader_)
	{
		DirectXManager::Instance()->GetDeviceContext()->HSSetShader(HullShader_, NULL, 0);
	}
	else
	{
		DirectXManager::Instance()->GetDeviceContext()->HSSetShader(NULL, NULL, 0);
	}

	if (DomainShader_)
	{
		DirectXManager::Instance()->GetDeviceContext()->DSSetShader(DomainShader_, NULL, 0);
	}
	else
	{
		DirectXManager::Instance()->GetDeviceContext()->DSSetShader(NULL, NULL, 0);
	}

	//====================
	// Set Sampler States
	//====================

	for (int i = 0; i < SampleStates_.size(); i++)
	{
		DirectXManager::Instance()->GetDeviceContext()->PSSetSamplers(i, 1, &SampleStates_[i]);
	}

	// Render the object
	DirectXManager::Instance()->GetDeviceContext()->DrawIndexed(indexCount, 0, 0);

	return;
}

// Shader Management
void GameShader::AddSamplerState(D3D11_FILTER Filter, D3D11_TEXTURE_ADDRESS_MODE TextureMode)
{
	HRESULT Result;
	D3D11_SAMPLER_DESC samplerDesc;
	ID3D11SamplerState* samplerState;

	// Create a description for the second sampler state
	samplerDesc.Filter = Filter;
	samplerDesc.AddressU = TextureMode;
	samplerDesc.AddressV = TextureMode;
	samplerDesc.AddressW = TextureMode;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Create the sampler state using the above description
	Result = DirectXManager::Instance()->GetDevice()->CreateSamplerState(&samplerDesc, &samplerState);

	// Add to the vector
	SampleStates_.push_back(samplerState);
}

void GameShader::SendBuffersToShader()
{
	// Vertex Shader
	if (!VertexBuffers_.empty())
	{
		for (int i = 0; i < VertexBuffers_.size(); i++)
		{
			VertexBuffers_[i]->SendToShader(VertexShader, i);
		}
	}

	// Pixel Shader
	if (!PixelBuffers_.empty())
	{
		for (int i = 0; i < PixelBuffers_.size(); i++)
		{
			PixelBuffers_[i]->SendToShader(PixelShader, i);
		}
	}

	// Hull Shader
	if (!HullBuffers_.empty())
	{
		for (int i = 0; i < HullBuffers_.size(); i++)
		{
			HullBuffers_[i]->SendToShader(HullShader, i);
		}
	}

	// Domain Shader
	if (!DomainBuffers_.empty())
	{
		for (int i = 0; i < DomainBuffers_.size(); i++)
		{
			DomainBuffers_[i]->SendToShader(DomainShader, i);
		}
	}
}

void GameShader::SendTextureToShader(int Slot, ID3D11ShaderResourceView* Texture)
{
	// Send the texture to the shader
	DirectXManager::Instance()->GetDeviceContext()->PSSetShaderResources(Slot, 1, &Texture);
}

// Error Handling
void GameShader::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename)
{
	char* compileErrors;
	unsigned long bufferSize, i;
	ofstream fout;

	// Get a pointer to the error message text buffer
	compileErrors = (char*)(errorMessage->GetBufferPointer());

	// Get the length of the message.
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
