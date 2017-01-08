#pragma once

#include <vector>


#include "GameObject.h"
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

class Text : public GameObject
{
public:
	struct SentenceType
	{
		int ID;
		string text;
		float value;
		bool useValue;
		Vector2 position;
		Alignment align;
	};

public:
	Text();
	Text(const Text&);
	~Text();

	// Initialising
	bool initialise(HWND hwnd, string fontName, string fontTexture, int letterCount);

	// terminate
	void terminate();

	// Text Creation
	void createText(string text, Vector2 position, D3DXVECTOR4 colour, Alignment align = LEFT);

	// rendering
	bool render();

	// Updating
	bool setText(int ID, string text);
	bool setValue(int ID, float value);
	bool setPosition(int ID, Vector2 position);
	void setColour(int ID, D3DXVECTOR4 colour);
	void disableValue(int ID);

private:
	bool buildSentence(SentenceType sentence);
	bool prepareSentence(int index);

	// Window Size
	Rect2D m_windowResolution;

	// Font Data
	Font* m_font;

	// Model
	Model* m_model;
	Material* m_material;
	GameShader* m_shader;

	// Sentences
	vector<SentenceType> m_sentences;

	// Flags
	bool m_result;
};