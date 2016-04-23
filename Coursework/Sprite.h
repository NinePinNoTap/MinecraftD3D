#pragma once

#include <d3d11.h>
#include <d3dx10math.h>

#include "DirectXManager.h"

#include "GameObject.h"
#include "Model.h"
#include "Rect.h"
#include "PrimitiveFactory.h"

class Sprite : public GameObject
{
public:
	Sprite();
	Sprite(const Sprite&);
	~Sprite();
	
	// Initialising
	virtual bool Initialise(Rect3D spriteResolution, string textureFilename);

	// Frame
	bool Render();
};