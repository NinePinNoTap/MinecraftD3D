#include "DirectSound.h"

DirectSound::DirectSound()
{
	DirectSound_ = 0;
	ListenerBuffer_ = 0;
	Listener_ = 0;
}

DirectSound::DirectSound(const DirectSound& other)
{
}

DirectSound::~DirectSound()
{
}

bool DirectSound::Initialise(HWND hwnd)
{
	HRESULT result;
	DSBUFFERDESC bufferDesc;
	WAVEFORMATEX waveFormat;

	//==========================
	// Initialise DirectSound3D
	//==========================

	// Initialise the direct sound interface pointer for the default sound device.
	result = DirectSoundCreate8(NULL, &DirectSound_, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// Set the cooperative level to priority so the format of the primary sound buffer can be modified.
	result = DirectSound_->SetCooperativeLevel(hwnd, DSSCL_PRIORITY);
	if (FAILED(result))
	{
		return false;
	}

	//=============================
	// Create Listener Description
	//=============================

	// Setup the primary buffer description.
	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRL3D;
	bufferDesc.dwBufferBytes = 0;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = NULL;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

	// Get control of the primary sound buffer on the default sound device.
	result = DirectSound_->CreateSoundBuffer(&bufferDesc, &ListenerBuffer_, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// Setup the format of the primary sound bufffer.
	// In this case it is a .WAV file recorded at 44,100 samples per second in 16-bit stereo (cd audio format).
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = 44100;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nChannels = 2;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;

	// Set the primary buffer to be the wave format specified.
	result = ListenerBuffer_->SetFormat(&waveFormat);
	if (FAILED(result))
	{
		return false;
	}

	// Obtain a listener interface.
	result = ListenerBuffer_->QueryInterface(IID_IDirectSound3DListener8, (LPVOID*)&Listener_);
	if (FAILED(result))
	{
		return false;
	}

	// Set the position of the listener
	SetListenerPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	return true;
}

void DirectSound::Shutdown()
{
	// Release the listener interface.
	if (Listener_)
	{
		Listener_->Release();
		Listener_ = 0;
	}

	// Release the primary sound buffer pointer.
	if (ListenerBuffer_)
	{
		ListenerBuffer_->Release();
		ListenerBuffer_ = 0;
	}

	// Release the direct sound interface pointer.
	if (DirectSound_)
	{
		DirectSound_->Release();
		DirectSound_ = 0;
	}

	return;
}

void DirectSound::SetListenerPosition(D3DXVECTOR3 Position)
{
	Listener_->SetPosition(Position.x, Position.y, Position.z, DS3D_IMMEDIATE);
}

IDirectSound8* DirectSound::GetDirectSound()
{
	return DirectSound_;
}