#pragma once

#include "DataStructs.h"
#include "Sound3D.h"
#include "Utilities.h"

class AudioTrigger
{
public:
	AudioTrigger();
	~AudioTrigger();

	void Initialise(char* filename, float volume, D3DXVECTOR3 Position, ObjSize box);

	void Frame(D3DXVECTOR3 CameraPosition);

private:
	Sound3D* Sound_;
	BoundingBox BoundingBox_;
	bool Activated_;
};


