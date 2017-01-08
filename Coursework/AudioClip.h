#pragma once

#include <d3d11.h>
#include <d3dx10math.h>
#include <dsound.h>
#include <string>

#include "DirectSound.h"
#include "Utilities.h"
#include "WaveHeader.h"

class AudioClip
{
public:
	AudioClip();
	~AudioClip();

	// Initialising
	bool loadFile(const char* filename, bool is3D);
	void terminate();

	// Controls
	bool play(bool Loop);
	bool stop();

	// setters
	void setPosition(D3DXVECTOR3 Position);
	bool setVolume(float Vol);

	// getters
	bool isPlaying();

private:
	// Buffers
	IDirectSoundBuffer8* m_soundBuffer;
	IDirectSound3DBuffer8* m_soundBuffer3D;

	// Flags
	bool m_is3D;
	HRESULT m_result;

	// Properties
	D3DXVECTOR3 m_position;
};


