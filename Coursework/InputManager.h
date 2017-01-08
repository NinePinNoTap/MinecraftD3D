#pragma once

#include <Windows.h>

#include "Utilities.h"
#include "Singleton.h"

class InputManager : public Singleton<InputManager>
{
public:
	InputManager();
	InputManager(const InputManager&);
	~InputManager();

	// Initialising
	void initialise();

	void update();

	// getters
	bool getKeyDown(unsigned int key);
	bool getKey(unsigned int key);

	inline D3DXVECTOR2 getMousePosInWindow() { return m_mousePositionInWindow; }
	inline D3DXVECTOR2 getMousePosOnScreen() { return m_mousePositionOnScreen; }

private:
	bool m_keys[256];
	D3DXVECTOR2 m_mousePositionOnScreen;
	D3DXVECTOR2 m_mousePositionInWindow;
};


