#include "WindowManager.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	bool Result;
	WindowManager* windowManager;


	//===================
	// Create the window
	//===================

	windowManager = new WindowManager;
	if (!windowManager)
		return 0;

	// Initialise it
	Result = windowManager->Initialise();
	if (Result)
	{
		// Run the window
		windowManager->Run();
	}

	//==========
	// Shutdown
	//==========

	windowManager->Shutdown();
	delete windowManager;
	windowManager = 0;

	return 0;
}