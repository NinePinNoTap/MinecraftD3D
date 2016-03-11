#pragma once

#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")

#include <windows.h>
#include <mmsystem.h>
#include <dsound.h>
#include <stdio.h>
#include <string>
#include <d3d11.h>
#include <d3dx10math.h>

#include "Camera.h"
#include "Singleton.h"
#include "WaveHeader.h"

using namespace std;

class DirectSound : public Singleton<DirectSound>
{
public:
	DirectSound();
	DirectSound(const DirectSound&);
	~DirectSound();

	bool Initialise(HWND);
	void Shutdown();

	void Frame();

	IDirectSound8* GetDirectSound();

private:
	IDirectSound8* DirectSound_;
	IDirectSoundBuffer* ListenerBuffer_;
	IDirectSound3DListener8* Listener_;
};


