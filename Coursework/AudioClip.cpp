#include "AudioClip.h"

AudioClip::AudioClip()
{
	SoundBuffer_ = 0;
	SoundBuffer3D_ = 0;
	Position_ = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

AudioClip::~AudioClip()
{
	if (SoundBuffer_)
	{
		SoundBuffer_->Release();
		SoundBuffer_ = 0;
	}

	if (SoundBuffer3D_)
	{
		SoundBuffer3D_->Release();
		SoundBuffer3D_ = 0;
	}
}

// Initialising
bool AudioClip::LoadFile(const char* filename, bool Is3D)
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
	Is3DSound_ = Is3D;

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
	// Create Descriptions
	//=====================

	// Define the format of the loaded sound file
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = 44100;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nChannels = 1;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;

	// Create the description for the sound buffer
	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_CTRLVOLUME;
	bufferDesc.dwBufferBytes = waveFileHeader.dataSize;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = &waveFormat;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

	// If its a 3D sound, add the flag for it
	if (Is3DSound_)
	{
		bufferDesc.dwFlags += DSBCAPS_CTRL3D;
	}

	// Create a temporary sound buffer
	Result_ = DirectSound::Instance()->GetDirectSound()->CreateSoundBuffer(&bufferDesc, &tempBuffer, NULL);
	if (FAILED(Result_)) { return false; }

	// Test the temporary buffer against the sound interface
	Result_ = tempBuffer->QueryInterface(IID_IDirectSoundBuffer8, (void**)&SoundBuffer_);
	if (FAILED(Result_)) { return false; }

	//===========================
	// Read the File into Memory
	//===========================

	// Move to the data chunk
	fseek(filePtr, sizeof(WaveHeaderType), SEEK_SET);

	// Create the data storage
	waveData = new unsigned char[waveFileHeader.dataSize];
	if (!waveData) { return false; }

	// Read the file
	count = fread(waveData, 1, waveFileHeader.dataSize, filePtr);
	if (count != waveFileHeader.dataSize) { return false; }

	// Close the file
	error = fclose(filePtr);
	if (error != 0) { return false; }

	//==================
	// Load into Buffer
	//==================

	// Lock the sound buffer
	Result_ = SoundBuffer_->Lock(0, waveFileHeader.dataSize, (void**)&bufferPtr, (DWORD*)&bufferSize, NULL, 0, 0);
	if (FAILED(Result_)) { return false; }

	// Copy the temporary data to the buffer
	memcpy(bufferPtr, waveData, waveFileHeader.dataSize);

	// Unlock the sound buffer
	Result_ = SoundBuffer_->Unlock((void*)bufferPtr, bufferSize, NULL, 0);
	if (FAILED(Result_)) { return false; }

	// Create a 3D sound buffer if required
	if (Is3DSound_)
	{
		// Get the 3D interface to the secondary sound buffer.
		Result_ = SoundBuffer_->QueryInterface(IID_IDirectSound3DBuffer8, (void**)&SoundBuffer3D_);
		if (FAILED(Result_)) { return false; }
	}

	// Set default volume
	Result_ = SoundBuffer_->SetVolume(DSBVOLUME_MAX);
	if (FAILED(Result_)) { return false; }

	// Clean Up
	delete[] waveData;
	waveData = 0;
	
	tempBuffer->Release();
	tempBuffer = 0;

	return true;
}

// Controls
bool AudioClip::Play(bool Loop)
{
	if (!SoundBuffer_)
	{
		return false; 
	}

	// Set the buffer to start from the beginning
	Result_ = SoundBuffer_->SetCurrentPosition(0);
	if (FAILED(Result_)) { return false; }

	// Set the position of the sound
	if (Is3DSound_)
	{
		SoundBuffer3D_->SetPosition(Position_.x, Position_.y, Position_.z, DS3D_IMMEDIATE);
	}

	// Play the sound clip
	Result_ = SoundBuffer_->Play(0, 0, Loop);
	if (FAILED(Result_)) { return false; }

	return true;
}

bool AudioClip::Stop()
{
	Result_ = SoundBuffer_->Stop();
	if (FAILED(Result_)) { return false; }

	return true;
}

bool AudioClip::SetVolume(float Vol)
{
	// Convert from a percentage of normal volume to a percentage of reduction
	Vol = 1.0f - Vol;

	// Keep within a range of 0 to 1
	Clamp(Vol, 0, 1);

	// Convert to a decimal reduction
	Vol *= -10000;

	// Apply the volume change to the buffer
	Result_ = SoundBuffer_->SetVolume(Vol);
	if (FAILED(Result_)) { return false; }

	return true;
}

// Setters
void AudioClip::SetPosition(D3DXVECTOR3 Position)
{
	Position_ = Position;
}

// Getters
bool AudioClip::IsPlaying()
{
	DWORD dwStatus = 0;
	SoundBuffer_->GetStatus(&dwStatus);
	return ((dwStatus & DSBSTATUS_PLAYING) != 0);
}