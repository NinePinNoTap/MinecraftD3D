#include "WindowManager.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	bool m_result;
	WindowManager* windowManager;

	//===================
	// create the window
	//===================

	windowManager = new WindowManager;
	if (!windowManager)
		return 0;

	// initialise it
	m_result = windowManager->initialise();
	if (m_result)
	{
		// run the window
		windowManager->run();
	}

	//==========
	// terminate
	//==========

	windowManager->terminate();
	delete windowManager;
	windowManager = 0;

	return 0;
}