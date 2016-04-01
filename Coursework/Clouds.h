#pragma once

#include <d3d11.h>
#include <d3dx10math.h>
#include <vector>

using namespace std;

#include "GameObject.h"
#include "PrimitiveFactory.h"
#include "Texture.h"


class Clouds : public GameObject
{
public:
	Clouds();
	Clouds(const Clouds&);
	~Clouds();

	// Initialising
	bool Initialise(string cloudTextureFilename, string perturbTextureFilename);

	// Frame
	bool Frame();

	// Getters
	float GetScale();
	float GetBrightness();

private:
	// Properties
	float ScaleFactor_;
	float Brightness_;
};


