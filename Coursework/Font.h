#pragma once

#include <d3d11.h>
#include <d3dx10math.h>
#include <fstream>

#include "Texture.h"

using namespace std;

class Font
{
private:
	struct FontType
	{
		float left, right;
		int size;
	};

	struct VertexType
	{
		D3DXVECTOR3 position;
	    D3DXVECTOR2 texture;
	};

public:
	Font();
	Font(const Font&);
	~Font();

	// Initialising
	bool Initialise(const char* fontFilename, string textureFilename);

	// Shutdown
	void Shutdown();

	// Building
	void BuildVertexArray(void* vertices, char* sentence, float drawX, float drawY);

	// Getter
	float GetRenderSize(char* text);
	ID3D11ShaderResourceView* GetTexture();

private:
	// Loading
	bool LoadFontData(const char* filename);
	bool LoadTexture(string filename);

	// Font Data
	FontType* Font_;

	// Texture
	Texture* Texture_;

	bool Result_;
};


