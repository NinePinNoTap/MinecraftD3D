#pragma once

#include <d3d11.h>
#include <d3dx10math.h>

#include "Constants.h"
#include "Model.h"
#include "DataStructs.h"
#include "Primitive.h"

class Sprite : public Primitive
{
public:
	Sprite();
	Sprite(const Sprite&);
	~Sprite();

	bool Initialise(ObjSize Dimensions);
	bool SetTexture(WCHAR* filename);
};


