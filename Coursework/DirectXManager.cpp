#include "DirectXManager.h"

DirectXManager::DirectXManager()
{
	// Initiase pointers to null
	SwapChain_ = 0;
	Device_ = 0;
	DeviceContext_ = 0;
	RenderTargetView_ = 0;
	DepthStencilBuffer_ = 0;
	DepthStencilState_ = 0;
	DepthStencilView = 0;
	RasterState_ = 0;
	RasterStateNoCulling_ = 0;
	DepthDisabledStencilState_ = 0;
	AlphaEnableBlendingState_ = 0;
	NormalBlendingState_ = 0;
	AlphaCloudBlendState_ = 0;

	// Flag that wireframe mode is off
	Wireframe_ = false;
}

DirectXManager::DirectXManager(const DirectXManager& other)
{
}

DirectXManager::~DirectXManager()
{
}

bool DirectXManager::Initialise(ScreenResolution WindowResolution, HWND hwnd)
{
	HRESULT Result;
	IDXGIFactory* factory;
	IDXGIAdapter* adapter;
	IDXGIOutput* adapterOutput;
	unsigned int numModes, i, numerator, denominator, stringLength;
	DXGI_MODE_DESC* displayModeList;
	DXGI_ADAPTER_DESC adapterDesc;
	int error;
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	D3D_FEATURE_LEVEL featureLevel;
	ID3D11Texture2D* backBufferPtr;
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	D3D11_RASTERIZER_DESC rasterDesc;
	float fieldOfView, screenAspect;
	D3D11_DEPTH_STENCIL_DESC depthDisabledStencilDesc;
	D3D11_BLEND_DESC blendStateDescription;

	// Store the VSYNC_ENABLED setting.
	VSyncEnabled_ = VSYNC_ENABLED;

	// Create a DirectXManager graphics interface factory.
	Result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	if(FAILED(Result)) { return false; }

	// Use the factory to create an adapter for the primary graphics interface (video card).
	Result = factory -> EnumAdapters(0, &adapter);
	if(FAILED(Result)) { return false; }

	// Enumerate the primary adapter output (monitor).
	Result = adapter -> EnumOutputs(0, &adapterOutput);
	if(FAILED(Result)) { return false; }

	// Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format for the adapter output (monitor).
	Result = adapterOutput -> GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
	if(FAILED(Result)) { return false; }

	// Create a list to hold all the possible display modes for this monitor/video card combination.
	displayModeList = new DXGI_MODE_DESC[numModes];
	if(!displayModeList) { return false; }

	// Now fill the display mode list structures.
	Result = adapterOutput -> GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	if(FAILED(Result)) { return false; }

	// Now go through all the display modes and find the one that matches the screen width and height.
	// When a match is found store the numerator and denominator of the refresh rate for that monitor.
	for(i=0; i<numModes; i++)
	{
		if (displayModeList[i].Width == (unsigned int)WindowResolution.width)
		{
			if (displayModeList[i].Height == (unsigned int)WindowResolution.height)
			{
				numerator = displayModeList[i].RefreshRate.Numerator;
				denominator = displayModeList[i].RefreshRate.Denominator;
			}
		}
	}

	// Get the adapter (video card) description.
	Result = adapter -> GetDesc(&adapterDesc);
	if(FAILED(Result)) { return false; }

	// Store the dedicated video card memory in megabytes.
	VideoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	// Convert the name of the video card to a character array and store it.
	error = wcstombs_s(&stringLength, VideoCardDescription, 128, adapterDesc.Description, 128);
	if(error != 0) { return false; }

	// Release the display mode list.
	delete [] displayModeList;
	displayModeList = 0;

	// Release the adapter output.
	adapterOutput -> Release();
	adapterOutput = 0;

	// Release the adapter.
	adapter -> Release();
	adapter = 0;

	// Release the factory.
	factory -> Release();
	factory = 0;

	// Initialise the swap chain description.
    ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	// Set to a single back buffer.
    swapChainDesc.BufferCount = 1;

	// Set the width and height of the back buffer.
	swapChainDesc.BufferDesc.Width = WindowResolution.width;
	swapChainDesc.BufferDesc.Height = WindowResolution.height;

	// Set regular 32-bit surface for the back buffer.
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	// Set the refresh rate of the back buffer.
	if(VSyncEnabled_)
	{
	    swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
	    swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	// Set the usage of the back buffer.
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	// Set the handle for the window to render to.
    swapChainDesc.OutputWindow = hwnd;

	// Turn multisampling off.
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;

	// Set to full screen or windowed mode.
	if(FULL_SCREEN)
	{
		swapChainDesc.Windowed = false;
	}
	else
	{
		swapChainDesc.Windowed = true;
	}

	// Set the scan line ordering and scaling to unspecified.
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// Discard the back buffer contents after presenting.
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// Don't set the advanced flags.
	swapChainDesc.Flags = 0;

	// Set the feature level to DirectXManager 11
	featureLevel = D3D_FEATURE_LEVEL_11_0;

	// Create the swap chain, Direct3D device, and Direct3D device context
	Result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featureLevel, 1, D3D11_SDK_VERSION, &swapChainDesc, &SwapChain_, &Device_, NULL, &DeviceContext_);
	if(FAILED(Result)) { return false; }

	// Get the pointer to the back buffer
	Result = SwapChain_ -> GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
	if(FAILED(Result)) { return false; }

	// Create the render target view with the back buffer pointer
	Result = Device_ -> CreateRenderTargetView(backBufferPtr, NULL, &RenderTargetView_);
	if(FAILED(Result)) { return false; }

	// Release pointer to the back buffer as we no longer need it
	backBufferPtr -> Release();
	backBufferPtr = 0;

	// Initialise the description of the depth buffer.
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	// Set up the description of the depth buffer.
	depthBufferDesc.Width = WindowResolution.width;
	depthBufferDesc.Height = WindowResolution.height;
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
	Result = Device_ -> CreateTexture2D(&depthBufferDesc, NULL, &DepthStencilBuffer_);
	if(FAILED(Result)) { return false; }

	// Initialise the description of the stencil state.
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// Set up the description of the stencil state.
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create the depth stencil state.
	Result = Device_ -> CreateDepthStencilState(&depthStencilDesc, &DepthStencilState_);
	if(FAILED(Result)) { return false; }

	// Set the depth stencil state.
	DeviceContext_ -> OMSetDepthStencilState(DepthStencilState_, 1);

	// Initialise the depth stencil view.
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// Set up the depth stencil view description.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// Create the depth stencil view.
	Result = Device_ -> CreateDepthStencilView(DepthStencilBuffer_, &depthStencilViewDesc, &DepthStencilView);
	if(FAILED(Result)) { return false; }

	// Bind the render target view and depth stencil buffer to the output render pipeline.
	DeviceContext_ -> OMSetRenderTargets(1, &RenderTargetView_, DepthStencilView);

	// Setup the raster description which will determine how and what polygons will be drawn.
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	// Create the rasterizer state from the description we just filled out.
	Result = Device_ -> CreateRasterizerState(&rasterDesc, &RasterState_);
	if(FAILED(Result)) { return false; }

	// Now set the rasterizer state.
	DeviceContext_ -> RSSetState(RasterState_);
	
	// Setup a raster description which turns off back face culling.
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_NONE;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	// Create the no culling rasterizer state.
	Result = Device_ -> CreateRasterizerState(&rasterDesc, &RasterStateNoCulling_);
	if(FAILED(Result)) { return false; }

	// Setup the viewport for rendering.
	Viewport_.Width = (float)WindowResolution.width;
	Viewport_.Height = (float)WindowResolution.height;
    Viewport_.MinDepth = 0.0f;
    Viewport_.MaxDepth = 1.0f;
    Viewport_.TopLeftX = 0.0f;
    Viewport_.TopLeftY = 0.0f;

	// Create the viewport.
    DeviceContext_ -> RSSetViewports(1, &Viewport_);

	// Setup the projection matrix.
	fieldOfView = (float)D3DX_PI / 4.0f;
	screenAspect = (float)WindowResolution.width / (float)WindowResolution.height;

	// Create the projection matrix for 3D rendering.
	D3DXMatrixPerspectiveFovLH(&ProjectionMatrix_, fieldOfView, screenAspect, SCREEN_NEAR, SCREEN_DEPTH);

    // Initialise the world matrix to the identity matrix.
    D3DXMatrixIdentity(&WorldMatrix_);

	// Create an orthographic projection matrix for 2D rendering.
	D3DXMatrixOrthoLH(&OrthoMatrix_, (float)WindowResolution.width, (float)WindowResolution.height, SCREEN_NEAR, SCREEN_DEPTH);

	// Clear the second depth stencil state before setting the parameters.
	ZeroMemory(&depthDisabledStencilDesc, sizeof(depthDisabledStencilDesc));

	// Now create a second depth stencil state which turns off the Z buffer for 2D rendering.  The only difference is 
	// that DepthEnable is set to false, all other parameters are the same as the other depth stencil state.
	depthDisabledStencilDesc.DepthEnable = false;
	depthDisabledStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthDisabledStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthDisabledStencilDesc.StencilEnable = true;
	depthDisabledStencilDesc.StencilReadMask = 0xFF;
	depthDisabledStencilDesc.StencilWriteMask = 0xFF;
	depthDisabledStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthDisabledStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthDisabledStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthDisabledStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create the state using the device.
	Result = Device_ -> CreateDepthStencilState(&depthDisabledStencilDesc, &DepthDisabledStencilState_);
	if(FAILED(Result)) { return false; }

	// Clear the blend state description.
	ZeroMemory(&blendStateDescription, sizeof(D3D11_BLEND_DESC));

	// Create an alpha enabled blend state description.
	blendStateDescription.RenderTarget[0].BlendEnable = TRUE;
	blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	// Create the blend state using the description.
	Result = Device_ -> CreateBlendState(&blendStateDescription, &AlphaEnableBlendingState_);
	if(FAILED(Result)) { return false; }

	// Modify the description to create an alpha disabled blend state description.
	blendStateDescription.RenderTarget[0].BlendEnable = FALSE;

	// Create the second blend state using the description.
	Result = Device_ -> CreateBlendState(&blendStateDescription, &NormalBlendingState_);
	if(FAILED(Result)) { return false; }

	// Create a secondary alpha blend state description.
	blendStateDescription.RenderTarget[0].BlendEnable = TRUE;
	blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	// Create the blend state using the description.
	Result = Device_ -> CreateBlendState(&blendStateDescription, &AlphaCloudBlendState_);
	if(FAILED(Result)) { return false; }

	// Create an fire alpha enabled blend state description.
	blendStateDescription.RenderTarget[0].BlendEnable = TRUE;
	blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	// Create the blend state using the description.
	Result = Device_ -> CreateBlendState(&blendStateDescription, &AlphaFireBlendState_);
	if (FAILED(Result))
	{
		return false;
	}

    return true;
}

void DirectXManager::Shutdown()
{
	// Release swap chain first otherwise it will crash
	if(SwapChain_)
	{
		SwapChain_ -> SetFullscreenState(false, NULL);
	}

	// Release everything else
	if(AlphaCloudBlendState_)
	{
		AlphaCloudBlendState_ -> Release();
		AlphaCloudBlendState_ = 0;
	}

	if(AlphaEnableBlendingState_)
	{
		AlphaEnableBlendingState_ -> Release();
		AlphaEnableBlendingState_ = 0;
	}

	if(NormalBlendingState_)
	{
		NormalBlendingState_ -> Release();
		NormalBlendingState_ = 0;
	}

	if(DepthDisabledStencilState_)
	{
		DepthDisabledStencilState_ -> Release();
		DepthDisabledStencilState_ = 0;
	}

	if(RasterStateNoCulling_)
	{
		RasterStateNoCulling_ -> Release();
		RasterStateNoCulling_ = 0;
	}

	if(RasterState_)
	{
		RasterState_ -> Release();
		RasterState_ = 0;
	}

	if(DepthStencilView)
	{
		DepthStencilView -> Release();
		DepthStencilView = 0;
	}

	if(DepthStencilState_)
	{
		DepthStencilState_ -> Release();
		DepthStencilState_ = 0;
	}

	if(DepthStencilBuffer_)
	{
		DepthStencilBuffer_ -> Release();
		DepthStencilBuffer_ = 0;
	}

	if(RenderTargetView_)
	{
		RenderTargetView_ -> Release();
		RenderTargetView_ = 0;
	}

	if(DeviceContext_)
	{
		DeviceContext_ -> Release();
		DeviceContext_ = 0;
	}

	if(Device_)
	{
		Device_ -> Release();
		Device_ = 0;
	}

	if(SwapChain_)
	{
		SwapChain_ -> Release();
		SwapChain_ = 0;
	}

	return;
}

void DirectXManager::BeginScene(float red, float green, float blue, float alpha)
{
	float color[4] = { red, green, blue, alpha };

	// Clear the back buffer.
	DeviceContext_ -> ClearRenderTargetView(RenderTargetView_, color);
    
	// Clear the depth buffer.
	DeviceContext_ -> ClearDepthStencilView(DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	return;
}

void DirectXManager::EndScene()
{
	// Present the back buffer to the screen since rendering is complete.
	if(VSyncEnabled_)
	{
		SwapChain_ -> Present(1, 0); // Lock to screen refresh rate.
	}
	else
	{
		SwapChain_ -> Present(0, 0); // Present as fast as possible.
	}

	return;
}

ID3D11Device* DirectXManager::GetDevice()
{
	return Device_;
}

ID3D11DeviceContext* DirectXManager::GetDeviceContext()
{
	return DeviceContext_;
}

void DirectXManager::GetProjectionMatrix(D3DXMATRIX& projectionMatrix)
{
	projectionMatrix = ProjectionMatrix_;
	return;
}

void DirectXManager::GetWorldMatrix(D3DXMATRIX& worldMatrix)
{
	worldMatrix = WorldMatrix_;
	return;
}

void DirectXManager::GetOrthoMatrix(D3DXMATRIX& orthoMatrix)
{
	orthoMatrix = OrthoMatrix_;
	return;
}

void DirectXManager::ToggleZBuffer(bool isON)
{
	// Toggle between Z buffer on or off
	if(isON)
	{
		DeviceContext_ -> OMSetDepthStencilState(DepthStencilState_, 1);
	}
	else
	{
		DeviceContext_ -> OMSetDepthStencilState(DepthDisabledStencilState_, 1);
	}
}

void DirectXManager::ToggleAlphaBlending(bool isON)
{
	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	
	// Toggle between alpha blending on or off
	if(isON)
	{
		DeviceContext_ -> OMSetBlendState(AlphaEnableBlendingState_, blendFactor, 0xffffffff);
	}
	else
	{
		DeviceContext_ -> OMSetBlendState(NormalBlendingState_, blendFactor, 0xffffffff);
	}
}

void DirectXManager::ToggleCulling(bool isON)
{
	if(isON)
	{
		DeviceContext_ -> RSSetState(RasterState_);
	}
	else
	{
		DeviceContext_ -> RSSetState(RasterStateNoCulling_);
	}

	// If wireframe mode is on
	if(Wireframe_)
	{
		// Reactive it as it gets deactivated when these are switched
		ToggleWireframe(true);
	}
}

void DirectXManager::ToggleWireframe(bool isON)
{
	// Variables to hold the current rasterizer state and its description
	ID3D11RasterizerState * currentState ;
	D3D11_RASTERIZER_DESC currentDesc ;

	 // Retrieve the current state
	DeviceContext_ -> RSGetState( &currentState );

	// Gets the description for rasterizer state 
	currentState -> GetDesc( &currentDesc ); 

	// Toggle the wireframe
	if(isON)
	{
		// Turn on wireframe mode
		currentDesc.FillMode = D3D11_FILL_WIREFRAME;
	}
	else
	{
		// Turn off wireframe mode
		currentDesc.FillMode = D3D11_FILL_SOLID;
	}
	
	// Update the state
	Device_ -> CreateRasterizerState( &currentDesc, &currentState );
	DeviceContext_ -> RSSetState( currentState );

	// Store the wireframe mode state
	Wireframe_ = isON;
}

bool DirectXManager::GetWireframeStatus()
{
	// Return whether wireframe mode is on or not
	return Wireframe_;
}

void DirectXManager::SetBackBufferRenderTarget()
{
	// Bind the render target view and depth stencil buffer to the output render pipeline
	DeviceContext_ -> OMSetRenderTargets(1, &RenderTargetView_, DepthStencilView);

	return;
}

void DirectXManager::ResetViewport()
{
	// Set the viewport
    DeviceContext_ -> RSSetViewports(1, &Viewport_);

	return;
}

void DirectXManager::EnableSecondBlendState()
{
	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	
	// Turn on the alpha blending
	DeviceContext_ -> OMSetBlendState(AlphaCloudBlendState_, blendFactor, 0xffffffff);

	return;
}

void DirectXManager::EnableFireBlendState()
{
	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	// Turn on the alpha blending
	DeviceContext_ -> OMSetBlendState(AlphaFireBlendState_, blendFactor, 0xffffffff);
}