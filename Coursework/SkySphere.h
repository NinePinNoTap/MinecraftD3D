#pragma once

#include <d3d11.h>
#include <d3dx10math.h>
#include <fstream>

#include "AssetManager.h"
#include "GameObject.h"

using namespace std;

class SkySphere : public GameObject
{
public:
	SkySphere();
	SkySphere(const SkySphere&);
	~SkySphere();

	// Initialising
	bool initialise(const char* filename);

	// update
	bool update();
	bool render();

	// setters
	void toggleTime(bool);

	// getter
	D3DXVECTOR4 getTopColor();
	D3DXVECTOR4 getCenterColor();

private:
	// Properties
	D3DXVECTOR4 m_colours[2][2];
};