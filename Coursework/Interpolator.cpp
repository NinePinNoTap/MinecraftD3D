#include "Interpolator.h"

Interpolator::Interpolator()
{
}

Interpolator::~Interpolator()
{
}

void Interpolator::begin(D3DXVECTOR3 start, D3DXVECTOR3 end, float speed, float frames)
{
	// Store start / end positions
	m_current = start;
	m_finish = end;

	// movement
	m_speed = speed;

	// Counters
	m_frame = 0;
	m_totalFrames = frames;

	// Flags
	m_finished = false;
}

D3DXVECTOR3 Interpolator::update()
{
	// move to next frame
	m_frame += m_speed;

	// Calculate the percentage we are at
	float t = m_frame / m_totalFrames;

	// Calculate the distance before we reach the target
	float DistanceLeft = distance(m_current, m_finish);

	// round to 1 decimal place to stop e numbers
	roundFloat(DistanceLeft, 1);

	// Check if we have reached the target
	if (DistanceLeft == 0)
		m_finished = true;

	// Interpolate towards target
	m_current = m_finish*t + m_current*(1.0f - t);

	return m_current;
}

bool Interpolator::isComplete()
{
	return m_finished;
}