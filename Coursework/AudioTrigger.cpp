#include "AudioTrigger.h"

AudioTrigger::AudioTrigger()
{
	Clip_ = 0;
}

AudioTrigger::~AudioTrigger()
{

}

void AudioTrigger::Initialise(char* filename, float volume, D3DXVECTOR3 Position, Rect3D box)
{
	// Initialise the sound
	Clip_ = new AudioClip;
	Clip_->LoadFile(filename, true);
	Clip_->SetPosition(Position);
	Clip_->SetVolume(volume);

	// Store the bounding box
	BoundingBox_ = BoundingBox(box, Position);

	// Initialise flags
	Activated_ = false;
}

void AudioTrigger::Frame(D3DXVECTOR3 CameraPosition)
{
	if (Activated_)
		return;

	// Check if the camera is inside the bounds of the box
	if (CheckCollision(BoundingBox_, CameraPosition))
	{
		// Play the sound
		Activated_ = true;
		Clip_->Play(false);
	}
}