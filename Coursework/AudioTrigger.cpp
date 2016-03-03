#include "AudioTrigger.h"

AudioTrigger::AudioTrigger()
{
	Sound_ = 0;
}

AudioTrigger::~AudioTrigger()
{

}

void AudioTrigger::Initialise(char* filename, float volume, D3DXVECTOR3 Position, ObjSize box)
{
	// Initialise the sound
	Sound_ = new Sound3D;
	Sound_->LoadFile(filename, true);
	Sound_->SetPosition(Position);
	Sound_->SetVolume(volume);

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
		Sound_->Play(false);
	}
}