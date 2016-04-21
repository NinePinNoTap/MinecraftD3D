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

	bool Initialise();
	void Shutdown();
	void Run();

	Rect2D GetWindowResolution();
	HWND GetHWND() { return HWND_; }

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	void InitialiseWindows(int&, int&);

	LPCWSTR ApplicationManagerName_;
	HINSTANCE HInstance_;
	HWND HWND_;
	ApplicationManager* ApplicationManager_;
	Rect2D WindowResolution_;

	bool Result_;
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


