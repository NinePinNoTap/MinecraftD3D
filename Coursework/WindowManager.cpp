#include "WindowManager.h"

WindowManager::WindowManager()
{
}

WindowManager::WindowManager(const WindowManager& other)
{
}

WindowManager::~WindowManager()
{
}

bool WindowManager::initialise()
{
	// initialise the width and height of the screen to zero before sending the variables into the function
	createWindow(m_windowResolution.width, m_windowResolution.height);

	// initialise application
	m_applicationManager = new ApplicationManager;
	if (!m_applicationManager)
		return false;

	// initialise the ApplicationManager
	m_result = m_applicationManager->initialise(m_hwnd, m_windowResolution);
	if(!m_result)
	{
		return false;
	}

	return true;
}

void WindowManager::createWindow(int& screenWidth, int& screenHeight)
{
	WNDCLASSEX wc;
	DEVMODE dmScreensettings;
	D3DXVECTOR2 screenPosition;

	// get the instance of this ApplicationManager.
	m_hinstance = GetModuleHandle(NULL);

	// Give the ApplicationManager a name.
	m_applicationName = L"Programming Games in DirectX";

	// setup the windows class with default settings.
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hinstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	// Register the window class.
	RegisterClassEx(&wc);

	// Determine the resolution of the clients desktop screen.
	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// setup the screen settings depending on whether it is running in full screen or in windowed mode.
	if (System::FullScreen)
	{
		// If full screen set the screen to maximum size of the users desktop and 32bit.
		memset(&dmScreensettings, 0, sizeof(dmScreensettings));
		dmScreensettings.dmSize = sizeof(dmScreensettings);
		dmScreensettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreensettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreensettings.dmBitsPerPel = 32;
		dmScreensettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen.
		ChangeDisplaySettings(&dmScreensettings, CDS_FULLSCREEN);

		// set the position of the window to the top left corner.
		screenPosition.x = screenPosition.y = 0;
	}
	else
	{
		// set to default window resolution
		screenWidth = System::ScreenWidth;
		screenHeight = System::ScreenHeight;

		// Place the window in the middle of the active window space (top of screen to top of taskbar)
		screenPosition.x = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		screenPosition.y = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;

		// get size of the task bar
		RECT rect;
		HWND taskBar = FindWindow(L"Shell_traywnd", NULL);
		if (taskBar && GetWindowRect(taskBar, &rect))
		{
			// move the window up
			float taskbarHeight = (rect.bottom - rect.top);
			screenPosition.y -= (taskbarHeight / 2);
		}
	}

	// create the window with the screen settings and get the handle to it.
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		screenPosition.x, screenPosition.y, screenWidth, screenHeight,
		NULL, NULL, m_hinstance, NULL);

	// Bring the window up on the screen and set it as main focus.
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	return;
}

void WindowManager::terminate()
{
	// terminate scenes
	ApplicationManager::getInstance()->terminate();

	//=====================
	// terminate the Window
	//=====================

	// Show the mouse cursor
	ShowCursor(true);

	// Fix the display settings if leaving full screen mode
	if (System::FullScreen)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// Remove the window
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	// Remove the ApplicationManager instance
	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = NULL;

	return;
	
	return;
}

void WindowManager::run()
{
	MSG msg;
	bool Running = true;

	// initialise the message structure.
	ZeroMemory(&msg, sizeof(MSG));
	
	// Keep running until an escape is processed
	while(Running)
	{
		// handle the windows messages.
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// If windows signals to end the application then exit out.
		if(msg.message == WM_QUIT)
		{
			Running = false;
		}
		else
		{
			// Do the frame processing for the application object.
			m_result = m_applicationManager->update();
			if (!m_result)
			{
				Running = false;
			}
		}
	}

	return;
}

LRESULT CALLBACK WindowManager::Messagehandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch(umsg)
	{
		// Any other messages send to the default message handler as our ApplicationManager won't make use of them.
		default:
		{
			return DefWindowProc(hwnd, umsg, wparam, lparam);
		}
	}

	return DefWindowProc(hwnd, umsg, wparam, lparam);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	if(umessage == WM_DESTROY || umessage == WM_CLOSE)
	{
		PostQuitMessage(0);
		return 0;
	}
	else
	{
		return WindowManager::getInstance()->Messagehandler(hwnd, umessage, wparam, lparam);
	}
}