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

	// Getters
	bool GetKeyDown(unsigned int key);
	bool GetKey(unsigned int key);

private:
	bool Keys_[256];
};


