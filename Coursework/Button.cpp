#include "Button.h"
#include "InputManager.h"
#include "WindowManager.h"

Button::Button()
{
}


Button::~Button()
{
}

bool Button::initialise(Rect3D spriteResolution, string textureFilename)
{
	// Call parent initialise
	Sprite::initialise(spriteResolution, textureFilename);

	m_normalColour = D3DXVECTOR4(0, 0, 0, 0);
	m_highlightColour = D3DXVECTOR4(1, 1, 1, 0.5f);

	m_buttonText = new Text;
	if (!m_buttonText)
	{
		return false;
	}
	m_result = m_buttonText->initialise(WindowManager::getInstance()->getHWND(), "minecraftia.txt", "minecraftia.dds", 89);
	if (!m_result)
	{
		return false;
	}

	m_buttonText->createText("TEXT NOT SET", Vector2(0, 0), Colour::White, Alignment::CENTRE);

	return true;
}

bool Button::update()
{
	// Check if we are mousing over
	if (checkCollision(m_boundingBox, InputManager::getInstance()->getMousePosInWindow()))
	{
		m_model->getMaterial()->setVector4("BaseColour", m_highlightColour);

		if (InputManager::getInstance()->getKeyDown(VK_LBUTTON))
		{
			return false;
		}
	}
	else
	{
		m_model->getMaterial()->setVector4("BaseColour", m_normalColour);
	}

	return true;
}

bool Button::render()
{
	m_result = Sprite::render();
	if (!m_result)
	{
		return false;
	}

	m_result = m_buttonText->render();
	if (!m_result)
	{
		return false;
	}

	return true;
}

void Button::setNormalColour(D3DXVECTOR4 normalColour)
{
	m_normalColour = normalColour;
}

void Button::setHighlightColour(D3DXVECTOR4 highlightColour)
{
	m_highlightColour = highlightColour;
}

void Button::setButton(string buttonText, int x, int y)
{
	//====================
	// update Positioning
	//====================

	// Calculate the X and Y pixel position on the screen to start drawing to.
	float drawX = (float)(((WindowManager::getInstance()->getWindowResolution().width / 2) * -1) + x);
	float drawY = (float)((WindowManager::getInstance()->getWindowResolution().height / 2) - y);

	// set sprite position
	m_transform->setPosition(drawX, drawY, 0);

	// update bounding box
	m_boundingBox = m_model->getMesh()->getBoundingBox();
	m_boundingBox.setPosition(D3DXVECTOR3(drawX, drawY, 0));

	//=============
	// update Text
	//=============

	m_buttonText->setText(0, buttonText);
	m_buttonText->setPosition(0, Vector2(x, y - 20));
}