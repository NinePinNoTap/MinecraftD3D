#pragma once

#include <d3d11.h>
#include <d3dx10math.h>

#include <cmath>
#include <math.h>

#include "Utilities.h"

class Interpolator
{
public:
	Interpolator();
	~Interpolator();

	void begin(D3DXVECTOR3 start, D3DXVECTOR3 end, float speed, float frames);

	D3DXVECTOR3 update();

	bool isComplete();

private:
	D3DXVECTOR3 m_start;
	D3DXVECTOR3 m_current;
	D3DXVECTOR3 m_finish;

	float m_speed;
	float m_distance;
	float m_startTime;

	bool m_finished;

	float m_frame;
	float m_totalFrames;
};


