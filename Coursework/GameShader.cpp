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

bool GameShader::Initialise(HWND hwnd)
{
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

void GameShader::Render(int vertexCount, int instanceCount)
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
	DirectXManager::Instance()->GetDeviceContext()->DrawInstanced(vertexCount, instanceCount, 0, 0);

	return;
}

bool GameShader::Prepare(Material* objMaterial, Transform* objTransform)
{
	return true;
}

// Shader Management
void GameShader::AddShader(string shaderName)
{
	ShaderFiles_.push_back(shaderName);
}

void GameShader::AddLayout(LPCSTR semanticName, UINT semanticIndex, DXGI_FORMAT format, UINT inputSlot, UINT alignedByteOffset, D3D11_INPUT_CLASSIFICATION inputSlotClass, UINT instanceDataStepRate)
{
	D3D11_INPUT_ELEMENT_DESC layoutDesc;

	// Set up the description of the element
	layoutDesc.SemanticName = semanticName;
	layoutDesc.SemanticIndex = semanticIndex;
	layoutDesc.Format = format;
	layoutDesc.InputSlot = inputSlot;
	layoutDesc.AlignedByteOffset = alignedByteOffset;
	layoutDesc.InputSlotClass = inputSlotClass;
	layoutDesc.InstanceDataStepRate = instanceDataStepRate;
	
	LayoutElement_.push_back(layoutDesc);
}

void GameShader::AddSamplerState(D3D11_FILTER Filter, D3D11_TEXTURE_ADDRESS_MODE AddressUVW, FLOAT MipLODBias, UINT MaxAnisotropy, D3D11_COMPARISON_FUNC ComparisonFunc, D3DXVECTOR4 borderColor, FLOAT MinLOD, FLOAT MaxLOD)
{
	HRESULT result;
	D3D11_SAMPLER_DESC samplerDesc;
	ID3D11SamplerState* samplerState;

	// Create a description
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

	// Create the sampler state using the above description
	Result_ = DirectXManager::Instance()->GetDevice()->CreateSamplerState(&samplerDesc, &samplerState);
	if (FAILED(Result_))
	{
		OutputToDebug("Could not create sampler state");
	}

	// Add to the vector
	SampleStates_.push_back(samplerState);
}

bool GameShader::BuildShader(HWND hwnd)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* hullShaderBuffer;
	ID3D10Blob* domainShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC* polygonLayout;
	unsigned int numElements;

	// Initialise the pointers to null
	errorMessage = 0;
	vertexShaderBuffer = 0;
	hullShaderBuffer = 0;
	domainShaderBuffer = 0;
	pixelShaderBuffer = 0;

	// Make sure we have shader files to build
	if (ShaderFiles_.empty())
	{
		return false;
	}

	//================
	// Compile Shader
	//================

	for (int i = 0; i < ShaderFiles_.size(); i++)
	{
		// Find out the type and compile it
		if (ShaderFiles_[i].find(".vs") != std::string::npos)
		{
			Result_ = CompileShader(hwnd, ShaderFiles_[i], "VertexShaderMain", "vs_5_0", &vertexShaderBuffer);
			if (FAILED(Result_))
			{
				return false;
			}
			Result_ = DirectXManager::Instance()->GetDevice()->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &VertexShader_);
			if (FAILED(Result_))
			{
				return false;
			}
		}
		else if (ShaderFiles_[i].find(".ps") != std::string::npos)
		{
			Result_ = CompileShader(hwnd, ShaderFiles_[i], "PixelShaderMain", "ps_5_0", &pixelShaderBuffer);
			if (FAILED(Result_))
			{
				return false;
			}
			Result_ = DirectXManager::Instance()->GetDevice()->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &PixelShader_);
			if (FAILED(Result_))
			{
				return false;
			}
		}
		else if (ShaderFiles_[i].find(".hs") != std::string::npos)
		{
			Result_ = CompileShader(hwnd, ShaderFiles_[i], "HullShaderMain", "hs_5_0", &hullShaderBuffer);
			if (FAILED(Result_))
			{
				return false;
			}
			Result_ = DirectXManager::Instance()->GetDevice()->CreateHullShader(hullShaderBuffer->GetBufferPointer(), hullShaderBuffer->GetBufferSize(), NULL, &HullShader_);
			if (FAILED(Result_))
			{
				return false;
			}
		}
		else if (ShaderFiles_[i].find(".ds") != std::string::npos)
		{
			Result_ = CompileShader(hwnd, ShaderFiles_[i], "DomainShaderMain", "ds_5_0", &domainShaderBuffer);
			if (FAILED(Result_))
			{
				return false;
			}
			Result_ = DirectXManager::Instance()->GetDevice()->CreateDomainShader(domainShaderBuffer->GetBufferPointer(), domainShaderBuffer->GetBufferSize(), NULL, &DomainShader_);
			if (FAILED(Result_))
			{
				return false;
			}
		}
	}

	//=======================
	// Create Polygon Layout
	//=======================

	if (LayoutElement_.empty())
	{
		return false;
	}

	// Access how many layout settings we have
	numElements = LayoutElement_.size();

	// Create the layout description
	polygonLayout = new D3D11_INPUT_ELEMENT_DESC[numElements];

	for (int i = 0; i < LayoutElement_.size(); i++)
	{
		polygonLayout[i] = LayoutElement_[i];
	}
	
	// Create the vertex input layout
	Result_ = DirectXManager::Instance()->GetDevice()->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &Layout_);
	if (FAILED(Result_))
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

	ShaderFiles_.clear();
	LayoutElement_.clear();

	return true;
}

void GameShader::SendBuffersToShader()
{
	// Vertex Shader
	if (!VertexBuffers_.empty())
	{
		for (int i = 0; i < VertexBuffers_.size(); i++)
		{
			VertexBuffers_[i]->SendToRender(VertexShader, i);
		}
	}

	// Pixel Shader
	if (!PixelBuffers_.empty())
	{
		for (int i = 0; i < PixelBuffers_.size(); i++)
		{
			PixelBuffers_[i]->SendToRender(PixelShader, i);
		}
	}

	// Hull Shader
	if (!HullBuffers_.empty())
	{
		for (int i = 0; i < HullBuffers_.size(); i++)
		{
			HullBuffers_[i]->SendToRender(HullShader, i);
		}
	}

	// Domain Shader
	if (!DomainBuffers_.empty())
	{
		for (int i = 0; i < DomainBuffers_.size(); i++)
		{
			DomainBuffers_[i]->SendToRender(DomainShader, i);
		}
	}
}

void GameShader::SendTextureToShader(int shaderSlot, ID3D11ShaderResourceView* shaderTexture)
{
	// Send the texture to the shader
	DirectXManager::Instance()->GetDeviceContext()->PSSetShaderResources(shaderSlot, 1, &shaderTexture);
}

HRESULT GameShader::CompileShader(HWND hwnd, string filename, LPCSTR EntryPoint, LPCSTR Version, ID3D10Blob** Buffer)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	std::wstring output;
	
	// Create the full extension
	string compiledFilename = SHADER_DIR + filename;

	// Add the shader directory to the filename
	output = std::wstring(compiledFilename.begin(), compiledFilename.end());

	// Compile the Shader
	Result_ = D3DX11CompileFromFile(output.c_str(), NULL, NULL, EntryPoint, Version, D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, Buffer, &errorMessage, NULL);

	// Check if it failed
	if (FAILED(Result_))
	{
		if (errorMessage)
		{
			// Output the error message to a text file
			OutputShaderErrorMessage(errorMessage, hwnd, output.c_str());

			// Clean Up
			errorMessage->Release();
			errorMessage = 0;
		}
		else
		{
			// Notify the user that the file doesnt exist
			MessageBox(hwnd, output.c_str(), L"Missing Shader File", MB_OK);
		}

		return Result_;
	}

	return S_OK;
}

// Error Handling
void GameShader::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, const WCHAR* shaderFilename)
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
