#pragma once

#include <vector>

#include "Constants.h"
#include "Font.h"
#include "Model.h"
#include "Mesh3D.h"
#include "Material.h"
#include "GameShader.h"
#include "Utilities.h"

using namespace std;

enum Alignment
{
	LEFT, CENTRE, RIGHT
};

class Text
{
public:
	struct SentenceType
	{
		int ID;
		string text;
		float value;
		bool valueSet;
		Vector2 position;
		Alignment align;
	};

public:
	Text();
	Text(const Text&);
	~Text();

	// Initialising
	bool Initialise(HWND hwnd, string fontName, string fontTexture, int letterCount);

	// Shutdown
	void Shutdown();

	// Text Creation
	void CreateText(string text, Vector2 position, D3DXVECTOR4 colour, Alignment align = LEFT);

	// Rendering
	bool Render();

	// Updating
	bool SetText(int ID, string text);
	bool SetValue(int ID, float value);
	bool SetPosition(int ID, Vector2 position);
	void SetColour(int ID, D3DXVECTOR4 colour);

private:
	bool BuildSentence(SentenceType sentence);
	bool PrepareSentence(int index);

	// Window Size
	Rect2D WindowResolution_;

	// Font Data
	Font* Font_;

	// Model
	Model* Model_;
	Material* FontMaterial_;
	GameShader* Shader_;

	// Sentences
	vector<SentenceType> Sentences_;

	// Flags
	bool Result_;
};