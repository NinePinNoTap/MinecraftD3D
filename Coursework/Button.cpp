#include "Button.h"
#include "InputManager.h"
#include "WindowManager.h"

Button::Button()
{
}


Button::~Button()
{
}

bool Button::Initialise(Rect3D spriteResolution, string textureFilename)
{
	// Call parent initialise
	Sprite::Initialise(spriteResolution, textureFilename);

	NormalColour_ = D3DXVECTOR4(0, 0, 0, 0);
	HighlightColour_ = D3DXVECTOR4(1, 1, 1, 0.5f);

	ButtonText_ = new Text;
	if (!ButtonText_)
	{
		return false;
	}
	Result_ = ButtonText_->Initialise(WindowManager::Instance()->GetHWND(), "minecraftia.txt", "minecraftia.dds", 89);
	if (!Result_)
	{
		return false;
	}

	ButtonText_->CreateText("TEXT NOT SET", Vector2(0, 0), NormalColour_, Alignment::CENTRE);

	return true;
}

bool Button::Frame()
{
	D3DXVECTOR3 hey = InputManager::Instance()->GetMousePos();
	
	// Check if we are mousing over
	if (CheckCollision(Box_, InputManager::Instance()->GetMousePos()))
	{
		Model_->GetMaterial()->SetVector4("BaseColour", HighlightColour_);

		if (InputManager::Instance()->GetKeyDown(VK_LBUTTON))
		{
			return false;
		}
	}
	else
	{
		Model_->GetMaterial()->SetVector4("BaseColour", NormalColour_);
	}

	return true;
}

void Button::SetNormalColour(D3DXVECTOR4 normalColour)
{
	NormalColour_ = normalColour;
}

void Button::SetHighlightColour(D3DXVECTOR4 highlightColour)
{
	HighlightColour_ = highlightColour;
}

void Button::SetButton(string buttonText, int x, int y)
{
	//====================
	// Update Positioning
	//====================

	// Set sprite position
	Transform_->SetPosition(x, y, 0);

	// Update bounding box
	Box_ = Model_->GetMesh()->GetBoundingBox();
	Box_.SetPosition(D3DXVECTOR3(x, y, 0));

	//=============
	// Update Text
	//=============

	ButtonText_->SetText(0, buttonText);
	ButtonText_->SetPosition(0, Vector2(x, y - 20));
}