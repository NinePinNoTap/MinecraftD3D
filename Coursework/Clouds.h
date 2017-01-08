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
	bool initialise(string cloudTextureFilename, string perturbTextureFilename);

	// update
	bool update();
	bool render();

	// getters
	float getScale();
	float getBrightness();

private:
	// Properties
	float m_scaleFactor;
	float m_brightness;
};


