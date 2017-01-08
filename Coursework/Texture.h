#pragma once

#include <d3d11.h>
#include <d3dx10math.h>
#include <string>

#include "Config.h"
#include "Rect.h"

using namespace Config;
using namespace std;

class Texture
{
public:
	Texture();
	Texture(const Texture&);
	~Texture();

	// Initialising and shutdown
	bool initialise(string filename);
	bool initialise(Rect2D textureResolution);
	void terminate();

	// render to Texture
	void setRenderTarget();
	void clearRenderTarget(D3DXVECTOR4 clearColour);

	// getters
	ID3D11ShaderResourceView* getTexture();
	void getProjectionMatrix(D3DXMATRIX& projectionMatrix);
	void getOrthoMatrix(D3DXMATRIX& orthoMatrix);

private:
	// Texture
	ID3D11ShaderResourceView* m_texture;

	// render to Texture
	ID3D11Texture2D* m_renderTargetTexture;
	ID3D11RenderTargetView* m_renderTargetView;

	// Viewport
	ID3D11Texture2D* m_depthStencilBuffer;
	ID3D11DepthStencilView* m_depthStencilView;
	D3D11_VIEWPORT m_viewport;
	D3DXMATRIX m_projectionMatrix;
	D3DXMATRIX m_orthoMatrix;
};


