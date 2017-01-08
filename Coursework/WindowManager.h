#pragma once

#define WIN32_LEAN_AND_MEAN

#include <windows.h>

#include "ApplicationManager.h"
#include "Rect.h"
#include "Singleton.h"

class WindowManager : public Singleton<WindowManager>
{
public:
	WindowManager();
	WindowManager(const WindowManager&);
	~WindowManager();

	bool initialise();
	void terminate();
	void run();

	inline Rect2D getWindowResolution() { return m_windowResolution; }
	inline HWND getHWND() { return m_hwnd; }

	LRESULT CALLBACK Messagehandler(HWND, UINT, WPARAM, LPARAM);

private:
	void createWindow(int&, int&);

	LPCWSTR m_applicationName;
	HINSTANCE m_hinstance;
	HWND m_hwnd;
	ApplicationManager* m_applicationManager;
	Rect2D m_windowResolution;

	bool m_result;
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


