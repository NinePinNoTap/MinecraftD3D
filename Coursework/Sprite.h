#pragma once

#include <d3d11.h>
#include <d3dx10math.h>

#include "Constants.h"
#include "Model.h"
#include "Rect.h"
#include "Primitive.h"

class Sprite : public Primitive
{
public:
	Sprite();
	Sprite(const Sprite&);
	~Sprite();

	bool Initialise(Rect3D Dimensions);
	bool SetTexture(WCHAR* filename);
};


