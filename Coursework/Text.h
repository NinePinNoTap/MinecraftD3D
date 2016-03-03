#pragma once

#include <vector>

#include "Constants.h"
#include "Font.h"
#include "Utilities.h"


using namespace std;

enum Alignment { LEFT, CENTRE, RIGHT };

class Text
{
public:
	struct SentenceType
	{
		ID3D11Buffer* vertexBuffer;
		ID3D11Buffer* indexBuffer;
		int vertexCount;
		int indexCount;
		int maxLength;
		char* text;
		Vector2 position;
		Colour colour;
		Alignment align;
	};

	struct VertexType
	{
		D3DXVECTOR3 position;
	    D3DXVECTOR2 texture;
	};

public:
	Text();
	Text(const Text&);
	~Text();

	// Initialising
	bool Initialise(HWND hwnd, Rect2D WindowResolution);

	// Shutdown
	void Shutdown();

	// Text Creation
	void CreateText(char* text, Vector2 position, Colour colour, Alignment align = LEFT);

	// Rendering
	bool Render();

	// Updating
	bool SetText(int ID, char* text);
	bool SetText(int ID, char* text, float value);
	bool SetPosition(int ID, Vector2 position);
	bool SetColour(int ID, Colour colour);

private:
	bool InitialiseSentence(SentenceType** sentence, int maxLength);
	bool UpdateSentence(SentenceType* sentence, char* text, Vector2 position, Colour colour, Alignment align);
	void ReleaseSentence(SentenceType** sentence);
	bool RenderSentence(SentenceType* sentence);

	// Window Size
	Rect2D WindowResolution_;

	// Font Data
	Font* Font_;

	// Sentences
	vector<SentenceType*> Sentences_;

	// Flags
	bool Result_;
};