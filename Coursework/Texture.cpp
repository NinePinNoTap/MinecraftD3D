#include "Texture.h"
#include "DirectXManager.h"

Texture::Texture()
{
	// Initialise pointers
	RenderTargetTexture_ = 0;
	RenderTargetView_ = 0;
	Texture_ = 0;
	DepthStencilBuffer_ = 0;
	DepthStencilView_ = 0;
}

Texture::Texture(const Texture& other)
{
}

Texture::~Texture()
{
}

// Initialising
bool Texture::Initialise(string filename)
{
	HRESULT Result;

	// Convert to correct format
	std::wstring textureFilename = std::wstring(filename.begin(), filename.end());

	// Load the texture in
	Result = D3DX11CreateShaderResourceViewFromFile(DirectXManager::Instance()->GetDevice(), textureFilename.c_str(), NULL, NULL, &Texture_, NULL);
	if (FAILED(Result)) { return false; }

	return true;
}

bool Texture::Initialise(Rect2D textureResolution)
{
	D3D11_TEXTURE2D_DESC textureDesc;
	HRESULT Result;
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;

	// Initialise the render target texture description.
	ZeroMemory(&textureDesc, sizeof(textureDesc));

	// Setup the render target texture description.
	textureDesc.Width = textureResolution.width;
	textureDesc.Height = textureResolution.height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
    textureDesc.MiscFlags = 0;

	// Create the render target texture.
	Result = DirectXManager::Instance()->GetDevice()-> CreateTexture2D(&textureDesc, NULL, &RenderTargetTexture_);
	if(FAILED(Result)) { return false; }

	// Setup the description of the render target view.
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	// Create the render target view.
	Result = DirectXManager::Instance()->GetDevice() -> CreateRenderTargetView(RenderTargetTexture_, &renderTargetViewDesc, &RenderTargetView_);
	if(FAILED(Result)) { return false; }

	// Setup the description of the shader resource view.
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	// Create the shader resource view.
	Result = DirectXManager::Instance()->GetDevice() -> CreateShaderResourceView(RenderTargetTexture_, &shaderResourceViewDesc, &Texture_);
	if(FAILED(Result)) { return false; }

	// Initialise the description of the depth buffer.
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	// Set up the description of the depth buffer.
	depthBufferDesc.Width = textureResolution.width;
	depthBufferDesc.Height = textureResolution.height;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	// Create the texture for the depth buffer using the filled out description.
	Result = DirectXManager::Instance()->GetDevice() -> CreateTexture2D(&depthBufferDesc, NULL, &DepthStencilBuffer_);
	if(FAILED(Result)) { return false; }

	// Initailze the depth stencil view description.
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// Set up the depth stencil view description.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// Create the depth stencil view.
	Result = DirectXManager::Instance()->GetDevice() -> CreateDepthStencilView(DepthStencilBuffer_, &depthStencilViewDesc, &DepthStencilView_);
	if(FAILED(Result)) { return false; }

	// Setup the viewport for rendering.
	Viewport_.Width = (float)textureResolution.width;
	Viewport_.Height = (float)textureResolution.height;
    Viewport_.MinDepth = 0.0f;
    Viewport_.MaxDepth = 1.0f;
    Viewport_.TopLeftX = 0.0f;
    Viewport_.TopLeftY = 0.0f;

	// Setup the projection matrix.
	D3DXMatrixPerspectiveFovLH(&ProjectionMatrix_, ((float)D3DX_PI / 4.0f), ((float)textureResolution.width / (float)textureResolution.height), SCREEN_NEAR, SCREEN_DEPTH);

	// Create an orthographic projection matrix for 2D rendering.
	D3DXMatrixOrthoLH(&OrthoMatrix_, (float)textureResolution.width, (float)textureResolution.height, SCREEN_NEAR, SCREEN_DEPTH);

	return true;
}

// Shutdown
void Texture::Shutdown()
{
	if(DepthStencilView_)
	{
		DepthStencilView_ -> Release();
		DepthStencilView_ = 0;
	}

	if(DepthStencilBuffer_)
	{
		DepthStencilBuffer_ -> Release();
		DepthStencilBuffer_ = 0;
	}

	if(Texture_)
	{
		Texture_ -> Release();
		Texture_ = 0;
	}

	if(RenderTargetView_)
	{
		RenderTargetView_ -> Release();
		RenderTargetView_ = 0;
	}

	if(RenderTargetTexture_)
	{
		RenderTargetTexture_ -> Release();
		RenderTargetTexture_ = 0;
	}

	return;
}

void Texture::SetRenderTarget()
{
	// Bind the render target view and depth stencil buffer to the output render pipeline.
	DirectXManager::Instance()->GetDeviceContext()->OMSetRenderTargets(1, &RenderTargetView_, DepthStencilView_);
	
	// Set the viewport.
	DirectXManager::Instance()->GetDeviceContext()->RSSetViewports(1, &Viewport_);
	
	return;
}

void Texture::ClearRenderTarget(Colour ClearColour)
{
	// Convert our colour struct to a readable format for deviceContext
	float color[4] = { ClearColour.r, ClearColour.g, ClearColour.b, ClearColour.a };

	// Clear the back buffer to the colour
	DirectXManager::Instance()->GetDeviceContext()->ClearRenderTargetView(RenderTargetView_, color);
    
	// Clear the depth buffer
	DirectXManager::Instance()->GetDeviceContext()->ClearDepthStencilView(DepthStencilView_, D3D11_CLEAR_DEPTH, 1.0f, 0);

	return;
}

// Getters
ID3D11ShaderResourceView* Texture::GetTexture()
{
	return Texture_;
}

void Texture::GetProjectionMatrix(D3DXMATRIX& projectionMatrix)
{
	projectionMatrix = ProjectionMatrix_;
	return;
}

void Texture::GetOrthoMatrix(D3DXMATRIX& orthoMatrix)
{
	orthoMatrix = OrthoMatrix_;
	return;
}