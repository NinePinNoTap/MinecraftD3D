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

	void Start(D3DXVECTOR3 start, D3DXVECTOR3 end, float speed, float frames);

	D3DXVECTOR3 Frame();

	bool Finished();

private:
	D3DXVECTOR3 Start_;
	D3DXVECTOR3 Current_;
	D3DXVECTOR3 Finish_;

	float Speed_;
	float Distance_;
	float StartTime_;

	bool Finished_;

	float Frame_;
	float TotalFrames_;
};


