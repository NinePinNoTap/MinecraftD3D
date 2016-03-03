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
	bool LoadFile(char* filename, bool Is3D);

	// Controls
	bool Play(bool Loop);
	bool Stop();

	// Setters
	void SetPosition(D3DXVECTOR3 Position);
	bool SetVolume(float Vol);

	// Getters
	bool IsPlaying();

private:
	// Buffers
	IDirectSoundBuffer8* SoundBuffer_;
	IDirectSound3DBuffer8* SoundBuffer3D_;

	// Flags
	bool Is3DSound_;
	HRESULT Result_;

	// Properties
	D3DXVECTOR3 Position_;
};


