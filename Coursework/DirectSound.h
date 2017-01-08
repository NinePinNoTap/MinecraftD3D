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

#include "Singleton.h"
#include "WaveHeader.h"

using namespace std;

class DirectSound : public Singleton<DirectSound>
{
public:
	DirectSound();
	DirectSound(const DirectSound&);
	~DirectSound();

	bool initialise(HWND);
	void terminate();

	void setListenerPosition(D3DXVECTOR3 Position);

	IDirectSound8* getDirectSound();

private:
	IDirectSound8* m_directSound;
	IDirectSoundBuffer* m_listenerBuffer;
	IDirectSound3DListener8* m_listener;
};


