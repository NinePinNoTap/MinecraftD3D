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
	void Initialise();

	void Frame();

	// Getters
	bool GetKeyDown(unsigned int key);
	bool GetKey(unsigned int key);

	inline D3DXVECTOR2 GetMousePos() { return MousePos_; }

private:
	bool Keys_[256];
	D3DXVECTOR2 MousePos_;
};


