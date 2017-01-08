#pragma once

#include <functional>

#include "Sprite.h"
#include "Text.h"

using namespace std;

class Button : public Sprite
{
public:
	Button();
	~Button();

	bool initialise(Rect3D spriteResolution, string textureFilename);

	// update
	bool update();
	bool render();

	// setters
	void setNormalColour(D3DXVECTOR4 normalColour);
	void setHighlightColour(D3DXVECTOR4 highlightColour);
	void setButton(string buttonText, int x, int y);

	inline void execute() { m_buttonFuntion(); }

	inline void setFunction(function<void()> func)
	{
		m_buttonFuntion = func;
	}

private:
	BoundingBox m_boundingBox;

	D3DXVECTOR4 m_normalColour;
	D3DXVECTOR4 m_highlightColour;

	Text* m_buttonText;

	function<void()> m_buttonFuntion;
};

