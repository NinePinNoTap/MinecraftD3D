#include "AudioClip.h"

AudioClip::AudioClip()
{
	m_soundBuffer = 0;
	m_soundBuffer3D = 0;
	m_position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

AudioClip::~AudioClip()
{
	if (m_soundBuffer)
	{
		m_soundBuffer->Release();
		m_soundBuffer = 0;
	}

	if (m_soundBuffer3D)
	{
		m_soundBuffer3D->Release();
		m_soundBuffer3D = 0;
	}
}

// Initialising
bool AudioClip::loadFile(const char* filename, bool is3D)
{
	int error;
	FILE* filePtr;
	unsigned int count;
	WaveHeaderType waveFileHeader;
	WAVEFORMATEX waveFormat;
	DSBUFFERDESC bufferDesc;
	IDirectSoundBuffer* tempBuffer;
	unsigned char* waveData;
	unsigned char* bufferPtr;
	unsigned long bufferSize;

	// Store whether its a 3D sound or not
	m_is3D = is3D;

	//==================
	// Reading the file
	//==================

	// Open the wave file in binary.
	error = fopen_s(&filePtr, filename, "rb");
	if (error != 0)
	{
		return false;
	}

	// Read in the wave file header.
	count = fread(&waveFileHeader, sizeof(waveFileHeader), 1, filePtr);
	if (count != 1)
	{
		return false;
	}

	// Make sure the file is the right format
	if (string(waveFileHeader.chunkId).find("RIFF") != 0 ||
		string(waveFileHeader.format).find("WAVE") != 0 ||
		string(waveFileHeader.subChunkId).find("fmt") != 0 ||
		waveFileHeader.audioFormat != WAVE_FORMAT_PCM ||
		waveFileHeader.numChannels != 1 ||
		waveFileHeader.sampleRate != 44100 ||
		waveFileHeader.bitsPerSample != 16 ||
		string(waveFileHeader.dataChunkId).find("data") != 0)
	{
		return false;
	}

	//=====================
	// create Descriptions
	//=====================

	// Define the format of the loaded sound file
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = 44100;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nChannels = 1;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;

	// create the description for the sound buffer
	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_CTRLVOLUME;
	bufferDesc.dwBufferBytes = waveFileHeader.dataSize;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = &waveFormat;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

	// If its a 3D sound, add the flag for it
	if (m_is3D)
	{
		bufferDesc.dwFlags += DSBCAPS_CTRL3D;
	}

	// create a temporary sound buffer
	m_result = DirectSound::getInstance()->getDirectSound()->CreateSoundBuffer(&bufferDesc, &tempBuffer, NULL);
	if (FAILED(m_result))
	{
		return false;
	}

	// Test the temporary buffer against the sound interface
	m_result = tempBuffer->QueryInterface(IID_IDirectSoundBuffer8, (void**)&m_soundBuffer);
	if (FAILED(m_result))
	{
		return false;
	}

	//===========================
	// Read the File into Memory
	//===========================

	// move to the data chunk
	fseek(filePtr, sizeof(WaveHeaderType), SEEK_SET);

	// create the data storage
	waveData = new unsigned char[waveFileHeader.dataSize];
	if (!waveData) 
	{
		return false;
	}

	// Read the file
	count = fread(waveData, 1, waveFileHeader.dataSize, filePtr);
	if (count != waveFileHeader.dataSize)
	{
		return false;
	}

	// Close the file
	error = fclose(filePtr);
	if (error != 0)
	{
		return false;
	}

	//==================
	// onload into Buffer
	//==================

	// Lock the sound buffer
	m_result = m_soundBuffer->Lock(0, waveFileHeader.dataSize, (void**)&bufferPtr, (DWORD*)&bufferSize, NULL, 0, 0);
	if (FAILED(m_result))
	{
		return false;
	}

	// Copy the temporary data to the buffer
	memcpy(bufferPtr, waveData, waveFileHeader.dataSize);

	// Unlock the sound buffer
	m_result = m_soundBuffer->Unlock((void*)bufferPtr, bufferSize, NULL, 0);
	if (FAILED(m_result))
	{
		return false;
	}

	// create a 3D sound buffer if required
	if (m_is3D)
	{
		// get the 3D interface to the secondary sound buffer.
		m_result = m_soundBuffer->QueryInterface(IID_IDirectSound3DBuffer8, (void**)&m_soundBuffer3D);
		if (FAILED(m_result))
		{
			return false;
		}
	}

	// set default volume
	m_result = m_soundBuffer->SetVolume(DSBVOLUME_MAX);
	if (FAILED(m_result))
	{
		return false;
	}

	// Clean Up
	delete[] waveData;
	waveData = 0;
	
	tempBuffer->Release();
	tempBuffer = 0;

	return true;
}

void AudioClip::terminate()
{
	if (m_soundBuffer)
	{
		m_soundBuffer->Release();
		m_soundBuffer = 0;
	}

	if (m_soundBuffer3D)
	{
		m_soundBuffer3D->Release();
		m_soundBuffer3D = 0;
	}
}

// Controls
bool AudioClip::play(bool Loop)
{
	if (!m_soundBuffer)
	{
		return false; 
	}

	// set the buffer to start from the beginning
	m_result = m_soundBuffer->SetCurrentPosition(0);
	if (FAILED(m_result))
	{
		return false;
	}

	// set the position of the sound
	if (m_is3D)
	{
		m_soundBuffer3D->SetPosition(m_position.x, m_position.y, m_position.z, DS3D_IMMEDIATE);
	}

	// play the sound clip
	m_result = m_soundBuffer->Play(0, 0, Loop);
	if (FAILED(m_result))
	{
		return false;
	}

	return true;
}

bool AudioClip::stop()
{
	m_result = m_soundBuffer->Stop();
	if (FAILED(m_result))
	{
		return false;
	}

	return true;
}

bool AudioClip::setVolume(float Vol)
{
	// Convert from a percentage of normal volume to a percentage of reduction
	Vol = 1.0f - Vol;

	// Keep within a range of 0 to 1
	clamp(Vol, 0, 1);

	// Convert to a decimal reduction
	Vol *= -10000;

	// Apply the volume change to the buffer
	m_result = m_soundBuffer->SetVolume(Vol);
	if (FAILED(m_result))
	{
		return false;
	}

	return true;
}

// setters
void AudioClip::setPosition(D3DXVECTOR3 Position)
{
	m_position = Position;
}

// getters
bool AudioClip::isPlaying()
{
	DWORD dwStatus = 0;
	m_soundBuffer->GetStatus(&dwStatus);

	return ((dwStatus & DSBSTATUS_PLAYING) != 0);
}