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

	bool Initialise(Rect3D spriteResolution, string textureFilename);

	// Frame
	bool Frame();
	bool Render();

	// Setters
	void SetNormalColour(D3DXVECTOR4 normalColour);
	void SetHighlightColour(D3DXVECTOR4 highlightColour);
	void SetButton(string buttonText, int x, int y);

	inline void Execute() { ButtonFunction_(); }

	inline void SetFunction(function<void()> func)
	{
		ButtonFunction_ = func;
	}

private:
	BoundingBox Box_;

	D3DXVECTOR4 NormalColour_;
	D3DXVECTOR4 HighlightColour_;

	Text* ButtonText_;

	function<void()> ButtonFunction_;
};

