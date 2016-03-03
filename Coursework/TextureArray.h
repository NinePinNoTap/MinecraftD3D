#pragma once

#include <d3d11.h>
#include <d3dx11tex.h>
#include <vector>

using namespace std;

class TextureArray
{
public:
	TextureArray();
	TextureArray(const TextureArray&);
	~TextureArray();
	
	bool Initialise(vector<wstring>);
	void Shutdown();
	
	ID3D11ShaderResourceView* GetTextures();

private:
	ID3D11ShaderResourceView* Textures_;
};


