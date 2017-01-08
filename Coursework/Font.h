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
		int width, height;
	};

public:
	Font();
	Font(const Font&);
	~Font();

	// Initialising
	bool initialise(const string fontFilename, int letterCount);

	// terminate
	void terminate();
	
	// Building
	void buildVertexArray(VertexData* vertices, string sentence, float drawX, float drawY);

	// getter
	float getRenderLength(string text);

private:
	// Font Data
	FontType* m_font;
	
	bool m_result;
};


