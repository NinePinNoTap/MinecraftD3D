#include "Interpolator.h"

Interpolator::Interpolator()
{
}

Interpolator::~Interpolator()
{
}

void Interpolator::Start(D3DXVECTOR3 start, D3DXVECTOR3 end, float speed, float frames)
{
	// Store start / end positions
	Current_ = start;
	Finish_ = end;

	// Movement
	Speed_ = speed;

	// Counters
	Frame_ = 0;
	TotalFrames_ = frames;

	// Flags
	Finished_ = false;
}

D3DXVECTOR3 Interpolator::Frame()
{
	// Move to next frame
	Frame_ += Speed_;

	// Calculate the percentage we are at
	float t = Frame_ / TotalFrames_;

	// Calculate the distance before we reach the target
	float DistanceLeft = Distance(Current_, Finish_);

	// Round to 1 decimal place to stop e numbers
	Round(DistanceLeft, 1);

	// Check if we have reached the target
	if (DistanceLeft == 0)
		Finished_ = true;

	// Interpolate towards target
	Current_ = Finish_*t + Current_*(1.0f - t);

	return Current_;
}

bool Interpolator::Finished()
{
	return Finished_;
}