#include "DirectSound.h"

DirectSound::DirectSound()
{
	m_directSound = 0;
	m_listenerBuffer = 0;
	m_listener = 0;
}

DirectSound::DirectSound(const DirectSound& other)
{
}

DirectSound::~DirectSound()
{
}

bool DirectSound::initialise(HWND hwnd)
{
	HRESULT result;
	DSBUFFERDESC bufferDesc;
	WAVEFORMATEX waveFormat;

	//==========================
	// initialise DirectAudioClip
	//==========================

	// initialise the direct sound interface pointer for the default sound device.
	result = DirectSoundCreate8(NULL, &m_directSound, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// set the cooperative level to priority so the format of the primary sound buffer can be modified.
	result = m_directSound->SetCooperativeLevel(hwnd, DSSCL_PRIORITY);
	if (FAILED(result))
	{
		return false;
	}

	//=============================
	// create Listener Description
	//=============================

	// setup the primary buffer description.
	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRL3D;
	bufferDesc.dwBufferBytes = 0;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = NULL;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

	// get control of the primary sound buffer on the default sound device.
	result = m_directSound->CreateSoundBuffer(&bufferDesc, &m_listenerBuffer, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// setup the format of the primary sound bufffer.
	// In this case it is a .WAV file recorded at 44,100 samples per second in 16-bit stereo (cd audio format).
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = 44100;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nChannels = 2;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;

	// set the primary buffer to be the wave format specified.
	result = m_listenerBuffer->SetFormat(&waveFormat);
	if (FAILED(result))
	{
		return false;
	}

	// Obtain a listener interface.
	result = m_listenerBuffer->QueryInterface(IID_IDirectSound3DListener8, (LPVOID*)&m_listener);
	if (FAILED(result))
	{
		return false;
	}

	// set the position of the listener
	setListenerPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	return true;
}

void DirectSound::terminate()
{
	// Release the listener interface.
	if (m_listener)
	{
		m_listener->Release();
		m_listener = 0;
	}

	// Release the primary sound buffer pointer.
	if (m_listenerBuffer)
	{
		m_listenerBuffer->Release();
		m_listenerBuffer = 0;
	}

	// Release the direct sound interface pointer.
	if (m_directSound)
	{
		m_directSound->Release();
		m_directSound = 0;
	}

	return;
}

void DirectSound::setListenerPosition(D3DXVECTOR3 Position)
{
	m_listener->SetPosition(Position.x, Position.y, Position.z, DS3D_IMMEDIATE);
}

IDirectSound8* DirectSound::getDirectSound()
{
	return m_directSound;
}