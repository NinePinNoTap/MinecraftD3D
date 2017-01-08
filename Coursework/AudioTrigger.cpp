#include "AudioTrigger.h"

AudioTrigger::AudioTrigger()
{
	m_clip = 0;
}

AudioTrigger::~AudioTrigger()
{

}

void AudioTrigger::initialise(char* filename, float volume, D3DXVECTOR3 Position, Rect3D box)
{
	// initialise the sound
	m_clip = new AudioClip;
	m_clip->loadFile(filename, true);
	m_clip->setPosition(Position);
	m_clip->setVolume(volume);

	// Store the bounding box
	m_boundingBox = BoundingBox(box, Position);

	// initialise flags
	m_activated = false;
}

void AudioTrigger::update(D3DXVECTOR3 CameraPosition)
{
	if (m_activated)
		return;

	// Check if the camera is inside the bounds of the box
	if (checkCollision(m_boundingBox, CameraPosition))
	{
		// play the sound
		m_activated = true;
		m_clip->play(false);
	}
}