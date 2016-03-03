#pragma once

#include <d3d11.h>
#include <d3dx10math.h>

#include "DataStructs.h"

class Texture
{
public:
	Texture();
	Texture(const Texture&);
	~Texture();

	// Initialising and shutdown
	bool Initialise(WCHAR*);
	bool Initialise(ScreenResolution textureResolution);
	void Shutdown();

	// Render to Texture
	void SetRenderTarget();
	void ClearRenderTarget(Colour ClearColour);

	// Getters
	ID3D11ShaderResourceView* GetTexture();
	void GetProjectionMatrix(D3DXMATRIX& projectionMatrix);
	void GetOrthoMatrix(D3DXMATRIX& orthoMatrix);

private:
	// Texture
	ID3D11ShaderResourceView* Texture_;

	// Render to Texture
	ID3D11Texture2D* RenderTargetTexture_;
	ID3D11RenderTargetView* RenderTargetView_;

	// Viewport
	ID3D11Texture2D* DepthStencilBuffer_;
	ID3D11DepthStencilView* DepthStencilView_;
	D3D11_VIEWPORT Viewport_;
	D3DXMATRIX ProjectionMatrix_;
	D3DXMATRIX OrthoMatrix_;
};


