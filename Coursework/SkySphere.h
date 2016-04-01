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
	bool Initialise(const char* filename);

	// Setters
	void ToggleTime(bool);

	// Getter
	D3DXVECTOR4 GetTopColor();
	D3DXVECTOR4 GetCenterColor();

private:
	// Properties
	D3DXVECTOR4 Colors_[2][2];
};