#include "Texture.h"
#include "DirectXManager.h"

Texture::Texture()
{
	// initialise pointers
	m_renderTargetTexture = 0;
	m_renderTargetView = 0;
	m_texture = 0;
	m_depthStencilBuffer = 0;
	m_depthStencilView = 0;
}

Texture::Texture(const Texture& other)
{
}

Texture::~Texture()
{
}

// Initialising
bool Texture::initialise(string filename)
{
	HRESULT result;

	// Convert to correct format
	std::wstring textureFilename = std::wstring(filename.begin(), filename.end());

	// onload the texture in
	result = D3DX11CreateShaderResourceViewFromFile(DirectXManager::getInstance()->getDevice(), textureFilename.c_str(), NULL, NULL, &m_texture, NULL);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

bool Texture::initialise(Rect2D textureResolution)
{
	D3D11_TEXTURE2D_DESC textureDesc;
	HRESULT result;
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	float aspectRatio;

	// initialise the render target texture description.
	ZeroMemory(&textureDesc, sizeof(textureDesc));

	// setup the render target texture description.
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

	// create the render target texture.
	result = DirectXManager::getInstance()->getDevice()-> CreateTexture2D(&textureDesc, NULL, &m_renderTargetTexture);
	if(FAILED(result))
	{
		return false;
	}

	// setup the description of the render target view.
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	// create the render target view.
	result = DirectXManager::getInstance()->getDevice()->CreateRenderTargetView(m_renderTargetTexture, &renderTargetViewDesc, &m_renderTargetView);
	if (FAILED(result))
	{
		return false;
	}

	// setup the description of the shader resource view.
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	// create the shader resource view.
	result = DirectXManager::getInstance()->getDevice()->CreateShaderResourceView(m_renderTargetTexture, &shaderResourceViewDesc, &m_texture);
	if (FAILED(result))
	{
		return false;
	}

	// initialise the description of the depth buffer.
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	// set up the description of the depth buffer.
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

	// create the texture for the depth buffer using the filled out description.
	result = DirectXManager::getInstance()->getDevice()->CreateTexture2D(&depthBufferDesc, NULL, &m_depthStencilBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Initailze the depth stencil view description.
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// set up the depth stencil view description.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// create the depth stencil view.
	result = DirectXManager::getInstance()->getDevice()->CreateDepthStencilView(m_depthStencilBuffer, &depthStencilViewDesc, &m_depthStencilView);
	if (FAILED(result))
	{
		return false;
	}

	// setup the viewport for rendering.
	m_viewport.Width = (float)textureResolution.width;
	m_viewport.Height = (float)textureResolution.height;
    m_viewport.MinDepth = 0.0f;
    m_viewport.MaxDepth = 1.0f;
    m_viewport.TopLeftX = 0.0f;
    m_viewport.TopLeftY = 0.0f;

	aspectRatio = ((float)textureResolution.width / (float)textureResolution.height);

	// setup the projection matrix.
	D3DXMatrixPerspectiveFovLH(&m_projectionMatrix, ((float)D3DX_PI / 4.0f), aspectRatio, rendering::NearClipPlane, rendering::FarClipPlane);

	// create an orthographic projection matrix for 2D rendering.
	D3DXMatrixOrthoLH(&m_orthoMatrix, (float)textureResolution.width, (float)textureResolution.height, rendering::NearClipPlane, rendering::FarClipPlane);

	return true;
}

// terminate
void Texture::terminate()
{
	if(m_depthStencilView)
	{
		m_depthStencilView->Release();
		m_depthStencilView = 0;
	}

	if(m_depthStencilBuffer)
	{
		m_depthStencilBuffer->Release();
		m_depthStencilBuffer = 0;
	}

	if(m_texture)
	{
		m_texture->Release();
		m_texture = 0;
	}

	if(m_renderTargetView)
	{
		m_renderTargetView->Release();
		m_renderTargetView = 0;
	}

	if(m_renderTargetTexture)
	{
		m_renderTargetTexture->Release();
		m_renderTargetTexture = 0;
	}

	return;
}

void Texture::setRenderTarget()
{
	// Bind the render target view and depth stencil buffer to the output render pipeline.
	DirectXManager::getInstance()->getDeviceContext()->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);
	
	// set the viewport.
	DirectXManager::getInstance()->getDeviceContext()->RSSetViewports(1, &m_viewport);
	
	return;
}

void Texture::clearRenderTarget(D3DXVECTOR4 clearColour)
{
	// Clear the back buffer to the colour
	DirectXManager::getInstance()->getDeviceContext()->ClearRenderTargetView(m_renderTargetView, clearColour);
    
	// Clear the depth buffer
	DirectXManager::getInstance()->getDeviceContext()->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	return;
}

// getters
ID3D11ShaderResourceView* Texture::getTexture()
{
	return m_texture;
}

void Texture::getProjectionMatrix(D3DXMATRIX& projectionMatrix)
{
	projectionMatrix = m_projectionMatrix;
	return;
}

void Texture::getOrthoMatrix(D3DXMATRIX& orthoMatrix)
{
	orthoMatrix = m_orthoMatrix;
	return;
}