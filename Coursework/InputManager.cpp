#include "InputManager.h"
#include "WindowManager.h"

InputManager::InputManager()
{
}

InputManager::InputManager(const InputManager& other)
{
}

InputManager::~InputManager()
{
}

void InputManager::initialise()
{
	// initialise the keys to 'not currently pressed'
	for (int i = 0; i<256; i++)
	{
		m_keys[i] = false;
	}

	return;
}

void InputManager::update()
{
	POINT mousePos;

	// get Mouse Position
	GetCursorPos(&mousePos);

	// Store the position of the mouse
	m_mousePositionOnScreen.x = mousePos.x;
	m_mousePositionOnScreen.y = mousePos.y;

	// Convert to window space
	ScreenToClient(GetActiveWindow(), &mousePos);

	// Convert to view space
	m_mousePositionInWindow.x = ((WindowManager::getInstance()->getWindowResolution().width / 2) * -1) + mousePos.x;
	m_mousePositionInWindow.y = (WindowManager::getInstance()->getWindowResolution().height / 2) - mousePos.y;
}

bool InputManager::getKey(unsigned int key)
{
	// Make sure we are currently on the ApplicationManager to register key presses
	if (!windowActive())
		return false;

	// Check the status of the key and store it
	if (GetAsyncKeyState(key))
	{
		m_keys[key] = true;
	}
	else
	{
		m_keys[key] = false;
	}

	return m_keys[key];
}

bool InputManager::getKeyDown(unsigned int key)
{
	// Make sure we are currently on the ApplicationManager to register key presses
	if (!windowActive())
		return false;

	// If the key we want is down
	if (GetAsyncKeyState(key))
	{
		// If the key is already flagged as down
		if (m_keys[key])
		{
			return false;
		}
		else if (!m_keys[key])
		{
			// Key not down so flag that it is
			m_keys[key] = true;
			return true;
		}
	}
	else if (!GetAsyncKeyState(key))
	{
		m_keys[key] = false;
		return false;
	}

	return false;
}