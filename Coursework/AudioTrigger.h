#pragma once


#include "AudioClip.h"
#include "Utilities.h"

class AudioTrigger
{
public:
	AudioTrigger();
	~AudioTrigger();

	void initialise(char* filename, float volume, D3DXVECTOR3 Position, Rect3D box);

	void update(D3DXVECTOR3 CameraPosition);

private:
	AudioClip* m_clip;
	BoundingBox m_boundingBox;
	bool m_activated;
};


