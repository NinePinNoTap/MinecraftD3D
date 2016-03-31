#include "WindowManager.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	bool Result_;
	WindowManager* windowManager;

	//===================
	// Create the window
	//===================

	windowManager = new WindowManager;
	if (!windowManager)
		return 0;

	// Initialise it
	Result_ = windowManager->Initialise();
	if (Result_)
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