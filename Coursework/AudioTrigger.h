#pragma once

#include "Camera.h"
#include "AudioClip.h"
#include "Utilities.h"

class AudioTrigger
{
public:
	AudioTrigger();
	~AudioTrigger();

	void Initialise(char* filename, float volume, D3DXVECTOR3 Position, Rect3D box);

	void Frame();

private:
	AudioClip* Clip_;
	BoundingBox BoundingBox_;
	bool Activated_;
};


