#pragma once

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")

#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include <d3dx10math.h>

#include "Config.h"
#include "Rect.h"
#include "Singleton.h"

using namespace Config;

class DirectXManager : public Singleton<DirectXManager>
{
public:
	DirectXManager();
	DirectXManager(const DirectXManager&);
	~DirectXManager();

	// Initialising
	bool initialise(Rect2D WindowDimension, HWND hwnd);

	// terminate
	void terminate();
	
	// rendering
	void beginScene(D3DXVECTOR4 clearColour = Colour::Black);
	void endScene();

	// rendering Toggles
	void setDepthBufferOn(bool flag);
	void setAlphaBlendingOn(bool flag);
	void setBackfaceCullingOn(bool flag);
	void toggleWireframe(bool flag);

	// Blend States
	void setCloudBlendingOn();
	void setAlphaMaskingOn();

	// Misc
	void setBackBufferRenderTarget();
	void ResetViewport();

	// DirectXManager getters
	ID3D11Device* getDevice();
	ID3D11DeviceContext* getDeviceContext();

	// Matrix getters
	void getProjectionMatrix(D3DXMATRIX& ProjectionMatrix);
	void getWorldMatrix(D3DXMATRIX& WorldMatrix);
	void getOrthoMatrix(D3DXMATRIX& OrthoMatrix);

	// Wireframe
	bool getWireframeStatus();

private:
	// Video Card Information
	int m_videoCardMemory;
	char m_videoCardDescription[128];

	// Viewport
	D3D11_VIEWPORT m_viewport;

	// Matrices
	D3DXMATRIX m_orthoMatrix;
	D3DXMATRIX m_projectionMatrix;
	D3DXMATRIX m_worldMatrix;
	
	// Blend States
	ID3D11BlendState* m_alphaCloudBlendState;
	ID3D11BlendState* m_alphaEnableBlendState;
	ID3D11BlendState* m_alphaFireBlendState;
	ID3D11BlendState* m_normalBlendState;

	// Stencil States
	ID3D11DepthStencilState* m_depthDisabledStencilState;
	ID3D11DepthStencilState* m_depthStencilState;
	ID3D11DepthStencilView* m_depthStencilView;

	// Adapters
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;

	// Rasterizer States
	ID3D11RasterizerState* m_rasterStateNoCulling;
	ID3D11RasterizerState* m_rasterState;

	// rendering
	ID3D11RenderTargetView* m_renderTargetView;
	ID3D11Texture2D* m_depthStencilBuffer;
	IDXGISwapChain* m_swapChain;

	// Flags
	bool m_wireframe;
	bool m_vSyncEnabled;
};


