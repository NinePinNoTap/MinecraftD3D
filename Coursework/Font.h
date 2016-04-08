#pragma once

#include <d3d11.h>
#include <d3dx10math.h>
#include <fstream>

#include "Texture.h"
#include "VertexData.h"

using namespace std;

class Font
{
private:
	struct FontType
	{
		float left, right;
		int size;
	};

public:
	Font();
	Font(const Font&);
	~Font();

	// Initialising
	bool Initialise(const string fontFilename, int letterCount);

	// Shutdown
	void Shutdown();
	
	// Building
	void BuildVertexArray(VertexData* vertices, string sentence, float drawX, float drawY);

	// Getter
	float GetRenderSize(string text);

private:
	// Font Data
	FontType* Font_;
	
	bool Result_;
};


